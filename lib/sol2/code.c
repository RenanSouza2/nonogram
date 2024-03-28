#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "debug.h"
#include "../utils/struct.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



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

    char* cmp = bit_m_create(N);
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

    *t = (table2_t){N, l, c, cmp, res, rem};
}

void bars_verify(int N, int val[], int_vec_t bars)
{
    int n = bars.n;
    int spaces[n];

    
    int tot = N + 1 - n - int_arr_get_sum(n, bars);


    for(spaces_init(n, spaces); spaces_is_valid(spaces); spaces_next(n, spaces, tot))
    {

    }
}

bool table2_scan_line(table2_p t, int i)
{
    int val[t->N];
    table2 = 2

    for(int j=0; j<t->N; j++)
    {
        if(bit_m_get(t->cmp, t->N, i, j)) continue;

        char val = poss_verify(t->l[i], j);
        if(val < 0) continue;

        if(table_set_column(t, i, j, val)) return true;
    }

    return false;
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
            if(table_scan_column(t, j)) 
                return;
        }
    }
}
