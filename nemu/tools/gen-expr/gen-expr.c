
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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// Global buffer for generated expression
static char buf[1024] = {};
static char code_buf[1024 + 128] = {};
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned long result = %s; "
"  printf(\"%%lu\", result); "
"  return 0; "
"}";

uint32_t choose(uint32_t n) {
  return rand() % n;
}

static void gen_num() {
  uint32_t num = rand() % 100 + 1;
  char tmp[32];
  snprintf(tmp, sizeof(tmp), "%u", num);
  strcat(buf, tmp);
}

// Generate a random operator
static void gen_rand_op() {
  switch (choose(4)) {
    case 0: strcat(buf, "+"); break;
    case 1: strcat(buf, "-"); break;
    case 2: strcat(buf, "*"); break;
    case 3: strcat(buf, "/"); break;
  }
}

// Generate a random expression
void gen_rand_expr() {
  // Control recursion depth to avoid long expressions
  static int depth = 0;
  if (strlen(buf) > 1000 || depth > 3) {
    gen_num(); // Fallback to number
    return;
  }
  depth++;
  switch (choose(3)) {
    case 0: // Number
      gen_num();
      break;
    case 1: // (expr)
      strcat(buf, "(");
      gen_rand_expr();
      strcat(buf, ")");
      break;
    default: // expr op expr
      gen_rand_expr();
      gen_rand_op();
      gen_rand_expr();
      break;
  }
  depth--;
}

int main(int argc, char *argv[]) {
  srand(time(0));
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  for (int i = 0; i < loop; i++) {
    buf[0] = '\0'; // Clear buffer
    gen_rand_expr();
    sprintf(code_buf, code_format, buf); // Embed expression
    FILE *fp = fopen("/tmp/.code.c", "w");
    if (!fp) {
      perror("Failed to open /tmp/.code.c");
      return 1;
    }
    fputs(code_buf, fp);
    fclose(fp);
    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) {
      fprintf(stderr, "GCC compilation failed\n");
      return 1;
    }
    FILE *pipe = popen("/tmp/.expr", "r");
    if (!pipe) {
      perror("Failed to run /tmp/.expr");
      continue;
      // return 1;
    }
    uint64_t result;
    if (fscanf(pipe, "%lu", &result) != 1) { // Check fscanf return
      fprintf(stderr, "Failed to read result\n");
      pclose(pipe);
      continue;
      // return 1;
    }
    pclose(pipe);
    printf("%lu %s\n", result, buf); // Output test case
  }
  return 0;
}