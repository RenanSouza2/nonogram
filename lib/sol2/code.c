#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "../utils/struct.h"
#include "../../utils/assert.h"



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
// printf("\napproving");
// bit_arr_display(N, b);
// bit_arr_display(N, ftr);

    for(int i=0; i<N; i++)
        if(bit_is_valid(ftr[i]))
        if(b[i] != ftr[i])
            return false;

// printf("\tis true");

    return true;
}

void bars_verify(int N, char res[], bars_t bars)
{
    int n = bars.val.n;
    int tot = N + 1 - n - int_arr_get_sum(n, bars.val.arr);

// printf("\nbars: ");
// for(int i=0; i<n; i++)
//     printf("%d ", bars.val.arr[i]);

// printf("\nftr");
// printf("\n--------------");
// bit_arr_display(N, bars.ftr.arr);
// printf("\n--------------");

    int spaces[n];
    for(spaces_init(n, spaces); spaces_is_valid(spaces); spaces_next(n, spaces, tot))
    {
        bit_arr_fill(N, res, n, spaces, bars.val.arr);
        if(filter_approve(N, res, bars.ftr.arr))
            break;
    }
    assert(spaces_is_valid(spaces));

// printf("\nfirst");
// bit_arr_display(N, res);
// printf("\nnexts");

    int rem = bars.ftr.n;
    for(spaces_next(n, spaces, tot); spaces_is_valid(spaces) && rem; spaces_next(n, spaces, tot))
    {
        char tmp[N];
        bit_arr_fill(N, tmp, n, spaces, bars.val.arr);
        if(!filter_approve(N, tmp, bars.ftr.arr))
            continue;

// printf("\nvalid");
// bit_arr_display(N, tmp);

        for(int i=0; i<N; i++)
            if(bit_is_valid(res[i]))
            if(res[i] != tmp[i])
            {
                res[i] = -1;
                rem--;

                if(rem == 0) break;
            }

// bit_arr_display(N, res);
    }
}



void step2(table2_p t)
{
// getchar();
    clrscr();
    table2_display(t);
    struct timespec spec = (struct timespec){0, 1e8};
    nanosleep(&spec, NULL);
}

void filter_set(bit_vec_p b, int i, char val)
{
    b->arr[i] = val;
    b->n--;
}

bool table2_set(table2_p t, int i, int j, char val)
{
    printf("\nset %d %d %d", i, j, val);

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
// printf("\nscan line %d", i);

    int N = t->N;
    char set[N];
    bars_verify(N, set, t->l[i]);

// printf("\nres verify");
// bit_arr_display(N, set);

    char scan[N];
    memset(scan, 0, N);

    for(int j=0; j<N; j++)
    {
        if(bit_is_valid(bit_m_get(t->res, N, i, j))) continue;

        char val = set[j];
        if(!bit_is_valid(val)) continue;

        if(table2_set(t, i, j, val))
            return true;

        scan[j] = true;
    }

    for(int j=0; j<N; j++)
        if(scan[j])
        if(table2_scan_column(t, j))
            return true;

    return false;
}

bool table2_scan_column(table2_p t, int j)
{
// printf("\nscan column %d", j);

    int N = t->N;
    char set[N];
    bars_verify(N, set, t->c[j]);
    
// printf("\nres verify");
// bit_arr_display(N, set);

    char scan[N];
    memset(scan, 0, N);

    for(int i=0; i<N; i++)
    {
        if(bit_is_valid(bit_m_get(t->res, N, i, j))) continue;

        char val = set[i];
        if(!bit_is_valid(val)) continue;

        if(table2_set(t, i, j, val))
            return true;

        scan[i] = true;
    }

    for(int i=0; i<N; i++)
        if(scan[i])
        if(table2_scan_line(t, i))
            return true;

    return false;
}

void table2_solve(table2_p t)
{
    while(t->rem)
    {
// printf("\nnew loop");

        for(int i=0; i<t->N; i++)
            if(table2_scan_line(t, i)) 
                return;

        for(int j=0; j<t->N; j++)
            if(table2_scan_column(t, j)) 
                return;
    }
}
