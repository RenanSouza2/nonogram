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



int_vec_p int_arr_arr_create(int N)
{
    int_vec_p vec = malloc(N * sizeof(int_vec_t));
    assert(vec);
    return vec;
}

int_vec_t int_vec_create(int n, int arr[])
{
    size_t size = n * sizeof(int);
    int_vec_t vec = (int_vec_t){n, malloc(size)};
    assert(vec.arr);

    memcpy(vec.arr, arr, size);
    return vec;
}



void sol2_table_read(table2_p t, char name[])
{
    FILE *fp = file_open(name);

    int N = int_read(fp);
    char_read(fp);

    int_vec_p l = int_arr_arr_create(N);
    int_vec_p c = int_arr_arr_create(N);

    char* res = bit_m_create(N);

    int rem = N * N;

    int bars[N/2 + 1];
    for(int i=0; i<N; i++)
    {
        int n = int_arr_read(bars, fp);
        l[i] = int_vec_create(n, bars);
    }

    for(int j=0; j<N; j++)
    {
        int n = int_arr_read(bars, fp);
        c[j] = int_vec_create(n, bars);
    }

    *t = (table2_t){N, l, c, res, rem};
}

void bars_verify(int N, char val[], int_vec_t bars)
{
    int n = bars.n;
    int spaces[n];
    spaces_init(n, spaces);
    bit_arr_fill(N, val, n, spaces, bars.arr);
    
    int tot = N + 1 - n - int_arr_get_sum(n, bars.arr);
    int rem = N;
    for(spaces_next(n, spaces, tot); spaces_is_valid(spaces) && rem; spaces_next(n, spaces, tot))
    {
        char tmp[N];
        bit_arr_fill(N, tmp, n, spaces, bars.arr);
        for(int i=0; i<N; i++)
            if(bit_is_valid(val[i]))
            if(val[i] != tmp[i])
            {
                val[i] = -1;
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

bool table2_set(table2_p t, int i, int j, char val)
{
    t->rem--;
    bit_m_set(t->res, t->N, i, j, val);

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
        if(table2_set(t, i, j, val[i]))
            return true;

    for(int j=0; j<N; j++)
        if(table2_scan_column(t, j))
            return true;

    return false;
}

bool table2_scan_column(table2_p t, int j)
{
    return false;

    // int N = t->N;
    // char val[N];
    // bars_verify(N, val, t->l[j]);

    // for(int i=0; i<N; i++)
    //     if(table2_set(t, i, j, val[i]))
    //         return true;

    // for(int i=0; i<N; i++)
    //     if(table2_scan_column(t, i))
    //         return;

    // return false;
}

void table2_solve(table2_p t)
{
    while(t->rem)
    {
        for(int i=0; i<t->N; i++)
        {
            if(table2_scan_line(t, i)) 
                return;
        }

        for(int j=0; j<t->N; j++)
        {
            if(table2_scan_column(t, j)) 
                return;
        }
    }
}
