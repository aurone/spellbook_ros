#ifndef au_vec_h
#define au_vec_h

#include <ostream>

namespace au {

template <typename T>
struct vec2
{
    T x, y;

    vec2() : x(), y() { }
    vec2(T x, T y) : x(x), y(y) { }

    void normalize();
    T length() const;
    T length_sqrd() const;
    T norm() const;
    T norm_sqrd() const;

    vec2<T> normalized() const;
};

template <typename T>
vec2<T> operator+(const vec2<T>& u, const vec2<T>& v);

template <typename T>
vec2<T> operator-(const vec2<T>& u, const vec2<T>& v);

template <typename T, typename U>
vec2<T> operator*(T c, const vec2<T>& v);

template <typename T, typename U>
vec2<T> operator*(const vec2<T>& v, T c);

template <typename T, typename U>
vec2<T> operator/(const vec2<T>& v, U c);

template <typename T>
vec2<T> operator-(const vec2<T>& v);

template <typename T>
T length(const vec2<T>& v);

template <typename T>
T length_sqrd(const vec2<T>& v);

template <typename T>
T dot(const vec2<T>& u, const vec2<T>& v);

template <typename T>
std::ostream& operator<<(std::ostream& o, const vec2<T>& v);

typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<long double> vec2ld;

} // namespace au

#include "detail/vec2.h"

#endif
