#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdbool.h>

#include "../../utils/struct.h"

#define clrscr() printf("\e[1;1H\e[2J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

PLACEHOLDER(table);

void fbit_display(FILE *fp, char c);
void fbit_arr_display(FILE *fp, int N, char c[]);

void bit_display(char c);
void bit_arr_display(int N, char c[]);
void bit_m_display(int N, char c[]);

int int_arr_read(int bars[], FILE *fp);
char* bit_m_create(int N);
char bit_m_get(char *c, int N, int i, int j);
void bit_m_set(char *c, int N, int i, int j, char val);

bool bit_is_valid(char b);
int int_arr_sum_reduce(int n, int arr[]);

FILE* file_open(char name[]);
char char_read(FILE *fp);
int int_read(FILE *fp);

#endif
