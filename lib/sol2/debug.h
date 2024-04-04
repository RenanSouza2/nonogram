#ifndef __SOL2_DEBUG_H__
#define __SOL2_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG
#endif

void line_init(int N, char line[], int places[], line_info_p l);
bool line_next(int N, char line[], int places[], line_info_p l);
void line_fill(int N, char line[], int n, int places[], int bars[]);

#endif
