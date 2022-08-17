# c-bump-alloc
Bump allocators out of the box for whatever reason you need them in your C program. With each allocator initialised, a chunk of memory with the size provided by you will be `malloc()`'d. That is the memory that the allocator will work with.

This library provides two kinds of allocators: the variable one and the fixed one.

## `BumpAllocator`
With this allocator you have to pass the size of the memory you want to allocate and that size will be stored together with the memory that is reserved for you. The advantages are that you can allocate any size for any item and the drawbacks are that more memory is used to save the size. Try not to allocate a lot of small things continually, or there will be more memory used for storing the sizes than the actual useful data.

### Example
For examples look at the [tests](https://github.com/fabricopashaj/c-bump-alloc/tree/main/blob/test/lib_test.c).

## NOTE
Bump allocators work more like stacks, which means deallocations are only possible on the last allocated memory and that reallocations are impossible.

