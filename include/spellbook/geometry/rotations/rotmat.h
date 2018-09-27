#ifndef au_rotmat_h
#define au_rotmat_h

#include <ostream>

#include <spellbook/matrix/matrix.h>
#include <spellbook/geometry/vector/vec3.h>
#include <spellbook/geometry/rotations/euler_angles.h>

namespace au {

template <typename T>
class axis_angle;

template <typename T>
class quaternion;

template <typename T, int AngleConvention>
class euler_angles;

template <typename T>
class rotmat
{
public:

    rotmat();
    rotmat(const vec3<T>& _u1, const vec3<T>& _u2, const vec3<T>& _u3);
    rotmat(T a11, T a12, T a13, T a21, T a22, T a23, T a31, T a32, T a33);
    rotmat(const axis_angle<T>& aa);
    rotmat(const quaternion<T>& q);

    rotmat(const xzx_angles<T>& ea);
    rotmat(const xyx_angles<T>& ea);
    rotmat(const yxy_angles<T>& ea);
    rotmat(const yzy_angles<T>& ea);
    rotmat(const zyz_angles<T>& ea);
    rotmat(const zxz_angles<T>& ea);

    rotmat(const xzy_angles<T>& ea);
    rotmat(const xyz_angles<T>& ea);
    rotmat(const yxz_angles<T>& ea);
    rotmat(const yzx_angles<T>& ea);
    rotmat(const zyx_angles<T>& ea);
    rotmat(const zxy_angles<T>& ea);

    T& operator()(matrix_index i, matrix_index j);
    const T& operator()(matrix_index i, matrix_index j) const;

    const T* data() const;
    T* data();

    const vec3<T>& u1() const;
    const vec3<T>& u2() const;
    const vec3<T>& u3() const;

    void normalize();

private:

    matrix<T, 3, 3> m_mat;
};

template <typename T>
rotmat<T> operator*(const rotmat<T>& a, const rotmat<T>& b);

template <typename T>
rotmat<T> transpose(const rotmat<T>& r);

template <typename T>
rotmat<T> inverse(const rotmat<T>& r);

template <typename T>
vec3<T> operator*(const rotmat<T>& R, const vec3<T>& v);

template <typename T>
std::ostream& operator<<(std::ostream& o, const rotmat<T>& r);

typedef rotmat<float> rotmatf;
typedef rotmat<double> rotmatd;
typedef rotmat<long double> rotmatld;

} // namespace au

#include "detail/rotmat.h"

#endif
