#ifndef __SOL2_STRUCT_H__
#define __SOL2_STRUCT_H__

#include "header.h"
#include "../utils/struct.h"

typedef int * int_p;

STRUCT(bit_vec)
{
    int n;
    bit_p arr;
};

STRUCT(line_info)
{
    int n, h;
    int_p bars;
    int_p places;
    bit_p filter;
};

STRUCT(table)
{
    int N, rem;

    line_info_p r;
    line_info_p c;

    bit_p res;
};

#endif
