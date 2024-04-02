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

void int_arr_set(int n, int arr1[], int arr2[])
{
    memcpy(arr1, arr2, n * sizeof(int));
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

    return (line_info_t){_bars, _filter};
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
    FILE *fp = file_open(name);

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

void line_fill(int N, char line[], int n, int places[], int bars[], char fill)
{
    memset(line, fill, N);
    memset(line, 0, places[n-1]);

    for(int i=0; i<n; i++)
        line_set_bar(N, line, places[i], bars[i], false);
}

int line_verify(int N, char line[], char filter[])
{
    for(int i=0; i<N; i++)
    {
        if(bit_is_valid(line[i]))
        if(bit_is_valid(filter[i]))
        if(line[i] != filter[i]) return i;
    }

    return N;
}

bool line_approve(int N, char line[], char filter[])
{
    return line_verify(N, line, filter) == N;
}



bool places_init_rec(int i, int starter, int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;
// printf("\nplaces init rec %d %d", i, n);

    if(i == n)
    {
        line_fill(N, line, n, places, l->bars.arr, 0);
        return line_approve(N, line, l->filter.arr);
    }

    int max = N + 1 + i - n - int_arr_sum_reduce(n-i, &l->bars.arr[i]);
// printf("\nmax %d: %d", i, max);

    for(int place=starter; place<=max; place++)
    {
// printf("\nnew place %d: %d", i, place);

        places[i] = place;
        line_fill(N, line, i+1, places, l->bars.arr, -1);

// bit_arr_display(N, l->filter.arr);
// bit_arr_display(N, line);

        if(!line_approve(N, line, l->filter.arr))
            continue;

        if(places_init_rec(i+1, place + l->bars.arr[i] + 1, N, line, places, l))
            return true;
    }

    return false;
}

void places_init(int N, char line[], int places[], line_info_p l)
{
    assert(places_init_rec(0, 0, N, line, places, l));
    places[l->bars.n] = N+1;
}

bool places_next_fit(int i, int N, char line[], int places[], line_info_p l)
{
    if(i < 0) return false;

    int bar = l->bars.arr[i];
    int max = places[i+1] - 1 - bar;
    for(int place=places[i] + 1; place<=max; place++)
    {
        places[i] = place;
        line_set_bar(N, line, place, bar, true);
        if(line_approve(N, line, l->filter.arr))
            return true;
    }
    return false;
}

bool places_next_rec(int i, int N, char line[], int places[], line_info_p l)
{
    if(i < 0) return false;

    int n = l->bars.n;
    int _places[n+1];
    int_arr_set(n+1, _places, places);

    if(!places_next_fit(i, N, line, _places, l)) 
        return false;

    line_fill(N, line, n, _places, l->bars.arr, 0);
    if(
        line_approve(N, line, l->filter.arr) ||
        places_next_rec(i-1, N, line, _places, l)
    ) {
        int_arr_set(n, places, _places);
        return true;
    }

    return false;
}

bool places_next(int N, char line[], int places[], line_info_p l)
{
    int n = l->bars.n;
    for(int i=n-1; i>=0; i--)
        if(places_next_rec(i, N, line, places, l))
            return true;

    return false;
}



bool line_info_scan(int N, char line[], line_info_p l)
{
    int n = l->bars.n;
    int rem = l->filter.n;

printf("\nline scan");
printf("\nbars: ");
for(int i=0; i<l->bars.n; i++)
    printf(" %d", l->bars.arr[i]);
printf("\nfilter");
bit_arr_display(N, l->filter.arr);
    
    int places[n+1];
    places_init(N, line, places, l);

printf("\nfirst");
bit_arr_display(N, line);

    char tmp[N];
    while(places_next(N, tmp, places, l))
    {
printf("\n------------");
bit_arr_display(N, line);
bit_arr_display(N, tmp);

        for(int i=0; i<N; i++)
            if(bit_is_valid(line[i]))
            if(line[i] != tmp[i])
            {
                line[i] = -1;
                rem--;

                if(rem == 0) 
                {

// printf("\t NO CONCLUSION :(");

                    return false;
                }
            }

bit_arr_display(N, line);
printf("\trem: %d", rem);
getchar();
    }

bit_arr_display(N, line);
printf("\tCONCLUSION!!!!");
getchar();

    return true;
}




void step(table_p t, int i, int j, char val)
{
    // gotoxy(1 + 2 * j, 2 + i);
    // bit_display(val);

    // clrscr();
    table_display(t);

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
    filter_set(&t->r[i].filter, j, val);
    filter_set(&t->c[j].filter, i, val);

    step(t, i, j, val);
    return t->rem == 0;
}



bool table_scan_column(table_p t, int j);

bool table_scan_row(table_p t, int i)
{
    int N = t->N;
    // gotoxy(1 + 2 * N + 10, 2 + i);

    char set[N];
    line_info_scan(N, set, &t->r[i]);

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
    // gotoxy(1 + 2 * j, 2 + N + 10);

    char set[N];
    line_info_scan(N, set, &t->c[j]);
    
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
        if(table_scan_row(t, i))
            return true;

    return false;
}

void table_solve(table_p t)
{
    // clrscr();
    while(t->rem)
    {
        for(int i=0; i<t->N; i++)
            if(table_scan_row(t, i)) 
                return;

        for(int j=0; j<t->N; j++)
            if(table_scan_column(t, j)) 
                return;
    }
}
