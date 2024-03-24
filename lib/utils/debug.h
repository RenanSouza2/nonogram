#ifndef __UTILS_DEBUG_H__
#define __UTILS_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG

#define assert_char(C1, C2) assert(char_test(C1, C2))
#define assert_char_arr(C1, ...) assert(char_arr_test(C1, __VA_ARGS__))
#define assert_int(I1, I2) assert(int_test(I1, I2))
#define assert_int_arr(I1, ...) assert(int_arr_test(I1, __VA_ARGS__))

void int_arr_set(int arr[], int n, ...);

bool char_test(char c1, char c2);
bool char_arr_test(char arr[], int n, ...);

bool int_test(int i1, int i2);
bool int_arr_test(int arr[], int n, ...);

#endif

char char_m_get(char *c, int N, int i, int j);
int int_arr_get_tot(int n, int spaces[]);
void spaces_init(int n, int spaces[]);
bool spaces_is_valid(int spaces[]);
void spaces_next(int n, int spaces[], int tot);

char* bar_create(int N, int n, int spaces[], int bars[]);

poss_p poss_create(int N, int n, int spaces[], int bars[], poss_p p_next);

#endif
