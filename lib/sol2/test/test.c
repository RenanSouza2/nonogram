#include "../../utils/debug.h"

#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_sol2_line_scan()
{
    printf("\n\t%s", __func__);

    int n = 2;
    int N = 10;

    int places[n];
    char line[N];

    int bars[] = {1, 2};
    char filter[] = {-1, -1, -1, 1, 0, -1, -1, -1, -1, -1};

    line_info_t l = (line_info_t){{2, bars}, {8, filter}};
    places_init(10, line, places, &l);
    bit_arr_display(N, line);

    assert(places_next(N, line, places, &l));
    bit_arr_display(N, line);
    assert(places_next(N, line, places, &l));
    bit_arr_display(N, line);
    assert(places_next(N, line, places, &l));
    bit_arr_display(N, line);
    assert(places_next(N, line, places, &l));
    bit_arr_display(N, line);
    assert(places_next(N, line, places, &l) == false);

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
