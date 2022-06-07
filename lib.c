#include "lib.h"

#define SIZEOF_SIZE_T (sizeof(size_t))
#define ALIGN(size) (((size) + 7) & ~7)

AllocStatus BumpAlloc_alloc(BumpAlloc *ainfo, size_t size, void **ptr) {
  AllocStatus status = BUMP_ALLOC_OK;
  if (size & 0b111) size = ALIGN(size);
  if (ainfo->used >= ainfo->size ||
    ainfo->used + SIZEOF_SIZE_T + size > ainfo->size) {
    status = BUMP_ALLOC_CANT_FIT;
  } else {
    *(size_t *)(ainfo->pointer) = size;
    *ptr = ainfo->pointer + SIZEOF_SIZE_T;
    ainfo->used += SIZEOF_SIZE_T + size;
    ainfo->pointer += SIZEOF_SIZE_T + size;
  }
  return status;
}

void BumpAlloc_free(BumpAlloc *ainfo, void *ptr) {
  size_t size = *(size_t *)ptr - 1;
  if (ptr + size > ainfo->pointer)
    return;
  ainfo->used -= size + SIZEOF_SIZE_T;
  ainfo->pointer -= size + SIZEOF_SIZE_T;
}

AllocStatus FixedBumpAlloc_alloc(FixedBumpAlloc *ainfo, int count,
                 void **ptr)
{
  AllocStatus status = BUMP_ALLOC_OK;
  if (count == 0)
  {
    status = BUMP_ALLOC_NOTHING;
    goto end;
  }
  if (ainfo->used >= ainfo->size)
  {
    return BUMP_ALLOC_FULL;
  } else if (ainfo->used + count > ainfo->size)
  {
    status = BUMP_ALLOC_CANT_FIT;
  } else
  {
    *ptr = ainfo->data + ainfo->used * ainfo->item_size;
    ainfo->used += count;
  }
end:
  return status;
}
