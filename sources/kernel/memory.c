#include <brutal/alloc.h>
#include <brutal/debug.h>
#include "kernel/memory.h"

void memory_destroy(Memory *self)
{
    if (self->flags & MEMORY_OWNING)
    {
        if (self->type == MEMORY_HEAP)
        {
            heap_free(self->heap);
        }
        else
        {
            pmm_unused(self->pmm);
        }
    }

    alloc_free(alloc_global(), self);
}

Memory *memory_heap(HeapRange heap, MemoryFlags flags)
{
    Memory *self = alloc_make(alloc_global(), Memory);

    self->type = MEMORY_HEAP;
    self->flags = flags;
    self->heap = heap;

    object_init(base$(self), BR_OBJECT_MEMORY, (ObjectDtor *)memory_destroy);

    return self;
}

Memory *memory_pmm(PmmRange pmm, MemoryFlags flags)
{
    Memory *self = alloc_make(alloc_global(), Memory);

    self->type = MEMORY_PMM;
    self->flags = flags;
    self->pmm = pmm;

    object_init(base$(self), BR_OBJECT_MEMORY, (ObjectDtor *)memory_destroy);

    return self;
}

void memory_ref(Memory *self)
{
    object_ref(base$(self));
}

void memory_deref(Memory *self)
{
    object_deref(base$(self));
}

PmmRange memory_range(Memory *self)
{
    if (self->type == MEMORY_HEAP)
    {
        return heap_to_pmm(self->heap);
    }
    else
    {
        return self->pmm;
    }
}

uintptr_t memory_base(Memory *self)
{
    return memory_range(self).base;
}

size_t memory_size(Memory *self)
{
    return memory_range(self).size;
}
