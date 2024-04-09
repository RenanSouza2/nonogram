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

STRUCT(line_info)
{
    int_vec_t bars;
    int_vec_t places;
    bit_vec_t filter;
    int h;
};

STRUCT(table)
{
    int N;

    line_info_p r;
    line_info_p c;

    char* res;

    int rem;
};

#endif
