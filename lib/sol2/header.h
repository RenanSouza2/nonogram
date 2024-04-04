#ifndef __SOL2_H__
#define __SOL2_H__

#include "../../utils/struct.h"

PLACEHOLDER(table);

void solution_read(char name[]);

void table_read(table_p t, char name[]);
void table_solve(table_p t);
void table_display(table_p t);

#endif
