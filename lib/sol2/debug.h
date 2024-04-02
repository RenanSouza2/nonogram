#ifndef __SOL2_DEBUG_H__
#define __SOL2_DEBUG_H__

#include <stdbool.h>

#include "struct.h"

#ifdef DEBUG
#endif

void places_init(int N, char line[], int places[], line_info_p l);
bool places_next(int N, char line[], int places[], line_info_p l);

#endif
