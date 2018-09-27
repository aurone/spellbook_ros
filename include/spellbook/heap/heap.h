#ifndef au_heap_h
#define au_heap_h

#include <algorithm>
#include <memory>
#include <queue>
#include <vector>

namespace au {

/// @brief A priority queue that supports the stl::priority_queue interface
///        as well as additional interfaces for mutability and traversability.
template <typename T, typename Compare = std::less<T>>
class heap
{
public:

    typedef T value_type;
    typedef Compare compare;

    typedef std::vector<value_type> container_type;
    typedef typename container_type::size_type size_type;

    typedef int key_type;

private:

    typedef std::pair<value_type, size_type> element_type; // todo: remove size_type in favor of pointer arithmetic?

public:

    struct handle_type
    {
        handle_type();
        const value_type& operator*() const;
        bool valid() const;

    private:

        friend class heap;
        std::shared_ptr<element_type> elem_;
    };

    struct const_iterator
    {
        const_iterator();

        const_iterator operator++(int);
        const_iterator& operator++();

        const value_type* operator->() const;
        const value_type& operator*() const;

        bool operator==(const_iterator other) const;
        bool operator!=(const_iterator other) const;

        friend class heap;

    private:

        const std::shared_ptr<element_type>* elem_;
    };

    explicit heap(const compare& comp = compare(), const container_type& elements = container_type());
    heap(const heap& other);
    heap& operator=(const heap& rhs);

    /// @{ Access
    const value_type& min() const;
    const value_type& top() const { return min(); }
    /// @}

    /// @{ Iterators
    const_iterator begin() const;
    const_iterator end() const;
    static handle_type s_iterator_to_handle(const const_iterator& it);
    /// @}

    /// @{ Capacity
    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    void reserve(size_type new_cap);
    /// @}

    /// @{ Modifiers
    void clear();
    handle_type push(const value_type& value);
    void pop();
    bool contains(const handle_type& handle) const;
    void update(const handle_type& handle, const value_type& v);
    void increase(const handle_type& handle, const value_type& v);
    void decrease(const handle_type& handle, const value_type& v);
    void erase(const handle_type& handle);
    void swap(heap& other);
    /// @}

private:

    std::vector<std::shared_ptr<element_type>> m_elements;
    Compare m_comp;

    inline size_type right_child(size_type index) const { return (index << 1) + 1; }
    inline size_type left_child(size_type index) const { return index << 1; }
    inline size_type parent(size_type index) const { return index >> 1; }
    bool is_internal(size_type index) const { return index < m_elements.size(); }
    bool is_external(size_type index) const { return index >= m_elements.size(); }
    void percolate_down(size_type pivot);
    void percolate_up(size_type pivot);
    void make_heap(const std::vector<value_type>& elements);
    void make_heap(const std::vector<value_type>& elements, size_type root, size_type start, size_type end);

    inline value_type& value(element_type& ele) const { return ele.first; }
    inline size_type& pos(element_type& ele) const { return ele.second; }

    bool check_heap() const;
    bool check_heap(size_type curr) const;
    size_type height() const;
};

template <typename T, typename Compare = std::less<T>>
void swap(heap<T, Compare>& lhs, heap<T, Compare>& rhs);

} // namespace au

#include "detail/heap.h"

#endif

