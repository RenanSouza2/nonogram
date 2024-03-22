#include <string.h>

#include "debug.h"

#ifdef DEBUG

#include <stdarg.h>

#include "../../utils/clu/bin/header.h"

bool int_test(int i1, int i2)
{
    if(i1 != i2)
    {
        printf("\n\n\tINT ASSERTION ERROR | %d %d", i1, i2);
        return false;
    }

    return true;
}

bool int_arr_test(int arr[], int n, ...)
{
    va_list args;
    va_start(args, n);
    for(int i=0; i<n; i++)
    {
        int i2 = va_arg(args, int);
        if(!int_test(arr[i], i2))
        {
            printf("\n\tINT ARR ASSERTION ERROR | INT POS");
            return false;
        }
    }

    return true;
}

#endif

int spaces_get_tot(int n, int spaces[])
{
    int tot = 0;
    for(int i=0; i<n; i++)
        tot += spaces[i];

    return tot;
}

void spaces_init(int n, int spaces[])
{
    memset(spaces, 0, n * sizeof(int));
}

bool spaces_is_valid(int spaces[])
{
    return spaces[0] >= 0;
}

void spaces_next(int n, int spaces[], int tot)
{
    printf("\ngenerating next");
    getchar();

    if(n == 0) 
    {
        printf("\nInvalidating");
        spaces[1] = -1;
        return;
    }

    if(spaces_get_tot(n, spaces) < tot)
    {
        spaces[n-1]++;
        return;
    }

    spaces[n-1] = 0;
    spaces_next(n-1, spaces, tot);
    return;
}

void spaces_generate(int n, int tot)
{
    if(n == 0) return;

    int spaces[n];
    for(spaces_init(n, spaces); spaces_is_valid(spaces); spaces_next(n, spaces, tot))
    {
        printf("\n res %d", spaces[0]);
        for(int i=1; i<n; i++)
            printf(" %d", spaces[i]);
    }
}
