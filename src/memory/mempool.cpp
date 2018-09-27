#include <spellbook/memory/mempool.h>

namespace au
{

mempool::mempool() :
    m_buff(nullptr),
    m_num_bytes(0)
{
}

mempool::mempool(void* buff, size_t num_bytes) :
    m_buff(buff),
    m_num_bytes(num_bytes)
{
}

} // namespace au
