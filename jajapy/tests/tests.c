#include <check.h>
#include "test_symbolic_to_numeric.h"
#include "test_forward.h"

int main(void) {
    int number_failed;
    SRunner *sr;

    Suite* s1 = symbolic_to_numeric_suite();
    Suite* s2 = forward_suite();
    Suite* s3 = matrix_to_add_suite();

    sr = srunner_create(s1);
    srunner_add_suite(sr, s2);
    srunner_add_suite(sr, s3);
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}
