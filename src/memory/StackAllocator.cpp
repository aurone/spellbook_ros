#include <spellbook/memory/StackAllocator.h>

namespace au
{

/// \brief Construct the default Stack Allocator
///
/// Construct the default Stack Allocator. No mempool is present, thus no valid
/// allocations may occur.
StackAllocator::StackAllocator() :
    m_pool(),
    m_top(nullptr),
    m_start(nullptr),
    m_end(nullptr)
{
}

/// \brief Construct the allocator with a given mempool
StackAllocator::StackAllocator(const mempool& pool) :
    m_pool(),
    m_top(nullptr),
    m_start(nullptr),
    m_end(nullptr)
{
    initialize(pool);
}

/// \brief Deconstruct the Stack Allocator
StackAllocator::~StackAllocator()
{
}

/// \brief (Re)Initialize a Stack Allocator with a given mempool
///
/// (Re)Initialize the Stack Allocator with a given mempool. Whether or not the
/// Stack Allocator already has an associated mempool, it will be cleared so as
/// to begin allocations from the start of the mempool.
///
/// \return true if the pool is valid (non-null); false otherwise
void StackAllocator::initialize(const mempool& pool)
{
    clear();
    m_pool = pool;
    m_start = (byte_ptr)m_pool.buff();
    m_end = (byte_ptr)m_pool.buff() + m_pool.num_bytes();
    m_top = m_start;
}

/// \brief Allocate a chunk of memory from the associated mempool
/// \return Pointer to the allocated chunk of memory or nullptr if there is an
///     insufficient amount of memory remaining.
void* StackAllocator::alloc(size_t num_bytes)
{
    if (this->m_top + num_bytes > this->m_end) {
        return nullptr;
    }
    else {
        const byte_ptr ctop = this->m_top;

        this->m_top += num_bytes;

        return (void*)ctop;
    }
}

/// \brief Return the amount, in bytes, of memory allocated
size_t StackAllocator::size() const
{
    return this->m_top - this->m_start;
}

/// \brief Return the maximum amount of memory that may be allocated
size_t StackAllocator::capacity() const
{
    return this->m_end - this->m_start;
}

/// \brief Deallocate all memory allocated up to \marker
/// \note Does not call the destructors of any allocated objects freed by this
///     function.
/// \param marker A position into the mempool, returned via top().
void StackAllocator::rollback(Marker marker)
{
    this->m_top = marker;
}

/// \brief Deallocate all allocated memory
/// \note Does not call the destructors of any allocated objects freed by this
///     function.
void StackAllocator::clear()
{
    this->rollback(this->m_start);
}

}  // namespace au
