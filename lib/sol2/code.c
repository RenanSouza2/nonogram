#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "../utils/struct.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



void table_display(table_p t)
{
    bit_m_display(t->N, t->res);
}



bit_vec_t bit_vec_create(int n)
{
    char *c = malloc(n);
    assert(c);
    memset(c, -1, n);

    return (bit_vec_t){n, c};
}

int_vec_t int_vec_create(int n, int arr[])
{
    size_t size = n * sizeof(int);
    int* _arr = malloc(size);
    assert(_arr);

    memcpy(_arr, arr, size);
    return (int_vec_t){n, _arr};
}

poss_p poss_arr_create(int N)
{
    poss_p p = malloc(N * sizeof(poss_t));
    assert(p);
    return p;
}

poss_t poss_read(FILE *fp, int N)
{
    int bars[N/2 + 1];
    int n = int_arr_read(bars, fp);

    int_vec_t _bars = int_vec_create(n, bars);
    bit_vec_t ftr = bit_vec_create(N);

    int tot = N + 1 - n - int_arr_sum_reduce(n, bars);

    return (poss_t){tot, _bars, ftr};
}

poss_p poss_arr_read(FILE *fp, int N)
{
    poss_p p = poss_arr_create(N);
    for(int i=0; i<N; i++)
        p[i] = poss_read(fp, N);

    return p;
}

void table_read(table_p t, char name[])
{
    FILE *fp = file_open(name);

    int N = int_read(fp);
    char_read(fp);

    poss_p l = poss_arr_read(fp, N);
    poss_p c = poss_arr_read(fp, N);
    char* res = bit_m_create(N);

    int rem = N * N;
    *t = (table_t){N, l, c, res, rem};
}



void line_fill(int N, char line[], int n, int spaces[], int bars[], char fill)
{
    memset(line, fill, N);
    
    int j = 0;
    for(int i=0; i<n; i++)
    {
        memset(&line[j], 0, spaces[i]);
        j += spaces[i];

        memset(&line[j], 1, bars[i]);
        j += bars[i];

        line[j] = 0;
        j +=  1;
    }
}

bool line_approve(int N, char line[], char ftr[])
{
    for(int i=0; i<N; i++)
    {
        if(!bit_is_valid(line[i]))
            break;

        if(!bit_is_valid(ftr[i]))
            continue;

        if(line[i] != ftr[i])
            return false;
    }

    return true;
}



bool spaces_init_rec(int N, char line[], int spaces[], poss_p p, int i, int starter)
{
    int n = p->bars.n;
    if(i == n)
    {
        line_fill(N, line, n, spaces, p->bars.arr, 0);
        return line_approve(N, line, p->ftr.arr);
    }

    int max = p->tot - int_arr_sum_reduce(i, spaces);
    for(int space=starter; space <= max; space++)
    {
        spaces[i] = space;
        line_fill(N, line, i+1, spaces, p->bars.arr, -1);
        if(!line_approve(N, line, p->ftr.arr))
            continue;

        if(spaces_init_rec(N, line, spaces, p, i+1, 0))
            return true;
    }

    return false;
}

void spaces_init(int N, char line[], int spaces[], poss_p p)
{
    assert(spaces_init_rec(N, line, spaces, p, 0, 0));
}

bool spaces_next(int N, char line[], int spaces[], poss_p p)
{
    int n = p->bars.n;
    if(N == p->ftr.n)
    {
        if(spaces[0] == p->tot) return false;

        spaces[0]++;
        line_fill(N, line, n, spaces, p->bars.arr, 0);
        return true;
    }

    for(int i=n-1; i >= 0; i--)
        if(spaces_init_rec(N, line, spaces, p, i, spaces[i] + 1))
            return true;
    
    return false;
}

bool poss_verify(int N, char line[], poss_p p)
{
    int n = p->bars.n;
    int rem = p->ftr.n;

    int spaces[n];
    spaces_init(N, line, spaces, p);

    char tmp[N];
    while(spaces_next(N, tmp, spaces, p))
    {
        for(int i=0; i<N; i++)
            if(bit_is_valid(line[i]))
            if(line[i] != tmp[i])
            {
                line[i] = -1;
                rem--;

                if(rem == 0) 
                    return false;
            }
    }

    return true;
}



void step(table_p t, int i, int j, char val)
{
    gotoxy(1 + 2 * j, 2 + i);
    bit_display(val);

    // clrscr();
    // table_display(t);

    // struct timespec spec = (struct timespec){0, 1e6};
    // nanosleep(&spec, NULL);
}

void filter_set(bit_vec_p b, int i, char val)
{
    b->arr[i] = val;
    b->n--;
}

bool table_set(table_p t, int i, int j, char val)
{
    t->rem--;
    bit_m_set(t->res, t->N, i, j, val);
    filter_set(&t->l[i].ftr, j, val);
    filter_set(&t->c[j].ftr, i, val);

    step(t, i, j, val);
    return t->rem == 0;
}



bool table_scan_column(table_p t, int j);

bool table_scan_line(table_p t, int i)
{
    int N = t->N;
    gotoxy(1 + 2 * N + 10, 2 + i);

    char set[N];
    poss_verify(N, set, &t->l[i]);

    char scan[N];
    memset(scan, 0, N);

    for(int j=0; j<N; j++)
    {
        if(bit_is_valid(bit_m_get(t->res, N, i, j))) continue;

        char val = set[j];
        if(!bit_is_valid(val)) continue;

        if(table_set(t, i, j, val))
            return true;

        scan[j] = true;
    }

    for(int j=0; j<N; j++)
        if(scan[j])
        if(table_scan_column(t, j))
            return true;

    return false;
}

bool table_scan_column(table_p t, int j)
{
    int N = t->N;
    gotoxy(1 + 2 * j, 2 + N + 10);

    char set[N];
    poss_verify(N, set, &t->c[j]);
    
    char scan[N];
    memset(scan, 0, N);

    for(int i=0; i<N; i++)
    {
        if(bit_is_valid(bit_m_get(t->res, N, i, j))) continue;

        char val = set[i];
        if(!bit_is_valid(val)) continue;

        if(table_set(t, i, j, val))
            return true;

        scan[i] = true;
    }

    for(int i=0; i<N; i++)
        if(scan[i])
        if(table_scan_line(t, i))
            return true;

    return false;
}

void table_solve(table_p t)
{
    clrscr();
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
