#ifndef au_constants_h
#define au_constants_h

namespace au {

template <typename T>
class constants
{
public:
};

template <>
class constants<double>
{
public:

    static constexpr double pi() { return 3.141592653589793238462643383275028841; }
    static constexpr double sixth_pi() { return pi() / 6.0; }
    static constexpr double half_pi() { return pi() / 2.0; }
    static constexpr double quarter_pi() { return pi() / 4.0; }
    static constexpr double two_pi() { return 2.0 * pi(); }
    static constexpr double sqrt_two_pi() { return sqrt(two_pi()); }

    static constexpr double e() { return 2.71828182845904523536028747135266249775724709369995; }
};

template <>
class constants<float>
{
public:

    static constexpr float pi() { return 3.141592653589793238462643383275028841F; }
    static constexpr float sixth_pi() { return pi() / 6.0f; }
    static constexpr float half_pi() { return pi() / 2.0f; }
    static constexpr float quarter_pi() { return pi() / 4.0f; }
    static constexpr float two_pi() { return 2.0f * pi(); }
    static constexpr float sqrt_two_pi() { return sqrt(two_pi()); }

    static constexpr float e() { return 2.71828182845904523536028747135266249775724709369995F; }
};

template <>
class constants<long double>
{
public:

    static constexpr long double pi() { return 3.141592653589793238462643383275028841L; }
    static constexpr long double sixth_pi() { return pi() / 6.0L; }
    static constexpr long double half_pi() { return pi() / 2.0L; }
    static constexpr long double quarter_pi() { return pi() / 4.0L; }
    static constexpr long double two_pi() { return 2.0L * pi(); }
    static constexpr long double sqrt_two_pi() { return sqrt(two_pi()); }

    static constexpr long double e() { return 2.71828182845904523536028747135266249775724709369995L; }
};

} // namespace au

#endif
