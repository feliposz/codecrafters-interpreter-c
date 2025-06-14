#ifndef clox_memory_h
#define clox_memory_h

#include "object.h"
#include "vm.h"

#define GC_HEAP_GROW_FACTOR 2

#define GROW_CAPACITY(capacity) ((capacity) < 8 ? 8 : (capacity) * 2)
#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type *)reallocate(pointer, sizeof(type) * (oldCount), sizeof(type) * (newCount))
#define FREE_ARRAY(type, pointer, oldCount) \
    (type *)reallocate(pointer, sizeof(type) * (oldCount), 0)
#define FREE(type, pointer) \
    (type *)reallocate(pointer, sizeof(type), 0)
#define ALLOCATE(type, count) \
    (type *)reallocate(NULL, 0, sizeof(type) * count)

void *reallocate(void *pointer, size_t oldSize, size_t newSize);
void freeObjects();
void markObject(Obj *object);
void markValue(Value value);
void collectGarbage();

#endif