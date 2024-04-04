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
    bit_vec_t filter;
    int h;
};

#define ROW 0
#define COLUMN 1

STRUCT(scan)
{
    int n;
    int type;
    int i;
    scan_p s;
};

STRUCT(table)
{
    int N;

    line_info_p r;
    line_info_p c;

    char* res;

    int rem;

    scan_p s;
};

#endif
