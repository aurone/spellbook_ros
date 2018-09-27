#ifndef au_math_h
#define au_math_h

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841
#endif

namespace au {

template <typename T>
T sqrd(T a)
{
    return a * a;
}

template <typename T, typename U>
T interp(T a, T b, U alpha)
{
    return ((U)1 - alpha) * a + alpha * b;
}

} // namespace au

#endif
