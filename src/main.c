#include <stdio.h>
#include <stdbool.h>

#include "../lib/utils/struct.h"
#include "../utils/assert.h"

int main()
{
    setbuf(stdout, NULL);

    table_t t;
    table_read(&t, "../tables/table6.txt");
    table_solve(&t);
    // table_display(&t);

    printf("\n");
    return 0;
}