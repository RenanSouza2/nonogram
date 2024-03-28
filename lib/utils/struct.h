#ifndef __UTILS_STRUCT_H__
#define __UTILS_STRUCT_H__

#include "header.h"

STRUCT(poss)
{
    char *b;
    poss_p p;
};

STRUCT(table)
{
    int N;

    poss_p *l;
    poss_p *c;

    char* res;
    int rem;
};

#endif
