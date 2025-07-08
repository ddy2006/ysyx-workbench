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

#include "sdb.h"
#include "../../../include/utils.h"

#define NR_WP 32

// typedef struct watchpoint {
//   int NO;
//   char expr[32];
//   uint32_t old_val;
//   struct watchpoint *next;

//   /* TODO: Add more members if necessary */

// } WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;
WP* new_wp();
WP* find_wp(int no);
void free_wp(WP *wp);

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].expr[0]= '\0';
    wp_pool[i].old_val= 0;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

void watchpoint_display() {
  if (!head) {
    printf("No watchpoints\n");
    return;
  }
  printf("Num     Type       Disp Enb Expression\n");
  WP *wp = head;
  while (wp) {
    bool success;
    uint32_t new_val = expr(wp->expr, &success);
    printf("%-7d watchpoint keep y   %s = %u\n", wp->NO, wp->expr, success ? new_val : 0);
    wp = wp->next;
  }
}

void check_watchpoints() {
  for (WP *wp = head; wp; wp = wp->next) {
    bool success;
    uint32_t new_val = expr(wp->expr, &success);
    if (success && new_val != wp->old_val) {
      printf("Watchpoint %d triggered: %s changed from 0x%x to 0x%x\n",
             wp->NO, wp->expr, wp->old_val, new_val);
      wp->old_val = new_val;
      nemu_state.state = NEMU_STOP;
      break;
    }
  }
}

WP* new_wp(){
  if(free_==NULL){
    assert(0);
  }
  WP *wp=free_;
  free_=free_->next;
  wp->next=head;
  head=wp;
  wp->expr[0] = '\0';
  wp->old_val = 0;
  return wp;
}


void free_wp(WP *wp) {
  if (wp == NULL) {
    printf("Error: Cannot free NULL watchpoint\n");
    return;
  }
  // Find wp in head list
  WP **curr = &head;
  while (*curr && *curr != wp) {
    curr = &(*curr)->next;
  }
  if (*curr == wp) {
    *curr = wp->next;
    wp->next = free_;
    free_ = wp;
    wp->expr[0] = '\0';
    wp->old_val = 0;
  } else {
    printf("Error: Watchpoint %d not found in used list\n", wp->NO);
  }
}

WP* find_wp(int no) {
  WP *wp = head;
  while (wp && wp->NO != no) {
    wp = wp->next;
  }
  return wp;
}