#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "debug.h"
#include "../utils/struct.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



void table2_display(table2_p t)
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

bars_p bars_arr_create(int N)
{
    bars_p bars = malloc(N * sizeof(bars_t));
    assert(bars);
    return bars;
}



void table2_read(table2_p t, char name[])
{
    FILE *fp = file_open(name);

    int N = int_read(fp);
    char_read(fp);

    bars_p l = bars_arr_create(N);
    bars_p c = bars_arr_create(N);
    char* res = bit_m_create(N);

    int bars[N/2 + 1];
    for(int i=0; i<N; i++)
    {
        int n = int_arr_read(bars, fp);
        int_vec_t val = int_vec_create(n, bars);
        bit_vec_t ftr = bit_vec_create(N);
        l[i] = (bars_t){val, ftr};
    }

    for(int j=0; j<N; j++)
    {
        int n = int_arr_read(bars, fp);
        int_vec_t val = int_vec_create(n, bars);
        bit_vec_t ftr = bit_vec_create(N);
        c[j] = (bars_t){val, ftr};
    }

    int rem = N * N;
    *t = (table2_t){N, l, c, res, rem};
}

bool filter_approve(int N, char b[], char ftr[])
{
    for(int i=0; i<N; i++)
        if(bit_is_valid(ftr[i]))
        if(b[i] != ftr[i])
            return false;

    return true;
}

void bars_verify(int N, char res[], bars_t bars)
{
    int n = bars.val.n;
    int tot = N + 1 - n - int_arr_get_sum(n, bars.val.arr);

    int spaces[n];
    for(spaces_init(n, spaces); spaces_is_valid(spaces); spaces_next(n, spaces, tot))
    {
        bit_arr_fill(N, res, n, spaces, bars.val.arr);
        if(filter_approve(N, res, bars.ftr.arr))
            break;
    }
    assert(!spaces_is_valid(spaces));
    
    int rem = bars.ftr.n;
    for(spaces_next(n, spaces, tot); spaces_is_valid(spaces) && rem; spaces_next(n, spaces, tot))
    {
        char tmp[N];
        bit_arr_fill(N, tmp, n, spaces, bars.val.arr);
        if(!filter_approve(N, res, bars.ftr.arr))
            continue;

        for(int i=0; i<N; i++)
            if(bit_is_valid(res[i]))
            if(res[i] != tmp[i])
            {
                res[i] = -1;
                rem--;
            }
    }
}



void step2(table2_p t)
{
    clrscr();
    table2_display(t);
    struct timespec spec = (struct timespec){0, 5e6};
    nanosleep(&spec, NULL);
}

void filter_set(bit_vec_p b, int i, char val)
{
    b->arr[i] = val;
    b->n--;
}

bool table2_set(table2_p t, int i, int j, char val)
{
    t->rem--;
    bit_m_set(t->res, t->N, i, j, val);
    filter_set(&t->l[i].ftr, j, val);
    filter_set(&t->c[j].ftr, i, val);

    step2(t);
    return t->rem == 0;
}



bool table2_scan_column(table2_p t, int j);

bool table2_scan_line(table2_p t, int i)
{
    int N = t->N;
    char val[N];
    bars_verify(N, val, t->l[i]);

    for(int j=0; j<N; j++)
        if(bit_is_valid(val[j]))
        if(table2_set(t, i, j, val[j]))
            return true;

    for(int j=0; j<N; j++)
        if(bit_is_valid(val[j]))
        if(table2_scan_column(t, j))
            return true;

    return false;
}

bool table2_scan_column(table2_p t, int j)
{
    int N = t->N;
    char val[N];
    bars_verify(N, val, t->c[j]);

    for(int i=0; i<N; i++)
        if(bit_is_valid(val[i]))
        if(table2_set(t, i, j, val[i]))
            return true;

    for(int i=0; i<N; i++)
        if(bit_is_valid(val[i]))
        if(table2_scan_line(t, i))
            return true;

    return false;
}

void table2_solve(table2_p t)
{
    while(t->rem)
    {
        for(int i=0; i<t->N; i++)
            if(table2_scan_line(t, i)) 
                return;

        for(int j=0; j<t->N; j++)
            if(table2_scan_column(t, j)) 
                return;
    }
}
