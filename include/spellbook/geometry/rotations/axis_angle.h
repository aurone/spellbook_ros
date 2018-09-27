#ifndef au_axis_angle_h
#define au_axis_angle_h

#include <spellbook/geometry/vector/vec3.h>

namespace au {

template <typename T>
class quaternion;

template <typename T>
class axis_angle
{
public:
    
    vec3<T> axis;
    T angle;

    axis_angle();
    axis_angle(T angle, T x, T y, T z);
    axis_angle(T angle, const vec3<T>& axis);
    axis_angle(const quaternion<T>& q);
};

template <typename T>
std::ostream& operator<<(std::ostream& o, const axis_angle<T>& aa);

typedef axis_angle<float> axis_anglef;
typedef axis_angle<double> axis_angled;
typedef axis_angle<long double> axis_angleld;

} // namespace au

#include "detail/axis_angle.h"

#endif
