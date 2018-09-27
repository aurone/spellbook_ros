#ifndef au_mempool_h
#define au_mempool_h

#include <stdlib.h>

namespace au
{

/// \class mempool
///
/// \description Nothing fancy, just a container for a region of memory that you
///     have created somehow via stack allocation, new, etc. You are responsible
///     for maintaining allocation and deallocation of this region of memory.
struct mempool
{
public:

    mempool();
    mempool(void* buff, size_t num_bytes);

    void setBuff(void* buff, size_t num_bytes);

    void* buff() { return m_buff; }
    const void* buff() const { return m_buff; }

    size_t num_bytes() const { return m_num_bytes; }

private:

    void* m_buff;
    size_t m_num_bytes;
};

} // namespace au

#endif
