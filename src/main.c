#include <stdio.h>

#include "../lib/utils/struct.h"
#include "../utils/assert.h"

int main()
{
    setbuf(stdout, NULL);

    table_t t;
    table_read(&t, "../tables/table3.txt");
    table_solve(&t);
    char_m_display(t.N, t.res);

    printf("\n");
    return 0;
}