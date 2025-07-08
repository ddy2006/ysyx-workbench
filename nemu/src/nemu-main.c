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

#include <common.h>
#include <isa.h>
#include <cpu/cpu.h>
#include "monitor/sdb/sdb.h"
#include "memory/vaddr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  int i;
  char *input_file = NULL;

  for (i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-t") == 0 && i + 1 < argc) {
      input_file = argv[i + 1];
      i++;
    }
  }

  if (input_file) {
    FILE *fp = fopen(input_file, "r");
    if (!fp) {
      fprintf(stderr, "Error: Cannot open input file '%s'\n", input_file);
      return 1;
    }

    char line[65536];
    int test_count = 0;
    int pass_count = 0;

    init_regex();

    printf("Running expression tests from '%s'...\n", input_file);
    while (fgets(line, sizeof(line), fp)) {
      test_count++;
      
      uint32_t expected;
      char expr_str[65536];
      if (sscanf(line, "%u %[^\n]", &expected, expr_str) != 2) {
        printf("Test #%d: Invalid format: %s", test_count, line);
        continue;
      }

      bool success;
      uint32_t result = expr(expr_str, &success);

      if (success && result == expected) {
        printf("Test #%d: %s = %u [PASS]\n", test_count, expr_str, result);
        pass_count++;
      } else {
        printf("Test #%d: %s = %u [FAIL] (Expected: %u, Got: %u, Success: %d)\n",
               test_count, expr_str, result, expected, result, success);
      }
    }

    fclose(fp);
    printf("Test Summary: Passed %d/%d tests\n", pass_count, test_count);
    cleanup_regex();
    return pass_count == test_count ? 0 : 1;
  }
/* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

/* Start engine. */
  engine_start();
  return is_exit_status_bad();
}