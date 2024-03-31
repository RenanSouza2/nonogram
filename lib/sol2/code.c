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
    poss_p bars = malloc(N * sizeof(poss_t));
    assert(bars);
}

poss_p poss_arr_read(FILE *fp, int N)
{
    poss_p p = poss_arr_create(N);

    int bars[N/2 + 1];
    for(int i=0; i<N; i++)
    {
        int n = int_arr_read(bars, fp);
        int_vec_t val = int_vec_create(n, bars);
        bit_vec_t ftr = bit_vec_create(N);
        p[i] = (poss_t){val, ftr};
    }

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



bool filter_approve(int N, char b[], char ftr[])
{
    for(int i=0; i<N; i++)
        if(bit_is_valid(ftr[i]))
        if(b[i] != ftr[i])
            return false;

    return true;
}

void poss_verify(int N, char res[], poss_t bars)
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
    assert(spaces_is_valid(spaces));

    int rem = bars.ftr.n;
    for(spaces_next(n, spaces, tot); spaces_is_valid(spaces); spaces_next(n, spaces, tot))
    {
        char tmp[N];
        bit_arr_fill(N, tmp, n, spaces, bars.val.arr);
        if(!filter_approve(N, tmp, bars.ftr.arr))
            continue;

        for(int i=0; i<N; i++)
            if(bit_is_valid(res[i]))
            if(res[i] != tmp[i])
            {
                res[i] = -1;
                rem--;

                if(rem == 0) return;
            }
    }
}



void step(int i, int j, char val)
{
    gotoxy(1 + 2 * j, 1 + i);
    bit_display(val);
    struct timespec spec = (struct timespec){0, 1e6};
    nanosleep(&spec, NULL);
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

    step(i, j, val);
    return t->rem == 0;
}



bool table_scan_column(table_p t, int j);

bool table_scan_line(table_p t, int i)
{
    int N = t->N;
    char set[N];
    poss_verify(N, set, t->l[i]);

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
    char set[N];
    poss_verify(N, set, t->c[j]);
    
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
