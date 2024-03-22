#ifndef __EXAMPLE_DEBUG_H__
#define __EXAMPLE_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG

#define assert_int(I1, I2) assert(int_test(I1, I2))
#define assert_int_arr(I1, ...) assert(int_arr_test(I1, __VA_ARGS__))

void int_arr_set(int spaces[], int n, ...);

bool int_test(int i1, int i2);
bool int_arr_test(int arr[], int n, ...);

#endif

int spaces_get_tot(int n, int spaces[]);
void spaces_init(int n, int spaces[]);
bool spaces_is_valid(int spaces[]);
void spaces_next(int n, int spaces[], int tot);
void spaces_generate(int n, int tot);

#endif
