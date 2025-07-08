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

#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include "../../../include/memory/vaddr.h"
#include "../../../include/utils.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  cleanup_regex();
  nemu_state.state = NEMU_QUIT;
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
    int step = 1;
    if (args) {
        step = atoi(args);
    }
    cpu_exec(step);
    return 0;
}

static int cmd_info(char *args) {
    if (strcmp(args, "r") == 0) {
        isa_reg_display();
        // printf("222\n");
    } else if (strcmp(args, "w") == 0) {
        watchpoint_display();
    } else {
        printf("Unknown subcommand: %s\n", args);
    }
    return 0;
}

static int cmd_x(char *args) {
  int n;
  vaddr_t addr;
  if (sscanf(args, "%d %x", &n, &addr) != 2) {
      printf("Usage: x N EXPR\n");
      return 0;
  }
  for(int i=0;i<n;i++){
    printf("%08x:",addr+i*4);
    printf("%08x\n",vaddr_read(addr+i*4,4));
  }
  return 0;
}

static int cmd_p(char *args){
  bool success;
  // printf("111\n");
  printf("%d\n",expr(args , &success));
  return 0;
}

static int cmd_w(char *args) {
  if (!args) {
    printf("Usage: w EXPR\n");
    return 0;
  }
  WP *wp = new_wp();
  strncpy(wp->expr, args, sizeof(wp->expr) - 1);
  wp->expr[sizeof(wp->expr) - 1] = '\0';
  bool success;
  wp->old_val = expr(args, &success);
  if (!success) {
    printf("Error: Invalid expression '%s'\n", args);
    free_wp(wp);
    return 0;
  }
  printf("Watchpoint %d: %s\n", wp->NO, wp->expr);
  return 0;
}

static int cmd_d(char *args) {
  if (!args) {
    printf("Usage: d N\n");
    return 0;
  }
  char *endptr;
  long no = strtol(args, &endptr, 10);
  if (*endptr != '\0' || no < 0) {
    printf("Error: Invalid watchpoint number '%s'\n", args);
    return 0;
  }
  WP *wp = find_wp((int)no); // Use find_wp instead of direct head access
  if (!wp) {
    printf("Error: Watchpoint %d not found\n", (int)no);
    return 0;
  }
  free_wp(wp);
  printf("Watchpoint %d deleted\n", (int)no);
  return 0;
}
static struct {
    const char *name;
    const char *description;
    int (*handler)(char *);
} cmd_table[] = {
    { "help", "Display information about all supported commands", cmd_help },
    { "c", "Continue the execution of the program", cmd_c },
    { "q", "Exit NEMU", cmd_q },
    { "si", "Single step execution (si [N])", cmd_si },
    { "info", "Print program status (info SUBCMD)", cmd_info },
    { "x", "Scan memory (x N EXPR)", cmd_x },
    { "p", "Evaluate expression (p EXPR)", cmd_p },
    { "w", "Set watchpoint (w EXPR)", cmd_w },
    { "d", "Delete watchpoint (d N)", cmd_d },
    { NULL, NULL, NULL }  // 结束标记
};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;
  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
