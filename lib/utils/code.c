#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"



char* bit_arr_init(char str[])
{
    int N = strlen(str);
    char *b = malloc(N);
    assert(b);

    for(int i=0; i<N; i++)
        b[i] = str[i] == '0' ? 0 : 1;

    return b;
}

void int_arr_init_immed(int arr[], int n, ...)
{
    va_list args;
    va_start(args, n);
    for(int i=0; i<n; i++)
        arr[i] = va_arg(args, int);
}



bool char_test(char c1, char c2)
{
    if(c1 != c2)
    {
        printf("\n\n\tCHAR ASSERTION ERROR | %d %d", c1, c2);
        return false;
    }

    return true;
}

bool bit_arr_test(char b1[], char str[])
{
    char *b2 = bit_arr_init(str);
    for(int i=0; i<strlen(str); i++)
    {
        if(!char_test(b1[i], b2[i]))
        {
            printf("\n\tCHAR ARR ASSERTION ERROR | CHAR POS");
            return false;
        }
    }
    free(b2);

    return true;
}



bool int_test(int i1, int i2)
{
    if(i1 != i2)
    {
        printf("\n\n\tINT ASSERTION ERROR | %d %d", i1, i2);
        return false;
    }

    return true;
}

bool int_arr_test(int spaces[], int n, ...)
{
    va_list args;
    va_start(args, n);
    for(int i=0; i<n; i++)
    {
        int i2 = va_arg(args, int);
        if(!int_test(spaces[i], i2))
        {
            printf("\n\tINT ARR ASSERTION ERROR | INT POS");
            return false;
        }
    }

    return true;
}

#endif



void bit_display(char c)
{
    switch (c)
    {
        case -1: printf("  "); return;
        case  0: printf("░░"); return;
        case  1: printf("▓▓"); return;
    }
}

void bit_arr_display(int N, char c[])
{
    printf("\n");
    for(int i=0; i<N; i++)
        bit_display(c[i]);
}

void bit_m_display(int N, char c[])
{
    for(int i=0; i<N; i++)
        bit_arr_display(N, &c[N * i]);
}



bool bit_is_valid(char b)
{
    return b >= 0;
}

int int_arr_sum_reduce(int n, int arr[])
{
    int tot = 0;
    for(int i=0; i<n; i++)
        tot += arr[i];

    return tot;
}



char* bit_arr_create(int N)
{
    char *b = calloc(N, 1);
    assert(b);
    return b;
}



char bit_m_get(char *c, int N, int i, int j)
{
    return c[i * N + j];
}

void bit_m_set(char *c, int N, int i, int j, char val)
{
    c[i * N + j] = val;
}



FILE* file_open(char name[])
{
    FILE *fp = fopen(name, "r");
    assert(fp);
    return fp;
}

char char_read(FILE *fp)
{
    char c;
    fscanf(fp, "%c", &c);
    return c;
}

int int_read(FILE *fp)
{
    int i;
    fscanf(fp, "%d", &i);
    return i;
}

int int_arr_read(int bars[], FILE *fp)
{
    for(int n=0; ;n++)
    {
        bars[n] = int_read(fp);

        if(char_read(fp) == '\n')
            return n + 1;
    }
}

char* bit_m_create(int N)
{
    size_t size = N * N;
    char *c = malloc(size);
    assert(c);

    memset(c, -1, size);
    return c;
}
