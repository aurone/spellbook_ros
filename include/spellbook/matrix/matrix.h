#ifndef au_matrix_h
#define au_matrix_h

// standard includes
#include <stdlib.h>
#include <initializer_list>
#include <ostream>
#include <stdexcept>

namespace au {

typedef size_t matrix_index;

template <typename T, matrix_index M, matrix_index N>
class matrix
{
public:

    typedef T value_type;
    typedef size_t size_type;

    static const matrix_index row_count = M;
    static const matrix_index col_count = N;

    static matrix identity();
    static matrix zeros();
    static matrix ones();

    matrix();

    template <typename U>
    matrix(std::initializer_list<U> init);

    template <typename InputIt>
    matrix(InputIt first, InputIt last);

    // assignment operators
    template <typename U> matrix& operator+=(U c);
    template <typename U> matrix& operator+=(const matrix<U, M, N>& rhs);
    template <typename U> matrix& operator-=(U c);
    template <typename U> matrix& operator-=(const matrix<U, M, N>& rhs);
    template <typename U> matrix& operator*=(U c);
    template <typename U> matrix& operator*=(const matrix<U, N, N>& rhs);
    template <typename U> matrix& operator/=(U c);

    T& operator()(matrix_index i, matrix_index j);
    const T& operator()(matrix_index i, matrix_index j) const;

    T& operator[](matrix_index i);
    const T& operator[](matrix_index i) const;

    const T* data() const;
    T* data();

private:

    T m_data[M * N];
};

//////////////////////////
// arithmetic operators //
//////////////////////////

template <typename T, matrix_index M, matrix_index N>
matrix<T, M, N> operator+(const matrix<T, M, N>& a);

template <typename T, matrix_index M, matrix_index N>
matrix<T, M, N> operator-(const matrix<T, M, N>& a);

template <typename T, matrix_index M, matrix_index N>
matrix<T, M, N> operator+(const matrix<T, M, N>& a, const matrix<T, M, N>& b);

template <typename T, matrix_index M, matrix_index N>
matrix<T, M, N> operator-(const matrix<T, M, N>& a, const matrix<T, M, N>& b);

template <typename T, matrix_index M, matrix_index N>
matrix<T, M, N> operator*(T c, const matrix<T, M, N>& m);

template <typename T, matrix_index M, matrix_index N>
matrix<T, M, N> operator*(const matrix<T, M, N>& m, T c);

template <typename T, matrix_index M, matrix_index N>
matrix<T, M, N> operator/(const matrix<T, M, N>& m, T c);

template <typename T, matrix_index M, matrix_index N, matrix_index K>
matrix<T, M, K> operator*(const matrix<T, M, N>& a, const matrix<T, N, K>& b);

template <typename T, matrix_index M, matrix_index N>
matrix<T, N, M> transpose(const matrix<T, M, N>& m);

template <typename T, matrix_index M, matrix_index N>
matrix<T, N, M> inverse(const matrix<T, M, N>& m);

template <typename T, matrix_index M, matrix_index N>
T determinant(const matrix<T, M, N>& m);

template <typename T, matrix_index N>
matrix<T, N, 1> linsolve(const matrix<T, N, N>& A, const matrix<T, N, 1>& b);

template <typename T, matrix_index M, matrix_index N>
std::ostream& operator<<(std::ostream& o, const matrix<T, M, N>& m);

template <typename T> using matrix1 = matrix<T, 1, 1>;
template <typename T> using matrix2 = matrix<T, 2, 2>;
template <typename T> using matrix3 = matrix<T, 3, 3>;
template <typename T> using matrix4 = matrix<T, 4, 4>;

template <typename T, matrix_index M> using vector = matrix<T, M, 1>;
template <typename T> using vector1 = vector<T, 1>;
template <typename T> using vector2 = vector<T, 2>;
template <typename T> using vector3 = vector<T, 3>;
template <typename T> using vector4 = vector<T, 4>;

#define DECLARE_MATRIX_TYPEDEFS(type, suff) \
typedef matrix1<type> matrix1##suff;\
typedef matrix2<type> matrix2##suff;\
typedef matrix3<type> matrix3##suff;\
typedef matrix4<type> matrix4##suff;

#define DECLARE_VECTOR_TYPEDEFS(type, suff) \
typedef vector1<type> vector1##suff;\
typedef vector2<type> vector2##suff;\
typedef vector3<type> vector3##suff;\
typedef vector4<type> vector4##suff;

DECLARE_MATRIX_TYPEDEFS(bool,       b)

DECLARE_MATRIX_TYPEDEFS(short int,  s)
DECLARE_MATRIX_TYPEDEFS(int,        i)
DECLARE_MATRIX_TYPEDEFS(long int,   l)
DECLARE_MATRIX_TYPEDEFS(long long,  ll)

DECLARE_MATRIX_TYPEDEFS(unsigned short int,  us)
DECLARE_MATRIX_TYPEDEFS(unsigned int,        ui)
DECLARE_MATRIX_TYPEDEFS(unsigned long int,   ul)
DECLARE_MATRIX_TYPEDEFS(unsigned long long,  ull)

DECLARE_MATRIX_TYPEDEFS(float,          f)
DECLARE_MATRIX_TYPEDEFS(double,         d)
DECLARE_MATRIX_TYPEDEFS(long double,    ld)

DECLARE_VECTOR_TYPEDEFS(bool,       b)

DECLARE_VECTOR_TYPEDEFS(unsigned short int,  us)
DECLARE_VECTOR_TYPEDEFS(unsigned int,        ui)
DECLARE_VECTOR_TYPEDEFS(unsigned long int,   ul)
DECLARE_VECTOR_TYPEDEFS(unsigned long long,  ull)

DECLARE_VECTOR_TYPEDEFS(float,          f)
DECLARE_VECTOR_TYPEDEFS(double,         d)
DECLARE_VECTOR_TYPEDEFS(long double,    ld)

#undef DECLARE_MATRIX_TYPEDEFS
#undef DECLARE_VECTOR_TYPEDEFS

struct matrix_exception : public std::logic_error
{
    explicit matrix_exception(const std::string& what_arg);
    explicit matrix_exception(const char* what_arg);
};

} // namespace au

#include "detail/matrix.h"

#endif
