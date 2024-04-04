#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "../utils/struct.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif

#define goto_pixel(I, J) gotoxy(2 + 2 * (J), 3 + (I));

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

    int tot = N + 1 - n - int_arr_sum_reduce(n, _bars.arr);
    return (line_info_t){_bars, _filter, n * tot};
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
    *t = (table_t){N, l, c, res, rem, NULL};
}



void line_set_bar(int N, char line[], int place, int size, bool clean)
{
    if(clean) memset(line, -1, N);
    memset(&line[place], 1, size);
    bit_arr_set(N, line, place - 1, 0);
    bit_arr_set(N, line, place + size, 0);
}

void line_fill(int N, char line[], int n, int places[], int bars[], bool fill)
{
    memset(line, fill ? 0 : -1, N);
    memset(line, 0, places[n-1]);

    for(int i=0; i<n; i++)
        line_set_bar(N, line, places[i], bars[i], false);
}

int line_verify(int N, char line[], char filter[])
{
    for(int i=N-1; i>=0; i--)
    {
        if(bit_is_valid(line[i]))
        if(bit_is_valid(filter[i]))
        if(line[i] != filter[i]) return i;
    }

    return -1;
}

bool line_approve(int N, char line[], char filter[])
{
    return line_verify(N, line, filter) < 0;
}

bool line_init_rec(int i, int starter, int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;

    if(i == n)
    {
        line_fill(N, line, n, places, l->bars.arr, true);
        return line_approve(N, line, l->filter.arr);
    }

    int max = N + 1 + i - n - int_arr_sum_reduce(n-i, &l->bars.arr[i]);

    for(int place=starter; place<=max; place++)
    {
        places[i] = place;
        line_fill(N, line, i+1, places, l->bars.arr, false);

        if(!line_approve(N, line, l->filter.arr))
            continue;

        if(line_init_rec(i+1, place + l->bars.arr[i] + 1, N, line, places, l))
            return true;
    }

    return false;
}

void line_init(int N, char line[], int places[], line_info_p l)
{
    
    assert(line_init_rec(0, 0, N, line, places, l));
    places[l->bars.n] = N+1;
}



bool line_next_fit(int i, int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;
    int _places[n+1];
    int_arr_set(n+1, _places, places);

    int bar = l->bars.arr[i];
    int max = places[i+1] - bar;

    for(int place=places[i]+1; place<max; place++)
    {
        _places[i] = place;
        line_set_bar(N, line, place, bar, true);
        if(line_approve(N, line, l->filter.arr))
            return int_arr_set(n+1, places, _places);
    }

    return false;
}

bool line_next_rec(int moved[], int i, int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;

    if(i == n)
        return false;

    while(line_next_fit(i, N, line, places, l))
    {
        moved[i] = 1;
        line_fill(N, line, n, places, l->bars.arr, true);
        if(line_verify(N, line, l->filter.arr) < places[i])
            return true;
    } 
    
    if(!line_next_rec(moved, i+1, N, line, places, l))
        return false;
    
    line_fill(N, line, n, places, l->bars.arr, true);

    if(line_verify(N, line, l->filter.arr) < places[i])
        return true;
    
    return line_next_rec(moved, i, N, line, places, l);
}

bool line_next(int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;

    int mov_c = n+1;
    int places_c[n+1];

    for(int i=0; i<n; i++)
    {
        int moved[n];
        int_arr_clean(n, moved);

        int _places[n+1];
        int_arr_set(n+1, _places, places);
        if(!line_next_rec(moved, i, N, line, _places, l))
            continue;
        
        line_fill(N, line, n, _places, l->bars.arr, true);
        if(!line_approve(N, line, l->filter.arr))
            continue;

        int mov = int_arr_sum_reduce(n, moved);
        if(mov == 1)
            return int_arr_set(n+1, places, _places);

        if(mov >= mov_c)
            continue;

        mov_c = mov;
        int_arr_set(n+1, places_c, _places); 
    }

    if(mov_c < n+1)
    {
        line_fill(N, line, n, places_c, l->bars.arr, true);
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

                if(rem == 0) 
                {

                    return false;
                }
            }
    }

    for(int i=0; i<N; i++)
        if(line[i] == l->filter.arr[i])
            line[i] = -1;

    return true;
}



void step(table_p t, int i, int j, char val)
{
    goto_pixel(i, j);
    bit_display(val);

    // table_display(t);

    // struct timespec spec = (struct timespec){0, 1e8};
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



void scan_display(scan_p s)
{
    for(; s; s = s->s)
        printf("\n%d %d %d", s->n, s->type, s->i);
}

scan_p scan_create(int n, int type, int i, scan_p s_next)
{
    scan_p s = malloc(sizeof(scan_t));
    assert(s);

    *s = (scan_t){n, type, i, s_next};
    return s;
}

void scan_add(scan_p *s, int n, int type, int i)
{
    scan_p _s = *s;
    if(_s == NULL || _s->n > n)
    {
        *s = scan_create(n, type, i, _s);
        return;
    }

    if(_s->type == type && _s->i == i)
        return;

    return scan_add(&_s->s, n, type, i);
}

void table_scan_add(table_p t, int type, int i)
{
    line_info_t l;
    switch (type)
    {
        case ROW   : l = t->r[i]; break;
        case COLUMN: l = t->c[i]; break;
        default: assert(false);
    }

    scan_add(&t->s, l.h, type, i);
}

scan_p table_scan_pop(table_p t)
{
    scan_p s = t->s;
    assert(s);
    t->s = s->s;
    return s;
}



bool table_scan_row(table_p t, int i)
{
    int N = t->N;

    goto_pixel(i, N+10);

    char set[N];
    if(!line_info_scan(N, set, &t->r[i]))
        return false;

    for(int j=0; j<N; j++)
    if(bit_is_valid(set[j]))
    {
        if(table_set(t, i, j, set[j]))
            return true;
        
        table_scan_add(t, COLUMN, j);
    }

    return false;
}

bool table_scan_column(table_p t, int j)
{
    int N = t->N;

    goto_pixel(N+10, j);

    char set[N];
    if(!line_info_scan(N, set, &t->c[j]))
        return false;
    
    for(int i=0; i<N; i++)
    if(bit_is_valid(set[i]))
    {
        if(table_set(t, i, j, set[i]))
            return true;

        table_scan_add(t, ROW, i);
    }

    return false;
}

bool table_scan(table_p t)
{
    while(t->rem)
    {
        scan_p s = table_scan_pop(t);
        switch (s->type)
        {
            case ROW: 
                if(table_scan_row(t, s->i))
                    return true;
            break;

            case COLUMN: 
                if(table_scan_column(t, s->i))
                    return true;
            break;

            default: assert(false);
        }
    }

    return false;
}

void table_solve(table_p t)
{
    clrscr();
    table_display(t);

    for(int i=0; i<t->N; i++)
    {
        table_scan_add(t, ROW, i);
        table_scan_add(t, COLUMN, i);
    }

    assert(table_scan(t));

    goto_pixel(t->N + 10, 0);
}
