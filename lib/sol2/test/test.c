#include "../debug.h"
#include "../../../utils/assert.h"
#include "../../../utils/clu/bin/header.h"



void test_sol2()
{
    printf("\n\t%s", __func__);

    assert(clu_mem_empty());
}



void test_sol2()
{
    printf("\n%s", __func__);

    assert(clu_mem_empty());
}


int main() 
{
    setbuf(stdout, NULL);
    test_sol2();
    printf("\n\n\tTest successful\n\n");
    return 0;
}
