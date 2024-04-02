#include "../../utils/debug.h"

#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_sol2_line_scan_1()
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

void test_sol2_line_scan_2()
{
    printf("\n\t%s", __func__);

    int N = 10;
    int n = 2;


    int bars[2] = {1, 2};
    int places[3] = {0, 2, 11};
    char filter[10] = {-1, -1, -1, 1, 0, -1, -1, -1, -1, -1};
    line_info_t l = (line_info_t){{2, bars}, {8, filter}};

    char line[10];
    line_fill(N, line, n, places, bars, true);
    bit_arr_display(N, filter);
    bit_arr_display(N, line);


    assert(line_next(N, line, places, &l));
    bit_arr_display(N, line);

    assert(clu_mem_empty());
}

void test_sol2_line_scan_3()
{
    printf("\n\t%s", __func__);

    int N = 30;
    int n = 7;


    int bars[7] = {5, 1, 1, 1, 3, 1, 2};
    int places[8] = {6, 12, 16, 18, 20, 24, 26, 31};
    
    printf("\nbars: ");
    for(int i=0; i<n; i++)
        printf(" %d", bars[i]);

    char filter[30] = {
        -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, 
        -1,  0,  1,  0, -1, -1, -1, -1,  1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    bit_arr_display(N, filter);
    
    line_info_t l = (line_info_t){{n, bars}, {25, filter}};

    char line[N];
    line_fill(N, line, n, places, bars, true);
    bit_arr_display(N, line);

    assert(line_next(N, line, places, &l));
    
    printf("\n-------------------");
    bit_arr_display(N, filter);
    bit_arr_display(N, line);

    assert(clu_mem_empty());
}

void test_sol2_line_scan_4()
{
    printf("\n\t%s", __func__);

    int N = 30;
    int n = 3;


    int bars[7] = {5, 1, 1, 1, 3, 1, 2};
    int places[8] = {6, 12, 16, 18, 20, 24, 26, 31};
    
    printf("\nbars: ");
    for(int i=0; i<n; i++)
        printf(" %d", bars[i]);

    char filter[30] = {
        -1, -1, -1, -1, -1, -1, -1,  1, -1, -1, 
        -1,  0,  1,  0, -1, -1, -1, -1,  1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
    };
    bit_arr_display(N, filter);
    
    line_info_t l = (line_info_t){{n, bars}, {25, filter}};

    char line[N];
    line_fill(N, line, n, places, bars, true);
    bit_arr_display(N, line);

    assert(line_next(N, line, places, &l));
    
    printf("\n-------------------");
    bit_arr_display(N, filter);
    bit_arr_display(N, line);

    assert(clu_mem_empty());
}




void test_sol2()
{
    printf("\n%s", __func__);

    // test_sol2_line_scan_1();
    // test_sol2_line_scan_2();
    test_sol2_line_scan_3();

    assert(clu_mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_sol2();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
