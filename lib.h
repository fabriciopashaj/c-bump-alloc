#ifndef _CBUMP_ALLOC_H_
#define _CBUMP_ALLOC_H_
#include <stdint.h>
#define __inline__ static inline __attribute__((always_inline))
#define SIZEOF_SIZE_T (sizeof(size_t))
#define ALIGN(size) (((size) + 7) & ~7)

typedef struct BumpAlloc      BumpAlloc;
typedef struct FixedBumpAlloc FixedBumpAlloc;
#define BUMP_ALLOC_OK 0
#define BUMP_ALLOC_CANT_FIT 1
#define BUMP_ALLOC_FULL 2
#define BUMP_ALLOC_NOTHING 3

typedef uint32_t AllocStatus;

struct BumpAlloc {
	void   *data;
	void   *pointer;
	size_t size;
	size_t used;
};
struct FixedBumpAlloc {
	void   *data;
	size_t used;
	size_t size;
	size_t item_size;
};

__inline__
void BumpAlloc_init(BumpAlloc *ainfo, void *data, size_t size)
{
	ainfo->data = ainfo->pointer = data;
	ainfo->size = size;
	ainfo->used = 0;
}
__inline__
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
__inline__
void BumpAlloc_free(BumpAlloc *ainfo, void *ptr) {
	size_t size = *(size_t *)ptr - 1;
	if (ptr + size > ainfo->pointer)
		return;
	ainfo->used -= size + SIZEOF_SIZE_T;
	ainfo->pointer -= size + SIZEOF_SIZE_T;
}

__inline__
void FixedBumpAlloc_init(FixedBumpAlloc *ainfo, void *data,
						 size_t item_size, size_t size)
{
	ainfo->data = data;
	ainfo->item_size = item_size;
	ainfo->size = size;
	ainfo->used = 0;
}
__inline__
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

#undef __inline__
#undef SIZEOF_SIZE_T

#endif /* _CBUMP_ALLOC_H_ */
