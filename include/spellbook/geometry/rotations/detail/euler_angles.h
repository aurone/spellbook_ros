#ifndef au_detail_euler_angles_h
#define au_detail_euler_angles_h

#include <spellbook/geometry/rotations/rotmat.h>

namespace au {

template <typename T, int AngleConvention>
struct euler_angle_constructor
{
    euler_angle_constructor(
            euler_angles<T, AngleConvention>& ea,
            const rotmat<T>& R);
};

template <typename T>
struct euler_angle_constructor<T, angle_convention::ZYZ>
{
    euler_angle_constructor(zyz_angles<T>& ea, const rotmat<T>& R)
    {
        if (R(2, 2) == 1.0) {
            ea.alpha = 0.0;
            ea.beta  = 0.0;
            ea.gamma = atan2(R(0, 0), R(1, 0));
        }
        else if (R(2, 2) ==  -1.0) {
            ea.alpha = 0.0;
            ea.beta  = 0.0;
            ea.gamma = -atan2(-R(0, 0), -R(0, 1));
        }
        else {
            ea.alpha = atan2(R(1, 2), R(0, 2));
            ea.beta  = atan2(sqrt(sqrd(R(1, 3) + sqrd(R(1, 2)))), R(2, 2));
            ea.gamma = atan2(R(2, 1), -R(2, 0));
        }
    }
};

template <typename T>
struct euler_angle_constructor<T, angle_convention::XYZ>
{
    euler_angle_constructor(xyz_angles<T>& ea, const rotmat<T>& R)
    {
        // TODO: handle degenericies when cos(beta) = 0
        ea.alpha = atan2(R(1, 0), R(0, 0));
        ea.beta  = atan2(-R(2, 0), sqrt(sqrd(R(2, 1)) + sqrd(R(2, 2))));
        ea.gamma = atan2(R(2, 1), R(2, 2));
    }
};

template <typename T, int AngleConvention>
euler_angles<T, AngleConvention>::euler_angles(const rotmat<T>& R)
{
    euler_angle_constructor<T, AngleConvention>(*this, R);
}

template <typename T, int AngleConvention>
std::ostream& operator<<(
    std::ostream& o,
    const euler_angles<T, AngleConvention>& ea)
{
    o << "(" << ea.alpha << ", " << ea.beta << ", " << ea.gamma << ")";
    return o;
}

} // namespace au

#endif
