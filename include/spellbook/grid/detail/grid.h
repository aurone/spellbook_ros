////////////////////////////////////////////////////////////////////////////////
// Copyright (c) 2014, Andrew Dornbush All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
////////////////////////////////////////////////////////////////////////////////

#ifndef au_detail_grid_h
#define au_detail_grid_h

#include "../grid.h"

// standard includes
#include <string.h>
#include <iostream>
#include <limits>
#include <sstream>

template <class T> typename std::add_rvalue_reference<T>::type val();
template <class T> struct id { typedef T type; };

template <class T, class... P> struct mul_type;

// single-type base case
template <class T> struct mul_type<T> : id<T> {};

template<class T, class U, class... P>
struct mul_type<T,U,P...> : id<decltype(val<T>() * val<typename mul_type<U,P...>::type>())> {};

template <typename A>
typename mul_type<A>::type mul(A a)
{
    return a;
}

template <typename A, typename B>
typename mul_type<A, B>::type mul(A a, B b)
{
    return a * b;
}

template <typename A0, typename A1, typename... Sizes>
typename mul_type<A0, A1, Sizes...>::type mul(A0 a0, A1 a1, Sizes... dims)
{
   return a0 * mul(a1, dims...);
}

namespace au
{

////////////////////////////////////////////////////////////////////////////////
// grid Implementation
////////////////////////////////////////////////////////////////////////////////

template <int N, typename T>
grid<N, T>::grid() :
    data_(nullptr)
{
    this->clear();
}

template <int N, typename T>
template <typename... sizes>
grid<N, T>::grid(sizes... counts) :
    data_(nullptr)
{
    static_assert(sizeof...(counts) == N, "Invalid dimensions passed to grid constructor");
    this->resize(counts...);
}

template <int N, typename T>
grid<N, T>::grid(const grid& other) :
    data_(nullptr)
{
    this->copy(other);
}

template <int N, typename T>
grid<N, T>& grid<N, T>::operator=(const grid& rhs)
{
    if (this != &rhs) {
        this->copy(rhs);
    }
    return *this;
}

template <int N, typename T>
grid<N, T>::grid(grid&& other) :
    data_(other.data_),
    dims_()
{
    // copy over dimensions
    memcpy(dims_, other.dims_, N * sizeof(size_type));

    // clear other
    other.data_ = nullptr;
    other.clear();
}

template <int N, typename T>
grid<N, T>& grid<N, T>::operator=(grid&& rhs)
{
    if (this != &rhs) {
        clear();

        data_ = rhs.data_;
        memcpy(dims_, rhs.dims_, N * sizeof(size_type));

        // clear other
        rhs.data_ = nullptr;
        rhs.clear();
    }
    return *this;
}

template <int N, typename T>
grid<N, T>::~grid()
{
    this->clear();
}

template <int N, typename T>
template <typename... CoordTypes>
typename grid<N, T>::reference grid<N, T>::operator()(CoordTypes... coords)
{
    static_assert(sizeof...(coords) == N, "Invalid number of coordinates passed to operator()");
    size_type ind = this->coord_to_index(coords...);
    return data_[ind];
}

template <int N, typename T>
template <typename... CoordTypes>
typename grid<N, T>::const_reference grid<N, T>::operator()(CoordTypes... coords) const
{
    return const_cast<const T&>(const_cast<grid<N, T>*>(this)->operator()(coords...));
}

template <int N, typename T>
template <typename... CoordTypes>
typename grid<N, T>::reference grid<N, T>::at(CoordTypes... coords)
{
    size_type ind = this->coord_to_index(coords...);
    if (!(ind < this->total_size())) {
        std::stringstream ss;
        ss << "invalid index " << ind << " into array of size " << this->total_size();
        throw std::out_of_range(ss.str());
    }
    return data_[ind];
}

template <int N, typename T>
template <typename... CoordTypes>
typename grid<N, T>::const_reference grid<N, T>::at(CoordTypes... coords) const
{
    return const_cast<const T&>(const_cast<grid<N, T>*>(this)->at(coords...));
}

template <int N, typename T>
auto grid<N, T>::operator()(const index& i) -> reference
{
    size_type ind = this->coord_to_index(i);
    return data_[ind];
}

template <int N, typename T>
auto grid<N, T>::operator()(const index& i) const -> const_reference
{
    return const_cast<const T&>(const_cast<grid<N, T>*>(this)->operator()(i));
}

template <int N, typename T>
auto grid<N, T>::at(const index& i) -> reference
{
    size_type ind = this->coord_to_index(i);
    if (!(ind < this->total_size())) {
        std::stringstream ss;
        ss << "invalid index " << ind << " into array of size " << this->total_size();
        throw std::out_of_range(ss.str());
    }
    return data_[ind];
}

template <int N, typename T>
auto grid<N, T>::at(const index& i) const -> const_reference
{
    return const_cast<const T&>(const_cast<grid<N, T>*>(this)->at(i));
}

template <int N, typename T>
template <typename... CoordTypes>
bool grid<N, T>::within_bounds(CoordTypes... coords) const
{
    return within_bounds<0>(coords...);
}

template <int N, typename T>
typename grid<N, T>::iterator grid<N, T>::begin()
{
    iterator it;
    it.grid_ = this;
    if (total_size() == 0) {
        // it.begin_ = [ 0, 0, .., 0 ]
        it.end_ = this->create_last_index();
        it.curr_ = this->create_last_index();
        it.inc_ = std::numeric_limits<size_type>::max();
    }
    else {
        // it.begin_ = [ 0, 0, .., 0 ]
        it.end_ = this->create_last_index();
        // it.curr_ = [ 0, 0,, .., 0 ]
        // it.inc_ = N - 1
    }
    return it;
}

template <int N, typename T>
typename grid<N, T>::iterator grid<N, T>::end()
{
    iterator it;
    it.grid_ = this;
    // it.begin_ = [ 0, 0, .., 0 ]
    it.end_ = this->create_last_index();
    it.curr_ = this->create_last_index();
    it.inc_ = std::numeric_limits<size_type>::max();
    return it;
}

template <int N, typename T>
const typename grid<N, T>::iterator
grid<N, T>::begin() const
{
    return const_cast<grid<N, T>*>(this)->begin();
}

template <int N, typename T>
const typename grid<N, T>::iterator
grid<N, T>::end() const
{
    return const_cast<grid<N, T>*>(this)->end();
}

template <int N, typename T>
typename grid<N, T>::iterator
grid<N, T>::gbegin(const index& begin, const index& end)
{
    iterator it;
    it.grid_ = this;
    if (total_size() == 0) {
        // it.begin_ = [ 0, 0, .., 0 ]
        it.end_ = this->create_last_index();
        it.curr_ = this->create_last_index();
        it.inc_ = std::numeric_limits<size_type>::max();
    }
    else {
        it.begin_ = begin;
        it.end_ = end;
        it.curr_ = begin;
        // it.inc_ = N - 1
    }
    return it;
}

template <int N, typename T>
const typename grid<N, T>::iterator
grid<N, T>::gbegin(const index& begin, const index& end) const
{
    return const_cast<typename grid<N, T>::const_iterator>(const_cast<grid<N, T>*>(this)->gbegin(begin, end));
}

template <int N, typename T>
typename grid<N, T>::iterator
grid<N, T>::gend(const index& begin, const index& end)
{
    iterator it;
    it.grid_ = this;
    it.begin_ = begin;
    it.end_ = end;
    it.curr_ = end;
    it.inc_ = std::numeric_limits<size_type>::max();
    return it;
}

template <int N, typename T>
const typename grid<N, T>::iterator
grid<N, T>::gend(const index& begin, const index& end) const
{
    return const_cast<typename grid<N, T>::const_iterator>(const_cast<grid<N, T>*>(this)->gend(begin, end));
}

template <int N, typename T>
typename grid<N, T>::size_type grid<N, T>::size(size_type dim) const
{
    return dims_[dim];
}

template <int N, typename T>
typename grid<N, T>::size_type grid<N, T>::total_size() const
{
    size_type s = 1;
    for (int i = 0; i < N; ++i) { s *= dims_[i]; }
    return s;
}

template <int N, typename T>
void grid<N, T>::clear()
{
    if (data_) {
        delete[] data_;
        data_ = nullptr;
    }

    for (int i = 0; i < N; ++i) {
        dims_[i] = 0;
    }
}

template <int N, typename T>
template <typename... SizeTypes>
void grid<N, T>::resize(SizeTypes... sizes)
{
    static_assert(sizeof...(sizes) == N, "resize requires same number of arguments as dimensions");

    if (data_) {
        grid<N, T> new_grid(sizes...);
        copy_grid(new_grid);
        *this = std::move(new_grid);
    }
    else {
        clear();
        size_type total_size = mul(sizes...);
        data_ = new T[total_size];
        set_sizes<0>(dims_, sizes...);
    }
}

//template <int N, typename T>
//void resize(const index& sizes)
//{
//
//}
//
//template <int N, typename T>
//void resize(const index& sizes, const value_type& value)
//{
//
//}

template <int N, typename T>
void grid<N, T>::assign(const T& value)
{
    for (size_type i = 0; i < this->total_size(); ++i) {
        data_[i] = value;
    }
}

template <int N, typename T>
void grid<N, T>::copy(const grid& other)
{
    this->clear();

    if (other.data_) {
        size_type other_size = other.total_size();
        data_ = new T[other_size];
        for (size_type i = 0; i < other_size; ++i) {
            data_[i] = other.data_[i];
        }
        for (int i = 0; i < N; ++i) {
            dims_[i] = other.dims_[i];
        }
    }
}

template <int N, typename T>
struct IndexCoordinatorBase
{
    typedef grid<N, T> grid_type;
    typedef typename grid_type::size_type size_type;
    typedef typename grid_type::index index_type;

