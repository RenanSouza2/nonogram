#include <stdlib.h>
#include <string.h>
#include <time.h>



#include "debug.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"



bit_p bit_arr_init_immed(char str[])
{
    int n = strlen(str);
    bit_p b = malloc(n);
    assert(b);

    for(int i=0; i<n; i++)
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



bool bit_test(bit_t b1, bit_t b2)
{
    if(b1 != b2)
    {
        printf("\n\n\tBIT ASSERTION ERROR | (");
        bit_display(b1);
        printf(") (");
        bit_display(b2);
        printf(")");
        return false;
    }

    return true;
}

bool bit_arr_test(bit_t b1[], bit_t str[])
{
    bit_p b2 = bit_arr_init_immed(str);
    for(int i=0; i<strlen(str); i++)
    {
        if(!bit_test(b1[i], b2[i]))
        {
            printf("\n\tBIT ARR ASSERTION ERROR | BIT ERROR %d", i);
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



void fbit_display(FILE *fp, bit_t b)
{
    switch (b)
    {
        case -1: fprintf(fp, "  "); return;
        case  0: fprintf(fp, "░░"); return;
        case  1: fprintf(fp, "██"); return;
    }
    
    fprintf(fp, "\nINVALID BIT: %d", b);
    assert(false);
}

void fbit_arr_display(FILE *fp, int n, bit_t b[])
{
    fprintf(fp, "\n│");
    for(int i=0; i<n; i++)
        fbit_display(fp, b[i]);
    fprintf(fp, "│");
}

void bit_display(bit_t b)
{
    fbit_display(stdout, b);
}

void bit_arr_display(int n, bit_t b[])
{
    fbit_arr_display(stdout, n, b);
}

void bit_m_display(int n, bit_t b[])
{
    printf("\n┌");
    for(int i=0; i<n; i++)
        printf("──");
    printf("┐");

    for(int i=0; i<n; i++)
        bit_arr_display(n, &b[n * i]);

    printf("\n└");
    for(int i=0; i<n; i++)
        printf("──");
    printf("┘");
}



bool bit_is_valid(bit_t b)
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



bit_p bit_arr_create(int n)
{
    bit_p b = malloc(n);
    assert(b);

    memset(b, -1, n);
    return b;
}



bit_t bit_m_get(bit_t c[], int n, int i, int j)
{
    return c[i * n + j];
}

void bit_m_set(bit_t c[], int n, int i, int j, bit_t val)
{
    c[i * n + j] = val;
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
