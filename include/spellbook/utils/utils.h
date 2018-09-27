#ifndef utils_h
#define utils_h

template <typename T>
const T& clamp(const T& t, const T& min, const T& max)
{
    return t > max ? max : t < min ? min : t;
}

inline double signf(double d, double eps = 0.0)
{
    if (d < eps) {
        return -1.0;
    }
    else if (d > eps) {
        return 1.0;
    }
    else {
        return 0.0;
    }
}

template <typename T>
constexpr T sqrd(T t)
{
    return t * t;
}

inline const char* boolstr(bool b)
{
    return b ? "TRUE" : "FALSE";
}

#endif

