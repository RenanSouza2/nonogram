#ifndef __SOL2_STRUCT_H__
#define __SOL2_STRUCT_H__

#include "header.h"

STRUCT(bit_vec)
{
    int n;
    char *arr;
};

STRUCT(int_vec)
{
    int n;
    int *arr;
};

STRUCT(poss)
{
    int_vec_t val;
    bit_vec_t ftr;
};

STRUCT(table)
{
    int N;

    poss_p l;
    poss_p c;

    char* res;

    int rem;
};

#endif
