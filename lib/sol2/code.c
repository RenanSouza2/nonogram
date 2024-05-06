#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "debug.h"
#include "../utils/struct.h"
#include "../../utils/assert.h"



#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

#endif



#define clrscr() printf("\e[1;1H\e[2J")
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
#define goto_pixel(I, J) gotoxy(2 + 2 * (J), 3 + (I));



#define ALTERNATE 0
#define COMPARE
// #define DELAY 5e7



#ifdef COMPARE

bit_p global;

void solution_read(int N, char name[])
{
    char _name[50];
    snprintf(_name, 50, "res/res%s.txt", name);
    FILE *fp = file_open(_name);

    global = bit_arr_create(N * N);
    for(int i=0; i<N; i++)
    for(int j=0; j<N; j++)
    {
        bit_t val = int_read(fp);
        bit_m_set(global, N, i, j, val);
    }

    printf("\nRes read");
    bit_m_display(N, global);
}

#endif



void table_display(table_p t)
{
    bit_m_display(t->N, t->res);
}



void int_arr_clean(int n, int arr[])
{
    memset(arr, 0, n * sizeof(int));
}

void int_arr_copy(int n, int arr1[], int arr2[])
{
    memcpy(arr1, arr2, n * sizeof(int));
}

int_p int_arr_create(int n, int arr[])
{
    size_t size = n * sizeof(int);
    int_p _arr = malloc(size);
    assert(_arr);

    memcpy(_arr, arr, size);
    return _arr;
}



line_info_p line_info_arr_create(int N)
{
    line_info_p l = malloc(N * sizeof(line_info_t));
    assert(l);
    return l;
}

int_p places_init(int N, int n, int bars[])
{
    int places[n+1];
    places[n] = N+1;

    int j = 0;
    for(int i=0; i<n; i++)
    {
        places[i] = j;
        j += bars[i] + 1;
    }

    return int_arr_create(n+1, places);
}

line_info_t line_info_read(FILE *fp, int N)
{
    int _bars[N/2 + 1];
    int n = int_arr_read(_bars, fp);

    int_p bars = int_arr_create(n, _bars);
    int_p places = places_init(N, n, _bars);
    bit_p filter = bit_arr_create(N);

    return (line_info_t){n, 1, bars, places, filter};
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
    int area = N * N;

    #ifdef COMPARE
    solution_read(N, name);
    #endif

    char_read(fp);
    line_info_p l = line_info_arr_read(fp, N);
    line_info_p c = line_info_arr_read(fp, N);
    bit_p res = bit_arr_create(area);

    *t = (table_t){N, l, c, res};
}



void line_fill(int N, bit_t line[], int n, int places[], int bars[])
{
    memset(line, 0, N);
    for(int i=0; i<n; i++)
        memset(&line[places[i]], 1, bars[i]);
}

int line_verify(int N, bit_t line[], bit_t filter[])
{
    for(int i=N-1; i>=0; i--)
    {
        if(bit_is_valid(filter[i]))
        if(line[i] != filter[i]) 
            return i;
    }

    return -1;
}

bool line_approve(int N, bit_t line[], bit_t filter[])
{
    return line_verify(N, line, filter) < 0;
}



short line_move_bar(
    int i,
    int N,
    bit_t line[],
    int places[],
    int starter,
    line_info_p l
) {
    if(i < 0) return -1;
    
    int bar = l->bars[i];
    int max = places[i+1] - bar;

    for(int place = places[i] + starter; place < max; place++)
    {
        memset(&line[places[i]], 0, bar);
        places[i] = place;
        memset(&line[place], 1, bar);
        
        int diff = line_verify(N, line, l->filter);
        if(diff >= place)
        {
            place = diff - bar * l->filter[diff];
            continue;
        }

        if(diff < 0)
            return i;
        
        short first = line_move_bar(i-1, N, line, places, starter, l);
        if(first >= 0)
            return first;
    }

    return -1;
}

void line_init(int N, bit_t line[], int places[], line_info_p l)
{
    int n = l->n;

    int_arr_copy(n+1, places, l->places);
    line_fill(N, line, n, places, l->bars);
    if(line_approve(N, line, l->filter))
        return;
    
    assert(line_move_bar(n-1, N, line, places, 0, l) >= 0);
    int_arr_copy(n+1, l->places, places);
}

range_t line_next(int N, bit_t line[], int places[], line_info_p l)
{
    int n = l->n;
    
    int i_c, first_c; 
    int mov_c = n+1;
    int places_c[n+1];

    for(int i=n-1; i>=0; i--)
    {
        int _places[n+1];
        int_arr_copy(n+1, _places, places);

        line_fill(N, line, n, _places, l->bars);
        int first = line_move_bar(i, N, line, _places, 1, l);
        if(first < 0)
            continue;
        
        int mov = 0;
        for(int j=0; j<n; j++)
            mov += (places[j] != _places[j]);

        if(mov == 1)
        {
            int min = places[first];
            int_arr_copy(n+1, places, _places);
            return (range_t){min, places[i] + l->bars[i]};
        }

        if(mov < mov_c) 
        {
            i_c = i;
            first_c = first;
            mov_c = mov;
            int_arr_copy(n+1, places_c, _places);
        }
    }

    if(mov_c < n+1)
    {
        int min = places[first_c];
        line_fill(N, line, n, places_c, l->bars);
        int_arr_copy(n+1, places, places_c);
        return (range_t){min, places[i_c] + l->bars[i_c]};
    }
    
    return (range_t){0, -1};
}

