#ifndef au_detail_heap_h
#define au_detail_heap_h

#include "heap.h"

namespace au {

static inline std::size_t ipow2(std::size_t exp)
{
    return (exp == 0) ? 1 : ((exp % 2) ? 2 : 1) * ipow2(exp >> 1) * ipow2(exp >> 1);
}

static inline std::size_t ilog2(std::size_t i)
{
    std::size_t r = 0;
    while (i >>= 1) {
        ++r;
    }
    return r;
}

static inline bool ispow2(std::size_t val)
{
    // does not check for val == 0
    return !(val & (val - 1));
}

template <typename T, typename Compare>
heap<T, Compare>::heap(const Compare& comp, const std::vector<value_type>& elements) :
    m_elements(),
    m_comp(comp)
{
    make_heap(elements);
}

template <typename T, typename Compare>
heap<T, Compare>::heap(const heap& other) :
    m_elements(other.m_elements),
    m_comp(other.m_comp)
{
}

template <typename T, typename Compare>
heap<T, Compare>& heap<T, Compare>::operator=(const heap& rhs)
{
    if (this != &rhs) {
        m_elements = rhs.m_elements;
        m_comp = rhs.m_comp;
    }
    return *this;
}

template <typename T, typename Compare>
const typename heap<T, Compare>::value_type& heap<T, Compare>::min() const
{
    return value(*m_elements[1]);
}

template <typename T, typename Compare>
bool heap<T, Compare>::empty() const
{
    return m_elements.size() == 1;
}

template <typename T, typename Compare>
typename heap<T, Compare>::size_type
heap<T, Compare>::size() const
{
    return m_elements.size() - 1;
}

template <typename T, typename Compare>
typename heap<T, Compare>::size_type
heap<T, Compare>::max_size() const
{
    return m_elements.max_size() - 1;
}

template <typename T, typename Compare>
void heap<T, Compare>::reserve(size_type new_cap)
{
    m_elements.reserve(new_cap + 1);
}

template <typename T, typename Compare>
void heap<T, Compare>::clear()
{
    for (size_type i = 1; i < m_elements.size(); ++i) {
        std::shared_ptr<element_type>& elem = m_elements[i];
        pos(*elem) = 0;
    }
    this->make_heap({});
}

template <typename T, typename Compare>
typename heap<T, Compare>::handle_type
heap<T, Compare>::push(const value_type& value)
{
    std::shared_ptr<element_type> new_element(new element_type(value, m_elements.size()));
    m_elements.push_back(new_element);

    handle_type handle;
    handle.elem_ = new_element;

    percolate_up(m_elements.size() - 1);
    return handle;
}

template <typename T, typename Compare>
void heap<T, Compare>::pop()
{
    // todo: inverse percolate up for cache performance?
    std::shared_ptr<element_type> tmp = m_elements[1];
    pos(*tmp) = 0;

    m_elements[1] = m_elements.back();
    pos(*m_elements[1]) = 1;

    m_elements.pop_back();
    percolate_down(1);
}

template <typename T, typename  Compare>
bool heap<T, Compare>::contains(const handle_type& handle) const
{
    return handle.valid() && pos(*handle.elem_) != 0;
}

template <typename T, typename Compare>
void heap<T, Compare>::swap(heap& other)
{
    m_elements.swap(other.m_elements);
    std::swap(m_comp, other.m_comp);
}

template <typename T, typename Compare>
typename heap<T, Compare>::const_iterator heap<T, Compare>::begin() const
{
    const_iterator it;
    it.elem_ = ((size() == 0) ? nullptr : &m_elements[1]);
    return it;
}

template <typename T, typename Compare>
typename heap<T, Compare>::const_iterator heap<T, Compare>::end() const
{
    const_iterator it;
    it.elem_ = (&m_elements.back()) + 1;
    return it;
}

template <typename T, typename Compare>
typename heap<T, Compare>::handle_type
heap<T, Compare>::s_iterator_to_handle(const const_iterator& it)
{
    handle_type handle;
    handle.elem_ = it.elem_ ? *it.elem_ : std::shared_ptr<element_type>(nullptr);
    return handle;
}

template <typename T, typename Compare>
void heap<T, Compare>::update(const handle_type& handle, const value_type& v)
{
    bool less = m_comp(v, value(*handle.elem_));
    value(*handle.elem_) = v;
    if (less) {
        percolate_up(pos(*handle.elem_));
    }
    else {
        percolate_down(pos(*handle.elem_));
    }
}

template <typename T, typename Compare>
void heap<T, Compare>::increase(const handle_type& handle, const value_type& v)
{
    value(*handle.elem_) = v;
    percolate_down(pos(*handle.elem_));
}

template <typename T, typename Compare>
void heap<T, Compare>::decrease(const handle_type& handle, const value_type& v)
{
    value(*handle.elem_) = v;
    percolate_up(pos(*handle.elem_));
}

template <typename T, typename Compare>
void heap<T, Compare>::erase(const handle_type& handle)
{
    // todo: see pop()
    if (handle.elem_) {
        size_type pos = pos(*handle.elem_);
        std::shared_ptr<element_type>& top = m_elements[pos];
        m_elements[pos] = m_elements.back();
        pos(*m_elements[pos]) = pos;
        pos(*top) = 0;
        m_elements.pop_back();
        percolate_down(pos);
    }
}

template <typename T, typename Compare>
void heap<T, Compare>::percolate_down(size_type pivot)
{
    if (this->is_external(pivot)) {
        return; // percolate_down on empty heap (after final pop())
    }

    size_type left = left_child(pivot);
    size_type right = right_child(pivot);
    size_type start = pivot;
    std::shared_ptr<element_type> tmp = m_elements[start];
    while (is_internal(left)) {
        size_type s = right;
        if (is_external(right) || m_comp(value(*m_elements[left]), value(*m_elements[right]))) {
            s = left;
        }

        if (m_comp(value(*m_elements[s]), value(*tmp))) {
            m_elements[pivot] = m_elements[s];
            pos(*m_elements[pivot]) = pivot;
            pivot = s;
        }
        else {
            break;
        }
        left = left_child(pivot);
        right = right_child(pivot);
    }
    m_elements[pivot] = tmp;
    pos(*m_elements[pivot]) = pivot;
}

template <typename T, typename Compare>
void heap<T, Compare>::percolate_up(size_type pivot)
{
    std::shared_ptr<element_type> tmp = m_elements[pivot];
    while (pivot != 1) {
        size_type p = parent(pivot);
        if (m_comp(value(*m_elements[p]), value(*tmp))) {
            break;
        }
        else {
            m_elements[pivot] = m_elements[p];
            pos(*m_elements[pivot]) = pivot;
            pivot = p;
        }
    }
    m_elements[pivot] = tmp;
    pos(*m_elements[pivot]) = pivot;
}

template <typename T, typename Compare>
void heap<T, Compare>::make_heap(const std::vector<value_type>& elements)
{
    m_elements.resize(elements.size() + 1);
    make_heap(elements, 1, 0, elements.size());
}

template <typename T, typename Compare>
typename heap<T, Compare>::size_type heap<T, Compare>::height() const
{
    return m_elements.size() < 2 ? 0 : (ilog2(m_elements.size()) + (ispow2(m_elements.size())) ? 0 : 1);
}

template <typename T, typename Compare>
void heap<T, Compare>::make_heap(
    const std::vector<value_type>& elements,
    size_type root,
    size_type start,
    size_type end)
{
    if ((end - start) > 0) {
        m_elements[root].reset(new element_type(elements[start], root));
        size_type left = left_child(root);
        size_type right = right_child(root);

        size_type n = end - start;
        if (n == 1) {
            return;
        }

        size_type f = ilog2(n) - 1;
        size_type f2 = ipow2(f);
        size_type l = f2 - 1 + std::min(n - 2 * f2 + 1, f2);
        size_type r = n - 1 - l;

        size_type new_start = start + 1;
        size_type mid = new_start + l;

        make_heap(elements, left, new_start, mid);
        make_heap(elements, right, mid, end);
        percolate_down(root);
    }
}

template <typename T, typename Compare>
bool heap<T, Compare>::check_heap() const
{
    return check_heap(1);
}

template <typename T, typename Compare>
bool heap<T, Compare>::check_heap(size_type curr) const
{
    if (is_external(curr)) {
        return true;
    }

    size_type left = left_child(curr);
    size_type right = right_child(curr);
    // :(
    if (is_internal(left) && (m_elements[curr]->first > m_elements[left]->first)) {
        return false;
    }
    if (is_internal(right) && (m_elements[curr]->first > m_elements[right]->first)) {
        return false;
    }

    return check_heap(left) && check_heap(right);
}

////////////////////////////////////////////////////////////////////////////////
// heap::handle_type implementation
////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Compare>
heap<T, Compare>::handle_type::handle_type() :
    elem_(nullptr)
{ }

template <typename T, typename Compare>
const typename heap<T, Compare>::value_type&
heap<T, Compare>::handle_type::operator*() const
{ return *elem_.first; }

template <typename T, typename Compare>
bool heap<T, Compare>::handle_type::valid() const
{ return (bool)elem_; }

////////////////////////////////////////////////////////////////////////////////
// heap::const_iterator implementation
////////////////////////////////////////////////////////////////////////////////

template <typename T, typename Compare>
heap<T, Compare>::const_iterator::const_iterator() :
    elem_(nullptr)
{ }

template <typename T, typename Compare>
typename heap<T, Compare>::const_iterator heap<T, Compare>::const_iterator::operator++(int)
{ const_iterator o(*this); ++elem_; return o; }

template <typename T, typename Compare>
typename heap<T, Compare>::const_iterator& heap<T, Compare>::const_iterator::operator++()
{ ++elem_; return *this; }

template <typename T, typename Compare>
const typename heap<T, Compare>::value_type* heap<T, Compare>::const_iterator::operator->() const
{ return &((*elem_)->first); }

template <typename T, typename Compare>
const typename heap<T, Compare>::value_type& heap<T, Compare>::const_iterator::operator*() const
{ return (*elem_)->first; }

template <typename T, typename Compare>
bool heap<T, Compare>::const_iterator::operator==(const_iterator other) const
{ return elem_ == other.elem_; }

template <typename T, typename Compare>
bool heap<T, Compare>::const_iterator::operator!=(const_iterator other) const
{ return !operator==(other); }

template <typename T, typename Compare>
void swap(heap<T, Compare>& lhs, heap<T, Compare>& rhs)
{ return lhs.swap(rhs); }

} // namespace au

#endif
