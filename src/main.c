#include <stdio.h>

#include "../lib/utils/struct.h"
#include "../utils/assert.h"

int main()
{
    setbuf(stdout, NULL);

    table_t t;
    table_read(&t, "../tables/table4.txt");
    table_solve(&t);

    printf("\n");
    return 0;
}