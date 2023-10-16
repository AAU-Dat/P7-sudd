#include <check.h>
#include "test_symbolic_to_numeric.h"

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = symbolic_to_numeric_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}
