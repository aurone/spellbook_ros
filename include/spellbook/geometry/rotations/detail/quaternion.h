#ifndef au_detail_quaternion_h
#define au_detail_quaternion_h

// standard includes
#include <math.h>

// project includes
#include <spellbook/geometry/rotations/axis_angle.h>
#include <spellbook/geometry/rotations/rotmat.h>

namespace au {

template <typename T>
quaternion<T>::quaternion() :
    w(), x(), y(), z()
{
}

template <typename T>
quaternion<T>::quaternion(T w, T x, T y, T z) :
    w(w), x(x), y(y), z(z)
{
}

template <typename T>
quaternion<T>::quaternion(const axis_angle<T>& aa) :
    w(), x(), y(), z()
{
    w = cos(0.5 * aa.angle);
    vec3<T> v = sin(0.5 * aa.angle) * aa.axis;
    x = v.x;
    y = v.y;
    z = v.z;
}

template <typename T>
quaternion<T>::quaternion(const rotmat<T>& r)
{
    const T& r11 = r(0, 0);
    const T& r21 = r(1, 0);
    const T& r31 = r(2, 0);

    const T& r12 = r(0, 1);
    const T& r22 = r(1, 1);
    const T& r32 = r(2, 1);

    const T& r13 = r(0, 2);
    const T& r23 = r(1, 2);
    const T& r33 = r(2, 2);

    // gather all qi^2
    T qi2[4];
    qi2[0] = 0.25 * (1.0 + r11 + r22 + r33);
    qi2[1] = 0.25 * (1.0 + r11 - r22 - r33);
    qi2[2] = 0.25 * (1.0 - r11 + r22 - r33);
    qi2[3] = 0.25 * (1.0 - r11 - r22 + r33);

    // find the maximum qi^2
    int maxqi = 0;
    for (int i = 1; i < 4; ++i) {
        if (qi2[i] > qi2[maxqi]) {
            maxqi = i;
        }
    }

    // derive the remaining components
    if (maxqi == 0) {
        w = sqrt(qi2[0]);
        x = 0.25 * (r32 - r23) / w;
        y = 0.25 * (r13 - r31) / w;
        z = 0.25 * (r21 - r12) / w;
    }
    else if (maxqi == 1) {
        x = sqrt(qi2[1]);
        w = 0.25 * (r32 - r23) / x;
        y = 0.25 * (r12 + r21) / x;
        z = 0.25 * (r13 + r31) / x;
    }
    else if (maxqi == 2) {
        y = sqrt(qi2[2]);
        w = 0.25 * (r13 - r31) / y;
        x = 0.25 * (r12 + r21) / y;
        z = 0.25 * (r23 + r32) / y;
    }
    else {
        z = sqrt(qi2[3]);
        w = 0.25 * (r21 - r12) / z;
        x = 0.25 * (r13 + r31) / z;
        y = 0.25 * (r23 + r32) / z;
    }
}

template <typename T>
T quaternion<T>::length() const
{
    return sqrt(length_sqrd());
}

template <typename T>
T quaternion<T>::length_sqrd() const
{
    return w * w + x * x + y * y + z * z;
}

template <typename T>
void quaternion<T>::normalize()
{
    T l = length();
    w /= l;
    x /= l;
    y /= l;
    z /= l;
}

template <typename T>
quaternion<T> operator+(const quaternion<T>& p, const quaternion<T>& q)
{
    return quaternion<T>(p.w + q.w, p.x + q.x, p.y + q.y, p.z + q.z);
}

template <typename T, typename U>
quaternion<T> operator*(U w, const quaternion<T>& q)
{
    return quaternion<T>(w * q.w, w * q.x, w * q.y, w * q.z);
}

template <typename T, typename U>
quaternion<T> operator*(const quaternion<T>& q, U w)
{
    return quaternion<T>(q.w * w, q.x * w, q.y * w, q.z * w);
}

template <typename T, typename U>
quaternion<T> operator/(const quaternion<T>& q, U w)
{
    return quaternion<T>(q.w / w, q.x / w, q.y / w, q.z / w);
}

template <typename T>
quaternion<T> operator*(const quaternion<T>& p, const quaternion<T>& q)
{
    const vec3<T> u(p.x, p.y, p.z);
    const vec3<T> v(q.x, q.y, q.z);

    const T w = p.w * q.w - dot(u, v);
    const vec3<T> t = p.w * v + q.w * u + cross(u, v);

    return quaternion<T>(w, t.x, t.y, t.z);
}

template <typename T>
quaternion<T> conj(const quaternion<T>& q)
{
    return quaternion<T>(q.w, -q.x, -q.y, -q.z);
}

template <typename T>
quaternion<T> inverse(const quaternion<T>& q)
{
    return conj(q) / length_sqrd(q);
}

template <typename T>
T length(const quaternion<T>& q)
{
    return sqrt(length_sqrd(q));
}

template <typename T>
T length_sqrd(const quaternion<T>& q)
{
    return q.length_sqrd();
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const quaternion<T>& q)
{
    o << "(" << q.w << ", " << q.x << ", " << q.y << ", " << q.z << ")";
    return o;
}

} // namespace au

#endif
