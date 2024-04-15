#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdbool.h>

#include "../../utils/struct.h"

typedef char bit_t;
typedef bit_t * bit_p;

void fbit_display(FILE *fp, bit_t c);
void fbit_arr_display(FILE *fp, int n, bit_t c[]);

void bit_display(bit_t c);
void bit_arr_display(int n, bit_t c[]);
void bit_m_display(int n, bit_t c[]);

bit_p bit_arr_create(int n);

int int_arr_read(int bars[], FILE *fp);
bit_p bit_m_create(int n);
bit_t bit_m_get(bit_t c[], int n, int i, int j);
void bit_m_set(bit_t c[], int n, int i, int j, bit_t val);

bool bit_is_valid(bit_t b);
int int_arr_sum_reduce(int n, int arr[]);

FILE* file_open(char name[]);
char char_read(FILE *fp);
int int_read(FILE *fp);

#endif
