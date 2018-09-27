#ifndef au_pose2_h
#define au_pose2_h

#include <spellbook/geometry/vector/vec2.h>

namespace au {

template <typename T>
struct pose2
{
    vec2<T> pos;
    T yaw;

    pose2();
    pose2(T x, T y, T yaw);
    pose2(const vec2<T>& pos, T yaw);
};

template <typename T>
std::ostream& operator<<(std::ostream& o, const pose2<T>& p);

template <typename T>
pose2<T> interp(const pose2<T>& p, const pose2<T>& q, T t);

typedef pose2<float> pose2f;
typedef pose2<double> pose2d;
typedef pose2<long double> pose2ld;

} // namespace au

#include "detail/pose2.h"

#endif
