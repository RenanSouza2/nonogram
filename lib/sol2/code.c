#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "../utils/struct.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif

#define goto_pixel(I, J)    gotoxy(2 + 2 * (J), 3 + (I));

bool compare = false;
char *global;

void solution_read(char name[])
{
    char _name[50];
    snprintf(_name, 50, "res/res%s.txt", name);
    FILE *fp = file_open(_name);

    int N = int_read(fp);
    global = bit_m_create(N);
    for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
    {
        char val = int_read(fp);
        bit_m_set(global, N, i, j, val);
    }

    compare = true;
}



// #define ALTERNATE

void table_display(table_p t)
{
#ifndef ALTERNATE

    for(int i=0; i<t->N; i++)
    {
        goto_pixel(i, t->N + 1);
        printf("%3d", i);
    }
    goto_pixel(-2,0)

#endif

    bit_m_display(t->N, t->res);
}



bit_vec_t bit_vec_create(int n)
{
    char *c = malloc(n);
    assert(c);
    memset(c, -1, n);

    return (bit_vec_t){n, c};
}

void int_arr_clean(int n, int arr[])
{
    memset(arr, 0, n * sizeof(int));
}

bool int_arr_set(int n, int arr1[], int arr2[])
{
    memcpy(arr1, arr2, n * sizeof(int));
    return true;
}

void bit_arr_set(int n, char b[], int i, char val)
{
    if(i <  0) return;
    if(n <= i) return;

    b[i] = val;
}

int_vec_t int_vec_create(int n, int arr[])
{
    size_t size = n * sizeof(int);
    int* _arr = malloc(size);
    assert(_arr);

    memcpy(_arr, arr, size);
    return (int_vec_t){n, _arr};
}



line_info_p line_info_arr_create(int N)
{
    line_info_p l = malloc(N * sizeof(line_info_t));
    assert(l);
    return l;
}

line_info_t line_info_read(FILE *fp, int N)
{
    int bars[N/2 + 1];
    int n = int_arr_read(bars, fp);

    int_vec_t _bars = int_vec_create(n, bars);
    bit_vec_t _filter = bit_vec_create(N);

    return (line_info_t){_bars, _filter, 1};
}

line_info_p line_info_arr_read(FILE *fp, int N)
{
    line_info_p l = line_info_arr_create(N);
    for(int i=0; i<N; i++)
        l[i] = line_info_read(fp, N);

    return l;
}

void table_read(table_p t, char name[])
{
    char _name[50];
    snprintf(_name, 50, "tables/table%s.txt", name);
    FILE *fp = file_open(_name);

    int N = int_read(fp);
    char_read(fp);

    line_info_p l = line_info_arr_read(fp, N);
    line_info_p c = line_info_arr_read(fp, N);
    char* res = bit_m_create(N);

    int rem = N * N;
    *t = (table_t){N, l, c, res, rem};
}



void line_set_bar(int N, char line[], int place, int size, bool clean)
{
    if(clean) memset(line, -1, N);
    memset(&line[place], 1, size);
    bit_arr_set(N, line, place - 1, 0);
    bit_arr_set(N, line, place + size, 0);
}

void line_fill(int N, char line[], int n, int places[], int bars[])
{
    memset(line, 0, N);

    for(int i=0; i<n; i++)
        line_set_bar(N, line, places[i], bars[i], false);
}

int line_verify(int N, char line[], char filter[])
{
    for(int i=N-1; i>=0; i--)
    {
        if(bit_is_valid(line[i]))
        if(bit_is_valid(filter[i]))
        if(line[i] != filter[i]) 
            return i;
    }

    return -1;
}

bool line_approve(int N, char line[], char filter[])
{
    return line_verify(N, line, filter) < 0;
}



bool line_next_bar_rec(int moved[], int i, int N, char line[], int places[], line_info_p l)
{
    if(i < 0) return false;
    
    int n = l->bars.n;
    int bar = l->bars.arr[i];
    int max = places[i+1] - bar;

    for(int place = places[i] + 1; place < max; place++)
    {
        places[i] = place;
        line_fill(N, line, n, places, l->bars.arr);
        
        int diff = line_verify(N, line, l->filter.arr);
        if(diff >= place)
            continue;

        moved[i] = 1;

        if(diff < 0) 
            return true;

        if(line_next_bar_rec(moved, i-1, N, line, places, l))
            return true;
    }

    return false;
}

int line_next_bar(int i, int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;
    int moved[n];
    int_arr_clean(n, moved);

    if(!line_next_bar_rec(moved, i, N, line, places, l))
        return n+1;

    line_fill(N, line, n, places, l->bars.arr);
    if(!line_approve(N, line, l->filter.arr))
        return n+1;

    return int_arr_sum_reduce(n, moved);
}

