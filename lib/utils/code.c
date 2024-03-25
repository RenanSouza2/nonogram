#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "../../utils/assert.h"



#define clrscr() printf("\e[1;1H\e[2J")

#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"



void int_arr_set(int arr[], int n, ...)
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

bool char_arr_test(char arr[], int n, ...)
{
    va_list args;
    va_start(args, n);
    for(int i=0; i<n; i++)
    {
        char c2 = va_arg(args, int);
        if(!char_test(arr[i], c2))
        {
            printf("\n\tCHAR ARR ASSERTION ERROR | CHAR POS");
            return false;
        }
    }

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

bool int_arr_test(int arr[], int n, ...)
{
    va_list args;
    va_start(args, n);
    for(int i=0; i<n; i++)
    {
        int i2 = va_arg(args, int);
        if(!int_test(arr[i], i2))
        {
            printf("\n\tINT ARR ASSERTION ERROR | INT POS");
            return false;
        }
    }

    return true;
}

#endif



void slot_display(char c)
{
    if(c) printf("▓▓");
    else  printf("░░");
}

void bar_display(int N, char c[])
{
    printf("\n");
    for(int i=0; i<N; i++)
        slot_display(c[i]);
}

void char_m_display(int N, char c[])
{
    for(int i=0; i<N; i++)
        bar_display(N, &c[N * i]);
}

void table_display(table_p t)
{
    for(int i=0; i<t->N; i++)
    {
        printf("\n");
        for(int j=0; j<t->N; j++)
        {
            if(char_m_get(t->cmp, t->N, i, j)) slot_display(char_m_get(t->res, t->N, i, j));
            else printf("  ");
        }
    }
}


int int_arr_get_tot(int n, int arr[])
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

    if(int_arr_get_tot(n, spaces) < tot)
    {
        spaces[n-1]++;
        return;
    }

    spaces[n-1] = 0;
    spaces_next(n-1, spaces, tot);
    return;
}



char* bar_create(int N, int n, int spaces[], int bars[])
{
    char *b = calloc(N, 1);
    assert(b);
    int j = 0;
    for(int i=0; i<n; i++)
    {
        j += spaces[i];
        memset(&b[j], 1, bars[i]);
        j += bars[i] + 1;
    }
    return b;
}



poss_p poss_create(int N, int n, int spaces[], int bars[], poss_p p_next)
{
    poss_p p = malloc(sizeof(poss_t));
    assert(p);

    char *b = bar_create(N, n, spaces, bars);
    *p = (poss_t){N, b, p_next};
    return p;
}

poss_p poss_generate(int N, int n, int bars[])
{
    poss_p p = NULL;
    int spaces[n];
    int tot = N + 1 - n - int_arr_get_tot(n, bars);
    for(spaces_init(n, spaces); spaces_is_valid(spaces); spaces_next(n, spaces, tot))
        p = poss_create(N, n, spaces, bars, p);

    return p;
}

poss_p poss_filter(poss_p p, int i, char val)
{
    if(p == NULL) return NULL;

    poss_p p_next = poss_filter(p->p, i, val);

    if(p->b[i] == val)
    {
        p->p = p_next;
        return p;
    }

    free(p);
    return p_next;
}

char poss_verify_rec(poss_p p, int i, int val)
{
    if(p == NULL) return val;

    if(p->b[i] != val) return -1;

    return poss_verify_rec(p->p, i, val);
}

char poss_verify(poss_p p, int i)
{
    return poss_verify_rec(p->p, i, p->b[i]);
}



char char_m_get(char *c, int N, int i, int j)
{
    return c[i * N + j];
}

void char_m_set(char *c, int N, int i, int j, char val)
{
    c[i * N + j] = val;
}



bool table_scan_line(table_p t, int i);
bool table_scan_column(table_p t, int j);

bool table_set_line(table_p t, int i, int j, char val)
{
    t->rem--;
    char_m_set(t->cmp, t->N, i, j, 1);
    char_m_set(t->res, t->N, i, j, val);

    clrscr();
    table_display(t);
    struct timespec spec = (struct timespec){0, 1e8};
    nanosleep(&spec, NULL);

    if(t->rem == 0) return true;

    t->l[i] = poss_filter(t->l[i], j, val);
    return table_scan_column(t, j);
}

bool table_set_column(table_p t, int i, int j, char val)
{
    t->rem--;
    char_m_set(t->cmp, t->N, i, j, 1);
    char_m_set(t->res, t->N, i, j, val);

    clrscr();
    table_display(t);
    struct timespec spec = (struct timespec){0, 1e8};
    nanosleep(&spec, NULL);
    
    if(t->rem == 0) return true;

    t->c[j] = poss_filter(t->c[j], i, val);
    return table_scan_line(t, j);
}



bool table_scan_line(table_p t, int i)
{
    for(int j=0; j<t->N; j++)
    {
        if(char_m_get(t->cmp, t->N, i, j)) continue;

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
        if(char_m_get(t->cmp, t->N, i, j)) continue;

        char val = poss_verify(t->c[j], i);
        if(val < 0) continue;

        if(table_set_line(t, i, j, val)) return true;
    }

    return false;
}



void table_solve(table_p t)
{
    while(t->rem)
    {
        for(int i=0; i<t->N; i++)
            if(table_scan_line(t, i)) 
                return;

        for(int j=0; j<t->N; j++)
            if(table_scan_column(t, j)) 
                return;
    }
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

int bars_read(int bars[], FILE *fp)
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

char* char_m_create(int N)
{
    char *c = malloc(N*N);
    assert(c);
    return c;
}

void table_read(table_p t, char name[])
{
    FILE *fp = file_open(name);

    int N = int_read(fp);
    char_read(fp);

    poss_p *l = poss_arr_create(N);
    poss_p *c = poss_arr_create(N);

    char* cmp = char_m_create(N);
    char* res = char_m_create(N);

    int rem = N * N;

    int bars[N/2 + 1];
    for(int i=0; i<N; i++)
    {
        int n = bars_read(bars, fp);
        l[i] = poss_generate(N, n, bars);
    }

    printf("\n");

    for(int j=0; j<N; j++)
    {
        int n = bars_read(bars, fp);
        c[j] = poss_generate(N, n, bars);
    }

    *t = (table_t){N, l, c, cmp, res, rem};
}