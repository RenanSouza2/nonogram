#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdbool.h>

#include "../../utils/struct.h"

#define clrscr() printf("\e[1;1H\e[2J")

PLACEHOLDER(table);

void bit_display(char c);
void bit_arr_display(int N, char c[]);
void bit_m_display(int N, char c[]);

int int_arr_read(int bars[], FILE *fp);
char* bit_m_create(int N);
char bit_m_get(char *c, int N, int i, int j);
void bit_m_set(char *c, int N, int i, int j, char val);

bool bit_is_valid(char b);
int int_arr_get_sum(int n, int arr[]);

void bit_arr_fill(int N, char b[], int n, int spaces[], int bars[]);

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
