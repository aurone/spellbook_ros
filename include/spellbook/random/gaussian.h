#ifndef au_gaussian_h
#define au_gaussian_h

// standard includes
#include <math.h>

// project includes
#include <spellbook/matrix/matrix.h>
#include <spellbook/math/constants.h>
#include <spellbook/utils/utils.h>

namespace au {

inline double gaussian(double x, double mean, double sigma)
{
    return ((1.0 / sigma) * constants<double>::sqrt_two_pi()) * exp(-0.5 * sqrd((1.0 / sigma) * (x - mean)));
}

template <int N>
class gaussian_distribution { };

template <>
class gaussian_distribution<1>
{
public:

    gaussian_distribution(double mean, double sigma) :
        m_mean(mean),
        m_sigma(sigma)
    {
    }

    const double pdf(double x) const
    {
        return gaussian(x, m_mean, m_sigma);
    }

    const double operator()(double x) const
    {
        return gaussian(x, m_mean, m_sigma);
    }

private:

    double m_mean;
    double m_sigma;
};

template <>
class gaussian_distribution<2>
{
public:

    gaussian_distribution(const vector2d& mean, const matrix2d& cov) :
        m_mean(mean), m_cov(cov) { }

    const double pdf(const vector2d& x) const
    {
        const double k = 2.0; // dimensionality
        return (1.0 / sqrt(pow((2.0 * constants<double>::pi()), k)) * sqrt(determinant(m_cov))) * exp(-0.5 * ((transpose(x - m_mean) * inverse(m_cov) * (x - m_mean))(0, 0)));
    }

    const double operator()(const vector2d& x) const
    {
        return pdf(x);
    }

private:

    vector2d m_mean;
    matrix2d m_cov;
};

} // namespace au

#endif
