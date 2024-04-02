#include "../../utils/debug.h"

#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_sol2_line_scan()
{
    printf("\n\t%s", __func__);

    int N = 10;
    int n = 2;


    int bars[2] = {2, 3};
    int places[3] = {1, 6, 11};
    char filter[10] = {-1, 1, -1, -1, -1, -1, -1, 1, -1, -1};
    line_info_t l = (line_info_t){{2, bars}, {8, filter}};

    char line[10];
    line_fill(N, line, n, places, bars, true);
    bit_arr_display(N, filter);
    bit_arr_display(N, line);


    assert(line_next(N, line, places, &l));
    bit_arr_display(N, line);

    assert(clu_mem_empty());
}



void test_sol2()
{
    printf("\n%s", __func__);

    test_sol2_line_scan();

    assert(clu_mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_sol2();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
