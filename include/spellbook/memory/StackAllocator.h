#ifndef StackAllocator_h
#define StackAllocator_h

// C includes
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <utility> // std::forward

// module includes
#include "mempool.h"

namespace au
{

/// \class StackAllocator
/// \brief A general-purpose stack allocator
///
/// \description An allocator that allocates from memory in LIFO fashion.
///     Depending on the allocation method used, this allocator may (not) call
///     the constructor for each object allocated. This allocator does not ever
///     call the destructor for an object, so any object deconstruction is left
///     to the user.
class StackAllocator
{
public:

    typedef uint8_t byte;
    typedef byte* byte_ptr;
    typedef const byte* const_byte_ptr;

    typedef byte_ptr Marker;

    StackAllocator();
    StackAllocator(const mempool& pool);

    ~StackAllocator();

    void initialize(const mempool& pool);

    void* alloc(size_t num_bytes);
    template <typename T, typename... Args> T* alloc_object(Args&&... args);
    template <typename T, typename... Args> T* alloc_objects(size_t num_objects, Args&&... args);

    void rollback(Marker marker);
    void clear();

    size_t size() const;
    size_t capacity() const;
    Marker top() const { return m_top; }

private:

    mempool m_pool;
    byte_ptr m_top;
    byte_ptr m_start;
    byte_ptr m_end;
};

} // namespace au

#include "detail/StackAllocator.h"

#endif
