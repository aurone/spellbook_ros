#ifndef au_euler_angles_h
#define au_euler_angles_h

#include <ostream>

namespace au {

template <typename T>
class rotmat;

namespace angle_convention {
enum
{
    XYX = 0,
    XZX,
    YXY,
    YZY,
    ZXZ,
    ZYZ,
    XYZ,
    XZY,
    YXZ,
    YZX,
    ZXY,
    ZYX
};
} // namespace angle_convention

template <typename T, int AngleConvention>
class euler_angles
{
public:
    
    T alpha, beta, gamma;

    euler_angles() : alpha(), beta(), gamma() { }

    euler_angles(T alpha, T beta, T gamma) :
        alpha(alpha), beta(beta), gamma(gamma)
    { }

    euler_angles(const rotmat<T>& R);
};

template <typename T>
using xyx_angles = euler_angles<T, angle_convention::XYX>;
template <typename T>
using xzx_angles = euler_angles<T, angle_convention::XZX>;
template <typename T>
using yxy_angles = euler_angles<T, angle_convention::YXY>;
template <typename T>
using yzy_angles = euler_angles<T, angle_convention::YZY>;
template <typename T>
using zxz_angles = euler_angles<T, angle_convention::ZXZ>;
template <typename T>
using zyz_angles = euler_angles<T, angle_convention::ZYZ>;
template <typename T>
using xyz_angles = euler_angles<T, angle_convention::XYZ>;
template <typename T>
using xzy_angles = euler_angles<T, angle_convention::XZY>;
template <typename T>
using yxz_angles = euler_angles<T, angle_convention::YXZ>;
template <typename T>
using yzx_angles = euler_angles<T, angle_convention::YZX>;
template <typename T>
using zxy_angles = euler_angles<T, angle_convention::ZXY>;
template <typename T>
using zyx_angles = euler_angles<T, angle_convention::ZYX>;

#define DECLARE_EA_TYPEDEFS(base) \
typedef base<float> base##f;\
typedef base<double> base##d;\
typedef base<long double> base##ld;

DECLARE_EA_TYPEDEFS(xyx_angles);
DECLARE_EA_TYPEDEFS(xzx_angles);
DECLARE_EA_TYPEDEFS(yxy_angles);
DECLARE_EA_TYPEDEFS(yzy_angles);
DECLARE_EA_TYPEDEFS(zxz_angles);
DECLARE_EA_TYPEDEFS(zyz_angles);
DECLARE_EA_TYPEDEFS(xyz_angles);
DECLARE_EA_TYPEDEFS(xzy_angles);
DECLARE_EA_TYPEDEFS(yxz_angles);
DECLARE_EA_TYPEDEFS(yzx_angles);
DECLARE_EA_TYPEDEFS(zxy_angles);
DECLARE_EA_TYPEDEFS(zyx_angles);

#undef DECLARE_EA_TYPEDEFS

template <typename T, int AngleConvention>
std::ostream& operator<<(std::ostream& o, const euler_angles<T, AngleConvention>& v);

} // namespace au

#include "detail/euler_angles.h"

#endif
