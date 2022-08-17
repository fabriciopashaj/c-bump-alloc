#include <stdlib.h>
#include <stddef.h>
#include "testing.h"
#include "lib.h"

void test_BumpAlloc_alloc_in_stack(void) {
  char arena[0x100];
  CU_ASSERT_PTR_NOT_NULL_FATAL(arena);
  BumpAlloc info;
  BumpAlloc_init(&info, arena, 0x100);
  void *ptr = NULL;
  int status = BumpAlloc_alloc(&info, 20, &ptr);
  CU_ASSERT_EQUAL_FATAL(status, BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL(ptr, arena + sizeof(size_t));
  CU_ASSERT_EQUAL_FATAL(info.used, sizeof(size_t) + 24); // the size is
                                                         // aligned by 8
  CU_ASSERT_EQUAL_FATAL(info.pointer, arena + info.used);
}

void test_BumpAlloc_alloc_in_heap(void) {
  void *arena = malloc(0x1000);
  CU_ASSERT_PTR_NOT_NULL_FATAL(arena);
  BumpAlloc info;
  BumpAlloc_init(&info, arena, 0x1000);
  void *ptr = NULL;
  int status = BumpAlloc_alloc(&info, 20, &ptr);
  CU_ASSERT_EQUAL_FATAL(status, BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL(ptr, arena + sizeof(size_t));
  CU_ASSERT_EQUAL_FATAL(info.used, sizeof(size_t) + 24); // the size is
                                                         // aligned by 8
  CU_ASSERT_EQUAL_FATAL(info.pointer, arena + info.used);
  free(arena);
}

// XXX: If you can provide better examples, please do so

void test_FixedBumpAlloc_alloc_in_stack(void)
{
  char arena[0x100];
  // void *arena = malloc(0x1000);
  CU_ASSERT_PTR_NOT_NULL_FATAL(arena);
  FixedBumpAlloc info;
  FixedBumpAlloc_init(
      &info, arena, sizeof(FILE *), 0x100 / sizeof(FILE *)
  );
  FILE **ptr;
  CU_ASSERT_EQUAL_FATAL(FixedBumpAlloc_alloc(&info, 1, (void *)&ptr),
                        BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL((void *)ptr, arena + sizeof(FILE *) * 0);
  CU_ASSERT_EQUAL_FATAL(info.used, 1); // the size is
                                                        // aligned by 8
  *ptr = stdin;
  printf("%p, %p\n", arena, ptr);
  CU_ASSERT_EQUAL_FATAL(FixedBumpAlloc_alloc(&info, 1, (void *)&ptr),
                        BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL((void *)ptr, arena + sizeof(FILE *) * 1);
  CU_ASSERT_EQUAL_FATAL(info.used, 2); // the size is
                                                        // aligned by 8
  *ptr = stdout;
  CU_ASSERT_EQUAL_FATAL(FixedBumpAlloc_alloc(&info, 1, (void *)&ptr),
                        BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL((void *)ptr, arena + sizeof(FILE *) * 2);
  CU_ASSERT_EQUAL_FATAL(info.used, 3); // the size is
                                                        // aligned by 8
  *ptr = stderr;
}

void test_FixedBumpAlloc_alloc_in_heap(void)
{
  void *arena = malloc(0x1000);
  CU_ASSERT_PTR_NOT_NULL_FATAL(arena);
  FixedBumpAlloc info;
  FixedBumpAlloc_init(
      &info, arena, sizeof(FILE *), 0x100 / sizeof(FILE *)
  );
  FILE **ptr;
  CU_ASSERT_EQUAL_FATAL(FixedBumpAlloc_alloc(&info, 1, (void *)&ptr),
                        BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL((void *)ptr, arena + sizeof(FILE *) * 0);
  CU_ASSERT_EQUAL_FATAL(info.used, 1); // the size is
                                                        // aligned by 8
  *ptr = stdin;
  CU_ASSERT_EQUAL_FATAL(FixedBumpAlloc_alloc(&info, 1, (void *)&ptr),
                        BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL((void *)ptr, arena + sizeof(FILE *) * 1);
  CU_ASSERT_EQUAL_FATAL(info.used, 2); // the size is
                                                        // aligned by 8
  *ptr = stdout;
  CU_ASSERT_EQUAL_FATAL(FixedBumpAlloc_alloc(&info, 1, (void *)&ptr),
                        BUMP_ALLOC_OK);
  CU_ASSERT_EQUAL_FATAL((void *)ptr, arena + sizeof(FILE *) * 2);
  CU_ASSERT_EQUAL_FATAL(info.used, 3); // the size is
                                                        // aligned by 8
  *ptr = stderr;
  free(arena);
}

int main(int argc, char **argv) {
  int status = EXIT_SUCCESS;
  CU_TestInfo tests[] = {
    { "alloc/in stack", test_BumpAlloc_alloc_in_stack },
    { "alloc/in heap",  test_BumpAlloc_alloc_in_heap  },
    CU_TEST_INFO_NULL
  };
  CU_TestInfo tests_fixed[] = {
    { "alloc(in stack)", test_FixedBumpAlloc_alloc_in_stack },
    { "alloc(in heap)",  test_FixedBumpAlloc_alloc_in_heap  },
    CU_TEST_INFO_NULL
  };
  CU_SuiteInfo suites[] = {
    { "BumpAlloc",      NULL, NULL, NULL, NULL, tests       },
    { "FixedBumpAlloc", NULL, NULL, NULL, NULL, tests_fixed },
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
