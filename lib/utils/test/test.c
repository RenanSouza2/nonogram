#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_utils_int_arr_get_tot()
{
    printf("\n\t%s", __func__);

    int spaces[] = {0, 1, 2, 3};
    assert_int(int_arr_sum_reduce(1, spaces), 0);
    assert_int(int_arr_sum_reduce(2, spaces), 1);
    assert_int(int_arr_sum_reduce(3, spaces), 3);
    assert_int(int_arr_sum_reduce(4, spaces), 6);

    assert(clu_mem_empty());
}



void test_utils()
{
    printf("\n%s", __func__);

    test_utils_int_arr_get_tot();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_utils();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
