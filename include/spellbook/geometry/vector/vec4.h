#ifndef au_vec4_h
#define au_vec4_h

#include <ostream>

namespace au {

template <typename T>
struct vec4
{
    T x, y, z, w;

    vec4() : w(), x(), y(), z() { }
    vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) { }
};

typedef vec4<float> vec4f;
typedef vec4<double> vec4d;
typedef vec4<long double> vec4ld;

template <typename T>
std::ostream& operator<<(std::ostream& o, const vec4<T>& v);

template <typename U, typename V>
vec4<decltype(U()*V())> operator*(const matrix4<U>& m, const vec4<V>& v);

} // namespace au

#include "detail/vec4.h"

#endif
