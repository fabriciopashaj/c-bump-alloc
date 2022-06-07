#include <stdlib.h>
#include <stddef.h>
#include "testing.h"
#include "lib.h"
#define NULL4 NULL, NULL, NULL, NULL

void test_BumpAlloc_alloc(void) {
  void *arena = malloc(0x1000);
  CU_ASSERT_PTR_NOT_NULL_FATAL(arena);
  BumpAlloc info;
  BumpAlloc_init(&info, arena, 0x1000);
  void *ptr = NULL;
  int status = BumpAlloc_alloc(&info, 20, &ptr);
  CU_ASSERT_EQUAL_FATAL(status, BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL(ptr, arena + sizeof(size_t));
  CU_ASSERT_EQUAL_FATAL(info.used, sizeof(size_t) + 24);
  CU_ASSERT_EQUAL_FATAL(info.pointer, arena + info.used);
  free(arena);
}

int main(int argc, char **argv) {
  int status = EXIT_SUCCESS;
  CU_TestInfo tests[] = {
    { "alloc", test_BumpAlloc_alloc },
    CU_TEST_INFO_NULL
  };
  CU_SuiteInfo suites[] = {
    { "BumpAlloc", NULL4, tests },
    CU_SUITE_INFO_NULL
  };
  CU_initialize_registry();
  if (CU_register_suites(suites) != CUE_SUCCESS &&
    (status = EXIT_FAILURE))
    goto cleanup;
  RUN_TESTS;
cleanup:
  CU_cleanup_registry();
  return status;
}
