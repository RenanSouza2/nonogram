#include <stdio.h>
#include <stdbool.h>

#include "../lib/utils/struct.h"
#include "../lib/sol2/struct.h"
#include "../utils/assert.h"

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    assert(argc == 2);

    solution_read(argv[1]);
    
    table_t t;
    table_read(&t, argv[1]);
    table_solve(&t);

    // clrscr();
    table_display(&t);

    printf("\n");
    return 0;
}