#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_utils_space_generate()
{
    printf("\n%s", __func__);

    spaces_generate(1, 1);

    assert(clu_mem_empty());
}



void test_utils()
{
    printf("\n%s", __func__);

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
