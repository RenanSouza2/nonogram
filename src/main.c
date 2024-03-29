#include <stdio.h>
#include <stdbool.h>

#include "../lib/utils/struct.h"
#include "../lib/sol2/struct.h"
#include "../utils/assert.h"

int main(int argc, char **argv)
{
    setbuf(stdout, NULL);

    printf("\nargc: %d", argc);
    printf("\nargv: %s", argv[0]);
    printf("\nargv: %s", argv[1]);
    
    assert(argc == 2);
    char name[50];
    sprintf(name, "tables/table%s.txt", argv[1]);

    table2_t t;
    table2_read(&t, name);
    table2_solve(&t);

    printf("\n");
    return 0;
}