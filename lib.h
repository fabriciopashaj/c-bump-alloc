#ifndef _CBUMP_ALLOC_H_
#define _CBUMP_ALLOC_H_
#include <stdint.h>
#define __inline__ static inline __attribute__((always_inline))

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
AllocStatus BumpAlloc_alloc(BumpAlloc *, size_t, void **);
void BumpAlloc_free(BumpAlloc *, void *);

__inline__
void FixedBumpAlloc_init(FixedBumpAlloc *ainfo, void *data,
                         size_t item_size, size_t size)
{
  ainfo->data = data;
  ainfo->item_size = item_size;
  ainfo->size = size;
  ainfo->used = 0;
}
AllocStatus FixedBumpAlloc_alloc(FixedBumpAlloc *, int, void **);


#undef __inline__

#endif /* _CBUMP_ALLOC_H_ */
