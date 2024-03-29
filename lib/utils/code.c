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



poss_p poss_init_immed(int N, int n, ...)
{
    va_list args;
    va_start(args, n);
    
    poss_p p = NULL;
    for(int i=0; i<n; i++)
    {
        char *str = va_arg(args, char*);
        assert(strlen(str) == N);

        char *b = bit_arr_init(str);
        p = poss_create(N, b, p);
    }

    return p;
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

bool poss_test(poss_p p1, int N, int n, ...)
{
    va_list args;
    va_start(args, n);
    
    int i=0;
    for(; p1; p1 = p1->p, i++)
    {
        char *str = va_arg(args, char*);
        if(!bit_arr_test(p1->b, str))
        {
            printf("\n\tPOSS ASSERTION ERROR | BIT ARR ERROR | %d", i);
            return false;
        }
    }

    if(i < n)
    {
        printf("\n\n\tPOSS ASSERTION ERROR | LIST SHORTER | %d %d", i, n);
        return false;
    }

    if(p1)
    {
        printf("\n\n\tPOSS ASSERTION ERROR | LIST LONGER | %d", i);
        return false;
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

void table_display(table_p t)
{
    bit_m_display(t->N, t->res);
}



bool bit_is_valid(char b)
{
    return b >= 0;
}

int int_arr_get_sum(int n, int arr[])
{
    int tot = 0;
    for(int i=0; i<n; i++)
        tot += arr[i];

    return tot;
}



void spaces_init(int n, int spaces[])
{
    memset(spaces, 0, n * sizeof(int));
}

bool spaces_is_valid(int spaces[])
{
    return spaces[0] >= 0;
}

void spaces_next(int n, int spaces[], int tot)
{
    if(n == 0) 
    {
        spaces[0] = -1;
        return;
    }

    if(int_arr_get_sum(n, spaces) < tot)
    {
        spaces[n-1]++;
        return;
    }

    spaces[n-1] = 0;
    spaces_next(n-1, spaces, tot);
    return;
}



void bit_arr_fill(int N, char b[], int n, int spaces[], int bars[])
{
    memset(b, 0, N);
    
    int j = 0;
    for(int i=0; i<n; i++)
    {
        j += spaces[i];
        memset(&b[j], 1, bars[i]);
        j += bars[i] + 1;
    }
}

char* bit_arr_create(int N)
{
    char *b = calloc(N, 1);
    assert(b);
    return b;
}



poss_p poss_create(int N, char *b, poss_p p_next)
{
    poss_p p = malloc(sizeof(poss_t));
    assert(p);

    *p = (poss_t){b, p_next};
    return p;
}

poss_p poss_free(poss_p p)
{
    poss_p p_next = p->p;
    free(p->b);
    free(p);
    return p_next;
}

void poss_free_list(poss_p p)
{
    while(p) p = poss_free(p);
}

poss_p poss_generate(int N, int n, int bars[])
{
    poss_p p = NULL;
    int spaces[n];
    int tot = N + 1 - n - int_arr_get_sum(n, bars);
    for(spaces_init(n, spaces); spaces_is_valid(spaces); spaces_next(n, spaces, tot))
    {
        char *b = bit_arr_create(N);
        bit_arr_fill(N, b, n, spaces, bars);
        p = poss_create(N, b, p);
    }

    return p;
}

void poss_filter(poss_p *p, int i, char val)
{
    while(*p)
    {
        if((*p)->b[i] == val)
        {
            p = &(*p)->p;
        }
        else
        {
            poss_p p_aux = (*p)->p;
            poss_free(*p);
            *p = p_aux;
        }
    }
}

char poss_verify(poss_p p, int i)
{
    char val = p->b[i];
    for(p = p->p; p; p = p->p)
        if(p->b[i] != val)
            return -1;

    return val;
}




char bit_m_get(char *c, int N, int i, int j)
{
    return c[i * N + j];
}

void bit_m_set(char *c, int N, int i, int j, char val)
{
    c[i * N + j] = val;
}



void step(table_p t)
{
    clrscr();
    table_display(t);
    struct timespec spec = (struct timespec){0, 5e6};
    nanosleep(&spec, NULL);
}

bool table_set_line(table_p t, int i, int j, char val)
{
    t->rem--;
    bit_m_set(t->res, t->N, i, j, val);

    step(t);
    // printf("\nset line %d %d", i, j);
    if(t->rem == 0) return true;

    poss_filter(&t->l[i], j, val);
    return table_scan_column(t, j);
}

bool table_set_column(table_p t, int i, int j, char val)
{
    t->rem--;
    bit_m_set(t->res, t->N, i, j, val);

    step(t);
    // printf("\ncolumn line %d %d", i, j);
    if(t->rem == 0) return true;

    poss_filter(&t->c[j], i, val);
    return table_scan_line(t, i);
}



bool table_scan_line(table_p t, int i)
{
    for(int j=0; j<t->N; j++)
    {
        if(bit_is_valid(bit_m_get(t->res, t->N, i, j))) continue;

        char val = poss_verify(t->l[i], j);
        if(val < 0) continue;

        if(table_set_column(t, i, j, val)) return true;
    }

    return false;
}

bool table_scan_column(table_p t, int j)
{
    for(int i=0; i<t->N; i++)
    {
        if(bit_is_valid(bit_m_get(t->res, t->N, i, j))) continue;

        char val = poss_verify(t->c[j], i);
        if(val < 0) continue;

        if(table_set_line(t, i, j, val)) return true;
    }

    return false;
}



void table_solve(table_p t)
{
    int k;
    for(k=0; t->rem; k++)
    {
        for(int i=0; i<t->N; i++)
            if(table_scan_line(t, i)) 
                break;

        for(int j=0; j<t->N; j++)
            if(table_scan_column(t, j)) 
                break;
    }
    printf("\nk: %d", k);
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

poss_p* poss_arr_create(int N)
{
    poss_p *p = malloc(N * sizeof(poss_p));
    assert(p);
    return p;
}

char* bit_m_create(int N)
{
    size_t size = N * N;
    char *c = malloc(size);
    assert(c);

    memset(c, -1, size);
    return c;
}

void table_read(table_p t, char name[])
{
    FILE *fp = file_open(name);

    int N = int_read(fp);
    char_read(fp);

    poss_p *l = poss_arr_create(N);
    poss_p *c = poss_arr_create(N);

    char* res = bit_m_create(N);

    int rem = N * N;

    int bars[N/2 + 1];
    for(int i=0; i<N; i++)
    {
        int n = int_arr_read(bars, fp);
        l[i] = poss_generate(N, n, bars);
    }

    for(int j=0; j<N; j++)
    {
        int n = int_arr_read(bars, fp);
        c[j] = poss_generate(N, n, bars);
    }

    *t = (table_t){N, l, c, res, rem};
}
