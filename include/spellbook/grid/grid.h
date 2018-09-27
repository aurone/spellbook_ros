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

#ifndef au_grid_h
#define au_grid_h

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdexcept>
#include <iostream>
#include <type_traits>

namespace au
{

// Assumptions:
// * N > 0
// * size(i) != 0 for all i

template <int N, typename T> class grid_iterator;
template <int N, typename T> class grid_index;

template <int N, typename T>
class grid
{
public:

    typedef size_t                      size_type;
    typedef T                           value_type;
    typedef T&                          reference;
    typedef const T&                    const_reference;

    typedef grid_iterator<N, T>         iterator;
    typedef const grid_iterator<N, T>   const_iterator;

    typedef grid_index<N, T>            index;

    grid();

    template <typename... sizes>
    grid(sizes... counts);

    grid(const grid& other);
    grid& operator=(const grid& rhs);

    grid(grid&& other);
    grid& operator=(grid&& rhs);

    ~grid();

    /// \name Element access
    ///@{
    template <typename... CoordTypes>
    reference operator()(CoordTypes... coords);

    template <typename... CoordTypes>
    const_reference operator()(CoordTypes... coords) const;

    template <typename... CoordTypes>
    reference at(CoordTypes... coords);

    template <typename... CoordTypes>
    const_reference at(CoordTypes... coords) const;

    reference operator()(const index& i);
    const_reference operator()(const index& i) const;

    reference at(const index& i);
    const_reference at(const index& i) const;

    T* data() { return data_; }

    template <typename... CoordTypes>
    bool within_bounds(CoordTypes... coords) const;
    ///@}

    /// \name Iterators
    ///@{
    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    iterator gbegin(const index& start, const index& end);
    const_iterator gbegin(const index& start, const index& end) const;
    iterator gend(const index& start, const index& end);
    const_iterator gend(const index& start, const index& end) const;
    ///@}

    /// \name Capacity
    ///@{
    size_type size(size_type dim) const;
    size_type total_size() const;
    ///@}

    /// \name Modifiers
    ///@{
    void clear();

    template <typename... SizeTypes>
    void resize(SizeTypes... sizes);

//    void resize(const index& sizes);
//    void resize(const index& sizes, const value_type& value);

    void assign(const T& value);
    ///@}

private:

    value_type* data_;
    size_type   dims_[N];

    void copy(const grid& other);

    template <size_type DIM, typename SizeType, typename... SizeTypes> // TODO: how to declare DIM in a separate implementation?
    void set_sizes(size_type* dims, SizeType size, SizeTypes... sizes)
    {
        static_assert(DIM < N-1, "Invalid dimension passed to set_sizes");
        dims[DIM] = size;
        this->set_sizes<DIM+1>(dims, sizes...);
    }

    template <size_type DIM, typename SizeType>
    void set_sizes(size_type* dims, SizeType size)
    {
        static_assert(DIM == N-1, "Invalid number of sizes passed to set_sizes");
        dims[N-1] = size;
    }

    size_type coord_to_index(const index& i);

    template <typename... CoordTypes>
    size_type coord_to_index(CoordTypes... coords);

    template <int DIM, typename... CoordTypes, typename Coord>
    size_type coord_to_index_rec(size_type& agg, Coord coord, CoordTypes... coords);

    template <int DIM, typename Coord>
    size_type coord_to_index_rec(size_type& agg, Coord coord);

    template <int DIM>
    void assign_sizes(index& i);

    template <int DIM, typename Coord, typename... CoordTypes>
    bool within_bounds(Coord coord, CoordTypes... coords) const;

    template <int DIM, typename Coord>
    bool within_bounds(Coord coord) const;

    index create_last_index() const;

    void copy_grid(grid& g);
};

template <int N, typename T>
class grid_index
{
public:

    typedef typename grid<N, T>::size_type size_type;

    grid_index();

    template <typename... CoordTypes>
    grid_index(CoordTypes... coords);

    size_type& operator()(size_type dim) { return coords_[dim]; }
    const size_type& operator()(size_type dim) const { return coords_[dim]; }

    bool operator<(const grid_index& rhs) const {
        for (int i = 0; i < N; ++i) {
            if (coords_[i] < rhs.coords_[i]) {
                return true;
            }
            else if (coords_[i] == rhs.coords_[i]) {
                // continue on
            }
            else {
                return false;
            }
        }

        return false; // equal
    }

private:

    size_type coords_[N];

    template <size_type DIM, typename SizeType, typename... SizeTypes>
    void set_coords(SizeType size, SizeTypes... sizes)
    {
        static_assert(DIM < N-1, "Invalid dimension passed to set_coords");
        coords_[DIM] = size;
        this->set_coords<DIM+1>(sizes...);
    }

    template <size_type DIM, typename SizeType>
    void set_coords(SizeType size)
    {
        static_assert(DIM == N-1, "Invalid number of sizes passed to set_coords");
        coords_[N-1] = size;
    }

    void assign_all(size_type coord);
};

template <int N, typename T>
std::ostream& operator<<(std::ostream&, const grid_index<N, T>&);

template <int N, typename T>
class grid_iterator;

template <int N, typename T>
std::ostream& operator<<(std::ostream&, const grid_iterator<N, T>&);

template <int N, typename T>
class grid_iterator : public std::iterator<
        std::bidirectional_iterator_tag,
        typename grid<N, T>::value_type>
{
    friend class grid<N, T>;
    friend std::ostream& operator<< <N, T>(std::ostream&, const grid_iterator<N, T>&);

public:

    typedef typename grid<N, T>::size_type      size_type;
    typedef typename grid<N, T>::value_type     value_type;
    typedef typename grid<N, T>::index          index;

    grid_iterator();

    /// \name Iterator API
    ///@{
    grid_iterator& operator++();
    grid_iterator operator++(int);

    bool operator==(const grid_iterator& other) const;
    bool operator!=(const grid_iterator& other) const;

    value_type& operator*();
    value_type* operator->();

    grid_iterator& operator--();
    grid_iterator operator--(int);
    ///@}

    const index& cindex() const { return curr_; }
    size_type coord(size_type dim) const { return curr_(dim); }

public:

    grid<N, T>* grid_;
    index begin_;
    index end_;
    index curr_;
    size_type inc_;     // index into curr_ to be (in/de)cremented next

    size_type size(size_type dim) const { return end_(dim) - begin_(dim) + 1; }
};

//template <int N, typename T>
//std::ostream& operator<<(std::ostream&, const grid_iterator<N, T>&);

} // namespace au

#endif

#include "detail/grid.h"
