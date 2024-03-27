#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG

#define assert_char(C1, C2) assert(char_test(C1, C2))
#define assert_char_arr(C1, C2) assert(bit_arr_test(C1, C2))
#define assert_int(I1, I2) assert(int_test(I1, I2))
#define assert_int_arr(I1, ...) assert(int_arr_test(I1, __VA_ARGS__))

void int_arr_init_immed(int arr[], int n, ...);
poss_p poss_init_immed(int N, int n, ...);

bool char_test(char c1, char c2);
bool bit_arr_test(char b1[], char str[]);

bool int_test(int i1, int i2);
bool int_arr_test(int arr[], int n, ...);
bool poss_test(poss_p p1, int N, int n, ...);

#endif

char* bit_arr_create(int N, int n, int spaces[], int bars[]);

bool table_scan_line(table_p t, int i);
bool table_scan_column(table_p t, int j);

char bit_m_get(char *c, int N, int i, int j);
int int_arr_get_sum(int n, int spaces[]);
void spaces_init(int n, int spaces[]);
bool spaces_is_valid(int spaces[]);
void spaces_next(int n, int spaces[], int tot);

poss_p poss_create(int N, char *b, poss_p p_next);
void poss_free_list(poss_p p);
void poss_filter(poss_p *p, int i, char val);

#endif
