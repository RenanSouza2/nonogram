#include <stdbool.h>
#include <string.h>

#include "debug.h"

#ifdef DEBUG

#include "../../utils/clu/bin/header.h"

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

void spaces_next(int n, int spaces[], int tot) 
{
    if(n == 0) spaces[1] = -1;

    if(spaces_get_tot(n, spaces) < tot)
    {
        spaces[n-1]++;
        return;
    }

    spaces[n-1] = 0;
    spaces_next(n-1, spaces, tot);
    return;
}

bool spaces_valid(int spaces[])
{
    return spaces[0] >= 0;
}

void spaces_generate(int n, int tot)
{
    if(n == 0) return;

    int spaces[n];
    for(spaces_init(n, spaces); spaces_valid(spaces); spaces_next(n, spaces, tot))
    {
        printf("\n %d", spaces[0]);
        for(int i=1; i<n; i++)
            printf(" %d", spaces[i]);
    }
}