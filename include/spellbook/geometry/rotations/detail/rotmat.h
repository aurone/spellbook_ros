#ifndef au_detail_rotmat_h
#define au_detail_rotmat_h

#include <cmath>

#include <spellbook/geometry/rotations/axis_angle.h>
#include <spellbook/geometry/rotations/euler_angles.h>
#include <spellbook/geometry/rotations/quaternion.h>

namespace au {

template <typename T>
rotmat<T>::rotmat() :
    m_mat()
{
}

template <typename T>
rotmat<T>::rotmat(const vec3<T>& _u1, const vec3<T>& _u2, const vec3<T>& _u3) :
    m_mat()
{
    m_mat[0] = _u1.x;
    m_mat[1] = _u1.y;
    m_mat[2] = _u1.z;
    m_mat[3] = _u2.x;
    m_mat[4] = _u2.y;
    m_mat[5] = _u2.z;
    m_mat[6] = _u3.x;
    m_mat[7] = _u3.y;
    m_mat[8] = _u3.z;
}

template <typename T>
rotmat<T>::rotmat(
    T a11, T a12, T a13,
    T a21, T a22, T a23,
    T a31, T a32, T a33)
:
    m_mat()
{
    m_mat[0] = a11;
    m_mat[1] = a21;
    m_mat[2] = a31;
    m_mat[3] = a12;
    m_mat[4] = a22;
    m_mat[5] = a32;
    m_mat[6] = a13;
    m_mat[7] = a23;
    m_mat[8] = a33;
}

template <typename T>
rotmat<T>::rotmat(const axis_angle<T>& aa) :
    m_mat()
{
    const T& n1 = aa.axis.x;
    const T& n2 = aa.axis.y;
    const T& n3 = aa.axis.z;
    const T& th = aa.angle;

    const T cth = cos(th);
    const T sth = sin(th);
    const T n11 = n1*n1;
    const T n12 = n1*n2;
    const T n13 = n1*n3;
    const T n22 = n2*n2;
    const T n23 = n2*n3;
    const T n33 = n3*n3;

    (*this)(0, 0) = n11             + (1.0 - n11)*cth;
    (*this)(1, 0) = n12*(1.0 - cth) + n3*sth;
    (*this)(2, 0) = n13*(1.0 - cth) - n2*sth;

    (*this)(0, 1) = n12*(1.0 - cth) - n3*sth;
    (*this)(1, 1) = n22             + (1.0 - n22)*cth;
    (*this)(2, 1) = n23*(1.0 - cth) + n1*sth;

    (*this)(0, 2) = n13*(1.0 - cth) + n2*sth;
    (*this)(1, 2) = n23*(1.0 - cth) - n1*sth;
    (*this)(2, 2) = n33             + (1 - n33)*cth;
}

template <typename T>
rotmat<T>::rotmat(const quaternion<T>& q) :
    m_mat()
{
    const T& q0 = q.w;
    const T& q1 = q.x;
    const T& q2 = q.y;
    const T& q3 = q.z;

    (*this)(0, 0) = q0*q0 + q1*q1 - q2*q2 - q3*q3;
    (*this)(1, 0) = 2.0*(q0*q3 + q1*q2);
    (*this)(2, 0) = 2.0*(q1*q3 - q0*q2);

    (*this)(0, 1) = 2.0*(q1*q2 - q0*q3);
    (*this)(1, 1) = q0*q0 - q1*q1 + q2*q2 - q3*q3;
    (*this)(2, 1) = 2.0*(q2*q3 + q0*q1);

    (*this)(0, 2) = 2.0*(q1*q3 + q0*q2);
    (*this)(1, 2) = 2.0*(q2*q3 - q0*q1);
    (*this)(2, 2) = q0*q0 - q1*q1 - q2*q2 + q3*q3;
}

template <typename T>
rotmat<T>::rotmat(const xzx_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c2;
    m_mat(0, 1) = -c3 * s2;
    m_mat(0, 2) = s2 * s3;

    m_mat(1, 0) = c1 * s2;
    m_mat(1, 1) = c1 * c2 * c3 - s1 * s3;
    m_mat(1, 2) = -c3 * s1 - c1 * c2 * s3;

    m_mat(2, 0) = s1 * s2;
    m_mat(2, 1) = c1 * s3 + c2 * c3 * s1;
    m_mat(2, 2) = c1 * c3 - c2 * s1 * s3;
}

template <typename T>
rotmat<T>::rotmat(const xyx_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c2;
    m_mat(0, 1) = s2 * s3;
    m_mat(0, 2) = c3 * s2;

    m_mat(1, 0) = s1 * s2;
    m_mat(1, 1) = c1 * c3 - c2 * s1 * s3;
    m_mat(1, 2) = -c1 * s3 - c2 * c3 * s1;

    m_mat(2, 0) = -c1 * s2;
    m_mat(2, 1) = c3 * s1 + c1 * c2 * s3;
    m_mat(2, 2) = c1 * c2 * c3 - s1 * s3;
}

template <typename T>
rotmat<T>::rotmat(const yxy_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c3 - c2 * s1 * s3;
    m_mat(0, 1) = s1 * s2;
    m_mat(0, 2) = c1 * s3 + c2 * c3 * s1;

    m_mat(1, 0) = s2 * s3;
    m_mat(1, 1) = c2;
    m_mat(1, 2) = -c3 * s2;

    m_mat(2, 0) = -c3 * s1 - c1 * c2 * s3;
    m_mat(2, 1) = c1 * s2;
    m_mat(2, 2) = c1 * c2 * c3 - s1 * s3;
}

template <typename T>
rotmat<T>::rotmat(const yzy_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c2 * c3 - s1 * s3;
    m_mat(0, 1) = -c1 * s2;
    m_mat(0, 2) = c3 * s1 + c1 * c2 * s3;

    m_mat(1, 0) = c3 * s2;
    m_mat(1, 1) = c2;
    m_mat(1, 2) = s2 * s3;

    m_mat(2, 0) = -c1 * s3 - c2 * c3 * s1;
    m_mat(2, 1) = s1 * s2;
    m_mat(2, 2) = c1 * c3 - c2 * s1 * s3;
}

template <typename T>
rotmat<T>::rotmat(const zyz_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c2 * c3 - s1 * s3;
    m_mat(0, 1) = -c3 * s1 - c1 * c2 * s3;
    m_mat(0, 2) = c1 * s2;

    m_mat(1, 0) = c1 * s3 + c2 * c3 * s1;
    m_mat(1, 1) = c1 * c3 - c2 * s1 * s3;
    m_mat(1, 2) = s1 * s2;

    m_mat(2, 0) = -c3 * s2;
    m_mat(2, 1) = s2 * s3;
    m_mat(2, 2) = c2;
}

template <typename T>
rotmat<T>::rotmat(const zxz_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c3 - c2 * s1 * s3;
    m_mat(0, 1) = -c3 * s1 - c1 * c2 * s3;
    m_mat(0, 2) = s2 * s3;

    m_mat(1, 0) = c2 * c3 * s1 + c1 * s3;
    m_mat(1, 1) = c1 * c2 * c3 - s1 * s3;
    m_mat(1, 2) = -c3 * s2;

    m_mat(2, 0) = s1 * s2;
    m_mat(2, 1) = c1 * s2;
    m_mat(2, 2) = c2;
}

template <typename T>
rotmat<T>::rotmat(const xzy_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c2 * c3;
    m_mat(0, 1) = -s2;
    m_mat(0, 2) = c2 * s3;

    m_mat(1, 0) = s1 * s3 + c1 * c3 * s2;
    m_mat(1, 1) = c1 * c2;
    m_mat(1, 2) = c1 * s2 * s3 - c3 * s1;

    m_mat(2, 0) = c3 * s1 * s2 - c1 * s3;
    m_mat(2, 1) = c2 * s1;
    m_mat(2, 2) = c1 * c3 + s1 * s2 * s3;
}

template <typename T>
rotmat<T>::rotmat(const xyz_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c2 * c3;
    m_mat(0, 1) = -c2 * s3;
    m_mat(0, 2) = s2;

    m_mat(1, 0) = c1 * s3 + c3 * s1 * s2;
    m_mat(1, 1) = c1 * c3 - s1 * s2 * s3;
    m_mat(1, 2) = -c2 * s1;

    m_mat(2, 0) = s1 * s3 - c1 *c3 * s2;
    m_mat(2, 1) = c3 * s1 + c1 * s2 * s3;
    m_mat(2, 2) = c1 *c2;
}

template <typename T>
rotmat<T>::rotmat(const yxz_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c3 + s1 * s2 * s3;
    m_mat(0, 1) = c3 * s1 * s2 - c1 * s3;
    m_mat(0, 2) = c2 * s1;

    m_mat(1, 0) = c2 * s3;
    m_mat(1, 1) = c2 * c3;
    m_mat(1, 2) = -s2;

    m_mat(2, 0) = c1 * s2 * s3 - c3 * s1;
    m_mat(2, 1) = c1 * c3 * s2 + s1 * s3;
    m_mat(2, 2) = c1 * c2;
}

template <typename T>
rotmat<T>::rotmat(const yzx_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c2;
    m_mat(0, 1) = s1 * s3 - c1 * c3 * s2;
    m_mat(0, 2) = c3 * s1 + c1 * s2 * s3;

    m_mat(1, 0) = s2;
    m_mat(1, 1) = c2 * c3;
    m_mat(1, 2) = -c2 * s3;

    m_mat(2, 0) = -c2 * s1;
    m_mat(2, 1) = c1 * s3 + c3 * s1 * s2;
    m_mat(2, 2) = c1 * c3 - s1 * s2 * s3;
}

template <typename T>
rotmat<T>::rotmat(const zyx_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c2;
    m_mat(0, 1) = c1 * s2 * s3 - c3 * s1;
    m_mat(0, 2) = s1 * s3 + c1 * c3 * s2;

    m_mat(1, 0) = c2 * s1;
    m_mat(1, 1) = c1 * c3 + s1 * s2 * s3;
    m_mat(1, 2) = c3 * s1 * s2 - c1 * s3;

    m_mat(2, 0) = -s2;
    m_mat(2, 1) = c2 * s3;
    m_mat(2, 2) = c2 * c3;
}

template <typename T>
rotmat<T>::rotmat(const zxy_angles<T>& ea) :
    m_mat()
{
    const T c1 = std::cos(ea.alpha);
    const T c2 = std::cos(ea.beta);
    const T c3 = std::cos(ea.gamma);
    const T s1 = std::sin(ea.alpha);
    const T s2 = std::sin(ea.beta);
    const T s3 = std::sin(ea.gamma);

    m_mat(0, 0) = c1 * c3 - s1 * s2 * s3;
    m_mat(0, 1) = -c2 * s1;
    m_mat(0, 2) = c1 * s3 + c3 * s1 * s2;

    m_mat(1, 0) = c3 * s1 + c1 * s2 * s3;
    m_mat(1, 1) = c1 * c2;
    m_mat(1, 2) = s1 * s3 - c1 * c3 * s2;

    m_mat(2, 0) = -c2 * s3;
    m_mat(2, 1) = s2;
    m_mat(2, 2) = c2 * c3;
}

template <typename T>
T& rotmat<T>::operator()(matrix_index i, matrix_index j)
{
    return m_mat(j, i);
}

template <typename T>
const T& rotmat<T>::operator()(matrix_index i, matrix_index j) const
{
    return m_mat(j, i);
}

template <typename T>
const T* rotmat<T>::data() const
{
    return m_mat.data();
}

template <typename T>
T* rotmat<T>::data()
{
    return m_mat.data();
}

template <typename T>
const vec3<T>& rotmat<T>::u1() const
{
    return reinterpret_cast<const vec3<T>&>(m_mat.data()[0]);
}

template <typename T>
const vec3<T>& rotmat<T>::u2() const
{
    return reinterpret_cast<const vec3<T>&>(m_mat.data()[3]);
}

template <typename T>
const vec3<T>& rotmat<T>::u3() const
{
    return reinterpret_cast<const vec3<T>&>(m_mat.data()[6]);
}

template <typename T>
void rotmat<T>::normalize()
{
    double l1 = u1.length();
    double l2 = u2.length();
    double l3 = u2.length();

    (*this)(0, 0) /= l1;
    (*this)(1, 0) /= l1;
    (*this)(2, 0) /= l1;
    (*this)(0, 1) /= l2;
    (*this)(1, 1) /= l2;
    (*this)(2, 1) /= l2;
    (*this)(0, 2) /= l3;
    (*this)(1, 2) /= l3;
    (*this)(2, 2) /= l3;
}

template <typename T>
rotmat<T> operator*(const rotmat<T>& a, const rotmat<T>& b)
{
    rotmat<T> r;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            r(i, j) = (T)0;
            for (int k = 0; k < 3; ++k) {
                r(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return r;
}

template <typename T>
rotmat<T> transpose(const rotmat<T>& r)
{
    rotmat<T> out;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            out(i, j) = r(j, i);
        }
    }
    return out;
}

template <typename T>
rotmat<T> inverse(const rotmat<T>& r)
{
    return transpose(r);
}

template <typename T>
vec3<T> operator*(const rotmat<T>& R, const vec3<T>& v)
{
    vec3<T> vo;
    vo.x = R(0, 0) * v.x + R(0, 1) * v.y + R(0, 2) * v.z;
    vo.y = R(1, 0) * v.x + R(1, 1) * v.y + R(1, 2) * v.z;
    vo.z = R(2, 0) * v.x + R(2, 1) * v.y + R(2, 2) * v.z;
    return vo;
}

template <typename T>
std::ostream& operator<<(std::ostream& o, const rotmat<T>& r)
{
    o << "{ u1: " << r.u1() << ", u2: " << r.u2() << ", u3: " << r.u3() << " }";
    return o;
}

} // namespace au

#endif
