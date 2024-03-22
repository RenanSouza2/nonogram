#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_utils_spaces_get_tot()
{
    printf("\n\t%s", __func__);

    int spaces[] = {0, 1, 2, 3};
    assert_int(spaces_get_tot(1, spaces), 0);
    assert_int(spaces_get_tot(2, spaces), 1);
    assert_int(spaces_get_tot(3, spaces), 3);
    assert_int(spaces_get_tot(4, spaces), 6);

    assert(clu_mem_empty());
}

void test_utils_spaces_init()
{
    printf("\n\t%s", __func__);

    int spaces[] = {1, 2, 3};

    spaces_init(1, spaces);
    assert_int_arr(spaces, 3, 0, 2, 3);

    spaces_init(2, spaces);
    assert_int_arr(spaces, 3, 0, 0, 3);

    spaces_init(3, spaces);
    assert_int_arr(spaces, 3, 0, 0, 0);

    assert(clu_mem_empty());
}

void test_utils_spaces_is_valid()
{
    printf("\n\t%s", __func__);

    int spaces[] = {0};
    assert(spaces_is_valid(spaces) == true);

    spaces[0] = 1;
    assert(spaces_is_valid(spaces) == true);

    spaces[0] = -1;
    assert(spaces_is_valid(spaces) == false);

    assert(clu_mem_empty());
}

void test_utils_spaces_next()
{
    printf("\n\t%s", __func__);

    int spaces[4];
    spaces_next(0, spaces, 0);
    assert_int_arr(spaces, 1, -1);

    int_arr_set(spaces, 1, 0);
    spaces_next(1, spaces, 0);
    assert_int_arr(spaces, 1, -1);

    int_arr_set(spaces, 1, 0);
    spaces_next(1, spaces, 1);
    assert_int_arr(spaces, 1, 1);

    assert(clu_mem_empty());
}

void test_utils_space_generate()
{
    printf("\n\t%s", __func__);

    spaces_generate(1, 1);
    spaces_generate(1, 2);
    spaces_generate(2, 2);
    spaces_generate(3, 3);

    assert(clu_mem_empty());
}



void test_utils()
{
    printf("\n%s", __func__);

    test_utils_spaces_get_tot();
    test_utils_spaces_init();
    test_utils_spaces_is_valid();
    test_utils_spaces_next();

    test_utils_space_generate();

    assert(clu_mem_empty());
}



int main() 
{
    setbuf(stdout, NULL);
    test_utils();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
