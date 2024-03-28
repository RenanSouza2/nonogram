#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>

#include "../../utils/struct.h"

PLACEHOLDER(table);

int int_arr_read(int bars[], FILE *fp);
char* bit_m_create(int N);

int int_arr_get_sum(int n, int arr[]);

FILE* file_open(char name[]);
char char_read(FILE *fp);
int int_read(FILE *fp);

void spaces_init(int n, int spaces[]);
bool spaces_is_valid(int spaces[]);
void spaces_next(int n, int spaces[], int tot);

void table_read(table_p t, char name[]);
void table_solve(table_p t);
void table_display(table_p t);

#endif
