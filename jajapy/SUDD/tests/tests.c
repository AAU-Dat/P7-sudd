#include <check.h>
#include "test_forwards_backwards.h"
#include "test_conversion.h"

int main(void)
{
    int number_failed;
    SRunner* sr;

    Suite* s1 = conversion_suite();
    Suite* s2 = forwards_backwards_suite();

    sr = srunner_create(s1);
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_add_suite(sr, s2);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}
