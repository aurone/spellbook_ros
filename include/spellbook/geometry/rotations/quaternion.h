#ifndef au_quaternion_h
#define au_quaternion_h

// standard includes
#include <ostream>

// project includes
#include <spellbook/geometry/vector/vec3.h>

namespace au {

template <typename T>
class axis_angle;

template <typename T>
class rotmat;

template <typename T>
class quaternion
{
public:

    T w, x, y, z;

    quaternion();
    quaternion(T w, T x, T y, T z);
    quaternion(const axis_angle<T>& aa);
    quaternion(const rotmat<T>& r);

    T length() const;
    T length_sqrd() const;
    void normalize();
};

template <typename T>
quaternion<T> operator+(const quaternion<T>& p, const quaternion<T>& q);

template <typename T, typename U>
quaternion<T> operator*(U w, const quaternion<T>& q);

template <typename T, typename U>
quaternion<T> operator*(const quaternion<T>& q, U w);

template <typename T, typename U>
quaternion<T> operator/(const quaternion<T>& q, U w);

template <typename T>
quaternion<T> operator*(const quaternion<T>& p, const quaternion<T>& q);

template <typename T>
quaternion<T> conj(const quaternion<T>& q);

template <typename T>
quaternion<T> inverse(const quaternion<T>& q);

template <typename T>
T length(const quaternion<T>& q);

template <typename T>
T length_sqrd(const quaternion<T>& q);

template <typename T>
std::ostream& operator<<(std::ostream& o, const quaternion<T>& q);

typedef quaternion<float> quaternionf;
typedef quaternion<double> quaterniond;
typedef quaternion<long double> quaternionld;

} // namespace au

#include "detail/quaternion.h"

#endif
