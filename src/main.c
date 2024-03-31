#include <stdio.h>
#include <stdbool.h>

#include "../lib/utils/struct.h"
#include "../lib/sol2/struct.h"
#include "../utils/assert.h"

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);
    assert(argc == 2);
    
    char name[50];
    sprintf(name, "tables/table%s.txt", argv[1]);

    table_t t;
    table_read(&t, name);
    table_solve(&t);

    clrscr();
    table_display(&t);

    printf("\n");
    return 0;
}