bool line_info_scan(int N, bit_t line[], line_info_p l)
{
    #if ALTERNATE > 0
    printf("\nline info scan");
    printf("\nbars: ");
    for(int i=0; i<l->n; i++)
        printf(" %d", l->bars[i]);
    bit_arr_display(N, l->filter);
    #endif

    int n = l->n;
    int places[n+1];
    line_init(N, line, places, l);

    int rem = N;
    for(int i=0; i<N; i++)
    if(bit_is_valid(l->filter[i]))
    {
        rem--;
        line[i] = -1;
    }
    if(rem == 0) return false;

    bit_t tmp[N];

    for(
        range_t range = line_next(N, tmp, places, l); 
        range.max >= 0 ; 
        range = line_next(N, tmp, places, l)
    ) {
        #if ALTERNATE > 1
        bit_arr_display(N, tmp);
        #endif

        for(int i=0; i<range.max; i++)
        if(bit_is_valid(line[i]))
        if(line[i] != tmp[i])
        {
            rem--;
            line[i] = -1;
            
            if(rem == 0)
            {
                #if ALTERNATE > 1
                printf("\trem: 0");
                #endif

                return false;
            }
        }
        
        #if ALTERNATE > 1
        printf("\trem: %d", rem);
        #endif
    }

    #if ALTERNATE > 0
    printf("\n");
    for(int i=0; i<N+1; i++)
        printf("──");
    bit_arr_display(N, line);
    #endif

    return true;
}



void step(table_p t, int i, int j, bit_t val)
{
    #if ALTERNATE == 0
    goto_pixel(i, j);
    bit_display(val);
    #endif

    #ifdef COMPARE
    bit_t _val = bit_m_get(global, t->N, i, j);
    assert(_val == val);
    #endif

    #ifdef DELAY
    struct timespec spec = (struct timespec){0, DELAY};
    nanosleep(&spec, NULL);
    #endif
}

int table_set(table_p t, int i, int j, bit_t val)
{
    bit_m_set(t->res, t->N, i, j, val);
    t->r[i].filter[j] = val;
    t->c[j].filter[i] = val;

    step(t, i, j, val);
    return 1;
}



void table_row_set_flag(table_p t, int i, char val)
{
    t->r[i].h = val;

    #if ALTERNATE == 0
    goto_pixel(i, t->N+5);
    bit_display((val<<1) - 1);
    #endif
}

void table_column_set_flag(table_p t, int j, char val)
{
    t->c[j].h = val;

    #if ALTERNATE == 0
    goto_pixel(t->N+5, j);
    bit_display((val<<1) - 1);
    #endif
}

int table_scan_row(table_p t, int i)
{
    int N = t->N;
    char set[N];
    if(!line_info_scan(N, set, &t->r[i]))
        return 0;

    int solved = 0;
    for(int j=0; j<N; j++)
    if(bit_is_valid(set[j]))
    {
        solved++;
        table_set(t, i, j, set[j]);
        table_column_set_flag(t, j, 1);
    }

    #if ALTERNATE > 0
    table_display(t);
    #endif

    return solved;
}

int table_scan_column(table_p t, int j)
{
    int N = t->N;
    char set[N];
    if(!line_info_scan(N, set, &t->c[j]))
        return 0;
    
    int solved = 0;
    for(int i=0; i<N; i++)
    if(bit_is_valid(set[i]))
    {
        solved++;
        table_set(t, i, j, set[i]);
        table_row_set_flag(t, i, 1);
    }

    #if ALTERNATE > 0
    table_display(t);
    #endif

    return solved;
}

int table_scan(table_p t)
{
    int N = t->N;
    for(int rem = N * N; rem > 0;)
    {
        int last_rem = rem;

        int sum = 0;
        for(int i=0; i<N; i++)
        if(t->r[i].h)
        {
            table_row_set_flag(t, i, 0);
            sum += table_scan_row(t, i);
        }

        rem -= sum;
        if(rem == 0) break;

        sum = 0;
        for(int j=0; j<N; j++)
        if(t->c[j].h)
        {
            table_column_set_flag(t, j, 0);
            sum += table_scan_column(t, j);
        }

        rem -= sum;
        if(rem == last_rem)
            return false;
    }

    return true;
}

void table_solve(table_p t)
{
    #if ALTERNATE == 0
    clrscr();
    table_display(t);
    for(int i=0; i<t->N; i++)
    {
        table_row_set_flag(t, i, 1);
        table_column_set_flag(t, i, 1);
    }
    #else
    table_display(t);
    #endif

    assert(table_scan(t));

    #if ALTERNATE == 0
    for(int i=0; i<t->N; i++)
    {
        table_row_set_flag(t, i, 0);
        table_column_set_flag(t, i, 0);
    }
    goto_pixel(t->N + 6, 0);
    #else
    table_display(t);
    #endif
}
