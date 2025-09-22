#include "test.h"

// main
int main(void) {
  int no_failed;
  Suite *s, *ss, *sss, *ssss, *sssss;
  SRunner *runner;

  s = convertors_suite();
  ss = comparison_suite();
  sss = another_suite();
  ssss = helper_suite();
  sssss = arithmetic_suite();

  runner = srunner_create(s);
  srunner_add_suite(runner, ss);
  srunner_add_suite(runner, sss);
  srunner_add_suite(runner, ssss);
  srunner_add_suite(runner, sssss);

  srunner_run_all(runner, CK_NORMAL);
  no_failed = srunner_ntests_failed(runner);
  srunner_free(runner);
  return no_failed == 0 ? 0 : 1;
}