    IndexCoordinatorBase(size_type& agg) : agg_(agg) { }
    size_type& agg_;
};

template <int N, typename T, int DIM>
struct IndexCoordinator;

template <int N, typename T, int DIM>
struct IndexCoordinator : public IndexCoordinatorBase<N, T>
{
    typedef IndexCoordinatorBase<N, T> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::size_type size_type;
    typedef typename base::index_type index_type;

    IndexCoordinator(size_type& agg) : base(agg) { }

    size_type operator()(const grid_type& grid, const index_type& i)
    {
        size_type s = IndexCoordinator<N, T, DIM+1>(base::agg_)(grid, i);
        if (DIM + 1 != N) {
            base::agg_ *= grid.size(DIM + 1);
        }
        return base::agg_ * i(DIM) + s;
    }
};

template <int N, typename T>
struct IndexCoordinator<N, T, N> : public IndexCoordinatorBase<N, T>
{
    typedef IndexCoordinatorBase<N, T> base;
    typedef typename base::grid_type grid_type;
    typedef typename base::size_type size_type;
    typedef typename base::index_type index_type;

    IndexCoordinator(size_type& agg) : base(agg) { }

    size_type operator()(const grid_type& grid, const index_type& i)
    {
        return 0;
    }
};

template <int N, typename T>
typename grid<N, T>::size_type grid<N, T>::coord_to_index(const index& i)
{
    std::size_t agg = 1;
    return IndexCoordinator<N, T, 0>(agg)(*this, i);
}

template <int N, typename T>
template <typename... CoordTypes>
typename grid<N, T>::size_type grid<N, T>::coord_to_index(CoordTypes... coords)
{
    size_type s = 1;
    return this->coord_to_index_rec<0>(s, coords...);
}

template <int N, typename T>
template <int DIM, typename... CoordTypes, typename Coord>
typename grid<N, T>::size_type grid<N, T>::coord_to_index_rec(size_type& agg, Coord coord, CoordTypes... coords)
{
    size_type s = this->coord_to_index_rec<DIM+1>(agg, coords...);
    agg *= dims_[DIM + 1];
    return agg * coord + s;
}

template <int N, typename T>
template <int DIM, typename Coord>
typename grid<N, T>::size_type grid<N, T>::coord_to_index_rec(size_type& agg, Coord coord)
{
    static_assert(DIM == N - 1, "Something is wrong");
//    agg *= dims_[DIM];
    return coord;
}

template <int N, typename T>
template <int DIM, typename Coord, typename... CoordTypes>
bool grid<N, T>::within_bounds(Coord coord, CoordTypes... coords) const
{
    return (size_type)coord >= 0 && (size_type)coord < dims_[DIM] && within_bounds<DIM+1>(coords...);
}

template <int N, typename T>
template <int DIM, typename Coord>
bool grid<N, T>::within_bounds(Coord coord) const
{
    static_assert(DIM == N - 1, "Something is wrong");
    return (size_type)coord >= 0 && (size_type)coord < dims_[DIM];
}

template <int N, typename T, int DIM>
struct IndexSizeFiller
{
    void operator()(const grid<N, T>& g, typename grid<N, T>::index& i)
    {
        i(DIM) = g.size(DIM) - 1;
        IndexSizeFiller<N, T, DIM+1>()(g, i);
    }
};

template <int N, typename T>
struct IndexSizeFiller<N, T, N>
{
    void operator()(const grid<N, T>& g, typename grid<N, T>::index& i)
    {
    }
};

template <int N, typename T>
auto grid<N, T>::create_last_index() const -> typename grid<N, T>::index
{
    index i;
    IndexSizeFiller<N, T, 0>()(*this, i);
    return i;
}

template <int N, typename T>
void grid<N, T>::copy_grid(grid& g)
{
    index start; // [ 0, 0, .., 0 ]

    index end = create_last_index();
    for (int i = 0; i < N; ++i) {
        if (end(i) > g.size(i) - 1) {
            end(i) = g.size(i) - 1;
        }
    }

    // copy over old data into new buffer; shouldn't copy anything if the
    // current data buffer is null
    for (auto git = gbegin(start, end); git != gend(start, end); ++git) {
        g(git.cindex()) = *git;
    }
}

////////////////////////////////////////////////////////////////////////////////
// grid_index Implementation
////////////////////////////////////////////////////////////////////////////////

template <int N, typename T, int DIM>
struct IndexAssigner
{
    void operator()(grid_index<N, T>& ind, typename grid<N, T>::size_type coord)
    {
        ind(DIM) = coord;
        IndexAssigner<N, T, DIM+1>()(ind, coord);
    }
};

template <int N, typename T>
struct IndexAssigner<N, T, N>
{
    void operator()(grid_index<N, T>& ind, typename grid<N, T>::size_type coord)
    {
    }
};

template <int N, typename T>
grid_index<N, T>::grid_index()
{
    assign_all(0);
}

template <int N, typename T>
template <typename... Coords>
grid_index<N, T>::grid_index(Coords... coords)
{
    static_assert(sizeof...(coords) == N, "index constructor requires same number of arguments as dimensions");
    set_coords<0>(coords...);
}

template <int N, typename T>
void grid_index<N, T>::grid_index::assign_all(size_type coord)
{
    IndexAssigner<N, T, 0>()(*this, coord);
}

template <int N, typename T>
std::ostream& operator<<(std::ostream& o, const grid_index<N, T>& index)
{
    // TODO: unroll
    o << "(";
    for (int i = 0; i < N; ++i) {
        o << index(i);
        if (i != N - 1) {
            o << ", ";
        }
    }
    o << ")";
    return o;
}

////////////////////////////////////////////////////////////////////////////////
// grid_iterator Implementation
////////////////////////////////////////////////////////////////////////////////

template <int N, typename T>
grid_iterator<N, T>::grid_iterator() :
    grid_(nullptr),
    begin_(),   // 0, 0, ..., 0
    end_(),     // 0, 0, ..., 0
    curr_(),    // 0, 0, ..., 0
    inc_(N - 1)
{
}

template <int N, typename T>
grid_iterator<N, T>& grid_iterator<N, T>::grid_iterator::operator++()
{
    if (inc_ == std::numeric_limits<size_type>::max()) {
        return *this;
    }
    else if (curr_(inc_) < begin_(inc_) + size(inc_) - 1) {
        ++curr_(inc_);
        return *this;
    }
    else {
        --inc_;
        while (inc_ != std::numeric_limits<size_type>::max()) {
            if (curr_(inc_) < begin_(inc_) + size(inc_) - 1) {
                ++curr_(inc_);
                // unload the bases
                for (int j = inc_ + 1; j < N; ++j) {
                    curr_(j) = begin_(j);
                }
                inc_ = N - 1;
                break;
            }
            else {
                --inc_;
            }
        }
        return *this;
    }
}

template <int N, typename T>
grid_iterator<N, T> grid_iterator<N, T>::grid_iterator::operator++(int)
{
    grid_iterator git(*this);
    this->operator++();
    return git;
}

template <int N, typename T>
bool grid_iterator<N, T>::grid_iterator::operator==(const grid_iterator& other) const
{
    auto index_equals = [](const index& gi, const index& gj) -> bool
    {
        for (int i = 0; i < N; ++i) {
            if (gi(i) != gj(i)) {
                return false;
            }
        }
        return true;
    };

    // TODO: inc_ == other._inc => equal regardless?
    return index_equals(curr_, other.curr_) &&
           grid_ == other.grid_ &&
           index_equals(begin_, other.begin_) &&
           index_equals(end_, other.end_) &&
           inc_ == other.inc_;
}

template <int N, typename T>
bool grid_iterator<N, T>::grid_iterator::operator!=(const grid_iterator& other) const
{
    return !(this->operator==(other));
}

template <int N, typename T>
auto grid_iterator<N, T>::grid_iterator::operator*() -> value_type&
{
    return grid_->at(curr_);
}

template <int N, typename T>
auto grid_iterator<N, T>::grid_iterator::operator->() -> value_type*
{
    return grid_->at(curr_);
}

template <int N, typename T>
auto grid_iterator<N, T>::grid_iterator::operator--() -> grid_iterator&
{
    // TODO: implement
    return *this;
}

template <int N, typename T>
auto grid_iterator<N, T>::grid_iterator::operator--(int) -> grid_iterator
{
    grid_iterator git(*this);
    this->operator--();
    return git;
}

template <int N, typename T>
std::ostream& operator<<(std::ostream& o, const grid_iterator<N, T>& it)
{
    o << "{begin = " << it.begin_ << ", end = " << it.end_ << ", curr = " << it.curr_ << ", inc = " << it.inc_ << "}";
    return o;
}

} // namespace au

#endif