void line_init(int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;

    int j = 0;
    for(int i=0; i<n; i++)
    {
        places[i] = j;
        j += l->bars.arr[i] + 1;
    }
    places[n] = N+1;
    
    line_fill(N, line, n, places, l->bars.arr);
    if(line_approve(N, line, l->filter.arr))
        return;
    
    assert(line_next_bar(n-1, N, line, places, l));
}

bool line_next(int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;

    int mov_c = n+1;
    int places_c[n+1];

    for(int i=n-1; i>=0; i--)
    {
        int _places[n+1];
        int_arr_set(n+1, _places, places);

        int mov = line_next_bar(i, N, line, _places, l);
        if(mov >= mov_c) 
            continue;
    
        if(mov == 1)
            return int_arr_set(n+1, places, _places);

        mov_c = mov;
        int_arr_set(n+1, places_c, _places); 
    }

    if(mov_c < n+1)
    {
        line_fill(N, line, n, places_c, l->bars.arr);
        return int_arr_set(n+1, places, places_c);
    }
    
    return false;
}

bool line_info_scan(int N, char line[], line_info_p l)
{
    int n = l->bars.n;
    int rem = l->filter.n;

    int places[n+1];
    line_init(N, line, places, l);

    char tmp[N];
    while(line_next(N, tmp, places, l))
    {
        for(int i=0; i<N; i++)
        if(bit_is_valid(line[i]))
        if(line[i] != tmp[i])
        {
            line[i] = -1;
            rem--;

            if(rem == 0) return false;
        }
    }

    for(int i=0; i<N; i++)
        if(line[i] == l->filter.arr[i])
            line[i] = -1;

    return true;
}



void step(table_p t, int i, int j, char val)
{
#ifdef ALTERNATE

    table_display(t);

#else

    goto_pixel(i, j);
    bit_display(val);

#endif

    // struct timespec spec = (struct timespec){0, 5e7};
    // nanosleep(&spec, NULL);
}

void filter_set(bit_vec_p b, int i, char val)
{
    b->arr[i] = val;
    b->n--;
}

bool table_set(table_p t, int i, int j, char val)
{
    if(compare)
    {
        char _val = bit_m_get(global, t->N, i, j);
        assert(_val == val);
    }

    t->rem--;
    bit_m_set(t->res, t->N, i, j, val);
    filter_set(&t->r[i].filter, j, val);
    filter_set(&t->c[j].filter, i, val);

    step(t, i, j, val);
    return t->rem == 0;
}



bool table_scan_row(table_p t, int i)
{
    int N = t->N;

#ifndef ALTERNATE

    goto_pixel(i, N+5);
    bit_display(-1);
    goto_pixel(i, N+5);

#endif

    char set[N];
    if(!line_info_scan(N, set, &t->r[i]))
        return false;

    for(int j=0; j<N; j++)
    if(bit_is_valid(set[j]))
    {
        if(table_set(t, i, j, set[j]))
            return true;
        
        t->c[j].h = 1;

#ifndef ALTERNATE

        goto_pixel(N+5, j);
        bit_display(1);

#endif
    }

    return false;
}

bool table_scan_column(table_p t, int j)
{
    int N = t->N;

#ifndef ALTERNATE

    goto_pixel(N+5, j);
    bit_display(-1);
    goto_pixel(N+5, j);

#endif

    char set[N];
    if(!line_info_scan(N, set, &t->c[j]))
        return false;
    
    for(int i=0; i<N; i++)
    if(bit_is_valid(set[i]))
    {
        if(table_set(t, i, j, set[i]))
            return true;

        t->r[i].h = 1;
        
#ifndef ALTERNATE

        goto_pixel(i, N+5);
        bit_display(1);
    
#endif
    }

    return false;
}

bool table_scan(table_p t)
{
    int N = t->N;
    while(t->rem)
    {
        for(int i=0; i<N; i++)
        if(t->r[i].h)
        {
            t->r[i].h = 0;
            if(table_scan_row(t, i))
                return true;
        }

        for(int j=0; j<N; j++)
        if(t->c[j].h)
        {
            t->c[j].h = 0;
            if(table_scan_column(t, j))
                return true;
        }
    }

    return false;
}

void table_solve(table_p t)
{

#ifndef ALTERNATE

    clrscr();
    table_display(t);

#endif

    assert(table_scan(t));

#ifndef ALTERNATE
    
    goto_pixel(t->N + 6, 0);
    
#endif
}
