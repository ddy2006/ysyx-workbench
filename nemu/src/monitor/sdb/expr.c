/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include <regex.h>
#include <isa.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_HEX, TK_REG, TK_NEQ, TK_AND, TK_DEREF
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {
  {" +", TK_NOTYPE},        
  {"\\+", '+'},              
  {"\\-", '-'},              
  {"\\*", '*'},              
  {"\\/", '/'},              
  {"\\(", '('},              
  {"\\)", ')'},            
  {"0x[0-9a-fA-F]+", TK_HEX},
  {"[0-9]+", TK_NUM},
  {"==", TK_EQ},             
  {"!=", TK_NEQ},            
  {"&&", TK_AND},            
  {"\\$[a-zA-Z0-9]+", TK_REG} 
};

#define NR_REGEX ARRLEN(rules)

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[1024];
static int nr_token = 0;

static regex_t re [NR_REGEX];

void cleanup_regex() {
  for (int i = 0; i < NR_REGEX; i++) {
    regfree(&re[i]);
  }
}

void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      printf("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
      exit(1);
    }
  }
}

static bool check_parentheses(int p, int q) {
  if (tokens[p].type != '(' || tokens[q].type != ')') {
    return false;
  }
  
  int balance = 0;
  for (int i = p; i <= q; i++) {
    if (tokens[i].type == '(') balance++;
    else if (tokens[i].type == ')') balance--;
    
    if (balance < 0) return false; // Unmatched closing parenthesis
    if (balance == 0 && i < q) return false; // Early closing
  }
  return balance == 0;
}

static int find_main_op(int p, int q) {
  int balance = 0;
  int op_pos = -1;
  int priority = 100; 
  int last_op_pos = -1;
  
  for (int i = q; i >= p; i--) {
    if (tokens[i].type == ')') balance++;
    else if (tokens[i].type == '(') balance--;
    
    if (balance != 0) continue;
    
    int curr_priority = -1;
    bool is_unary = (i == p || tokens[i-1].type == '(' || 
                     tokens[i-1].type == '+' || tokens[i-1].type == '-' || 
                     tokens[i-1].type == '*' || tokens[i-1].type == '/' || 
                     tokens[i-1].type == TK_EQ || tokens[i-1].type == TK_NEQ || 
                     tokens[i-1].type == TK_AND);
    
    switch (tokens[i].type) {
      case '+':
      case '-': curr_priority = 2; break;
      case '*':
        if (is_unary) curr_priority = 4;
        else curr_priority = 3; 
        break;
      case '/': curr_priority = 3; break;
      case TK_EQ:
      case TK_NEQ: curr_priority = 0; break;
      case TK_AND: curr_priority = 1; break;
    }
    
    if (curr_priority >= 0 && curr_priority < priority) {
      priority = curr_priority;
      last_op_pos = i;
    }
  }
  
  op_pos = last_op_pos;
  return op_pos;
}

static uint32_t eval(int p, int q) {
  if (p > q) {
    printf("Error: Bad expression\n");
    return 0;
  }
  else if (p == q) {
    if (tokens[p].type == TK_NUM) {
      return atoi(tokens[p].str);
    } else if (tokens[p].type == TK_HEX) {
      return strtoul(tokens[p].str, NULL, 16);
    } else if (tokens[p].type == TK_REG) {
      bool success;
      uint32_t val;
      char *reg_name = tokens[p].str + 1; // Skip '$'
      // printf("1\n");
      val = isa_reg_str2val(reg_name, &success);
      return val;
    } else {
      printf("Error: Invalid single token\n");
      return 0;
    }
  }
  else if (check_parentheses(p, q)) {
    return eval(p + 1, q - 1);
  }
  else {
    int op_pos = find_main_op(p, q);
    if (op_pos == -1) {
      if (tokens[p].type == '*' && (p == 0 || tokens[p-1].type == '(' || 
          tokens[p-1].type == '+' || tokens[p-1].type == '-' || 
          tokens[p-1].type == '*' || tokens[p-1].type == '/' || 
          tokens[p-1].type == TK_EQ || tokens[p-1].type == TK_NEQ || 
          tokens[p-1].type == TK_AND)) {
        int32_t addr = eval(p + 1, q);
        return addr;
      }
      return 0;
    }
    
    int32_t val1 = (p < op_pos) ? eval(p, op_pos - 1) : 0;
    int32_t val2 = (op_pos < q) ? eval(op_pos + 1, q) : 0;
    // printf("val1=%d\nop_pos='%c'\nval2=%d\n",val1,tokens[op_pos].type,val2);
    
    switch (tokens[op_pos].type) {
      case '+': return val1 + val2;
      case '-': return val1 - val2;
      case '*': return val1 * val2;
      case '/': 
        if (val2 == 0) {
          printf("Error: Division by zero\n");
          return 0;
        }
        return val1 / val2;
      case TK_EQ: return val1 == val2;
      case TK_NEQ: return val1 != val2;
      case TK_AND: return val1 && val2;
      default:
        printf("Error: Unknown operator\n");
        return 0;
    }
  }
}

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0' && nr_token < 32) {
    for (i = 0; i < NR_REGEX; i++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        position += substr_len;

        if (rules[i].token_type == TK_NOTYPE) {
          break; // Skip spaces
        }

        if (substr_len >= sizeof(tokens[nr_token].str)) {
          printf("Token too long at position %d\n", position - substr_len);
          return false;
        }

        tokens[nr_token].type = rules[i].token_type;
        strncpy(tokens[nr_token].str, substr_start, substr_len);
        tokens[nr_token].str[substr_len] = '\0';

        if (tokens[nr_token].type == '*' && nr_token > 0) {
          int prev_type = tokens[nr_token-1].type;
          if (prev_type == '(' || prev_type == '+' || prev_type == '-' || 
              prev_type == '*' || prev_type == '/' || prev_type == TK_EQ || 
              prev_type == TK_NEQ || prev_type == TK_AND) {
            tokens[nr_token].type = TK_DEREF;
          }
         } else if (tokens[nr_token].type == '*' && nr_token == 0) {
          tokens[nr_token].type = TK_DEREF;
        }

        nr_token++;
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("No match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

int32_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  
  *success = true;
  return eval(0, nr_token - 1);
}