#ifndef au_detail_StackAllocator_h
#define au_detail_StackAllocator_h

#include <new>

namespace au
{

/// \brief Allocate an object, passing \args to its constructor.
template <typename T, typename... Args>
T* StackAllocator::alloc_object(Args&&... args)
{
    if (m_top + sizeof(T) > m_end) {
        return nullptr;
    }
    else {
        auto tmp = new ((void*)m_top) T(std::forward<Args>(args)...);
        const byte_ptr ctop = m_top;
        m_top += sizeof(T);
        return (T*)ctop;
    }
}

/// \brief Allocate an array of objects, passings \args to their constructors
template <typename T, typename... Args>
T* StackAllocator::alloc_objects(size_t num_objects, Args&&... args)
{
    if (m_top + num_objects * sizeof(T) > m_end) {
        return nullptr;
    }
    else {
        for (size_t i = 0; i < num_objects; ++i) {
            (void*)new ((void*)(&m_top[i * sizeof(T)])) T(std::forward<Args>(args)...);
        }
        const byte_ptr ctop = m_top;
        m_top += num_objects * sizeof(T);
        return (T*)ctop;
    }
}

} // namespace au

#endif
