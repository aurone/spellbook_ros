#ifndef au_detail_axis_angle_h
#define au_detail_axis_angle_h

#include <spellbook/geometry/rotations/quaternion.h>

namespace au {

template <typename T>
axis_angle<T>::axis_angle() :
    axis(),
    angle()
{
}

template <typename T>
axis_angle<T>::axis_angle(T angle, T x, T y, T z) :
    angle(angle),
    axis(x, y, z)
{
}

template <typename T>
axis_angle<T>::axis_angle(T angle, const vec3<T>& axis) :
    angle(angle),
    axis(axis)
{
}

template <typename T>
axis_angle<T>::axis_angle(const quaternion<T>& q) :
    angle(),
    axis()
{
    const T ql = sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
    const T qli = 1.0 / ql;

    angle = (T)2 * atan2(ql, q.w);
    axis.x = qli * q.x;
    axis.y = qli * q.y;
    axis.z = qli * q.z;
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const axis_angle<T>& aa)
{
    o << "{ angle: " << aa.angle << ", axis: " << aa.axis << " }";
    return o;
}

} // namespace au

#endif
