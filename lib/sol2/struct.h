#ifndef __SOL2_STRUCT_H__
#define __SOL2_STRUCT_H__

#include "header.h"

STRUCT(int_vec)
{
    int n;
    int *arr;
};

STRUCT(table2)
{
    int N;

    int_vec_p l;
    int_vec_p c;

    char* cmp;
    char* res;

    int rem;
};

#endif
