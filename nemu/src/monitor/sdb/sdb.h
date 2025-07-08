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

#ifndef __SDB_H__
#define __SDB_H__

#include <common.h>

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  char expr[32];
  uint32_t old_val;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

uint32_t expr(char *e, bool *success);
void init_regex();
void cleanup_regex();
void init_wp_pool();
void sdb_set_batch_mode();
void sdb_mainloop();
void init_sdb();
WP* new_wp();
WP* find_wp(int no);
void free_wp(WP *wp);
void watchpoint_display();
void check_watchpoints();

#endif
