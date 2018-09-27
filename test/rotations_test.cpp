#include <math.h>

#define BOOST_TEST_MODULE RotationsTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <spellbook/geometry/rotations/axis_angle.h>
#include <spellbook/geometry/rotations/rotmat.h>
#include <spellbook/geometry/vector/vec3.h>
#include <spellbook/math/constants.h>

#define BOOST_CHECK_VEC3D_ALMOST_EQUALITY(u, v)\
{\
    BOOST_CHECK(fabs((u).x - (v).x) < 1e-6);\
    BOOST_CHECK(fabs((u).y - (v).y) < 1e-6);\
    BOOST_CHECK(fabs((u).z - (v).z) < 1e-6);\
}

BOOST_AUTO_TEST_CASE(VectorConstructionTest)
{
    au::vec3d x(1.0, 0.0, 0.0);
    au::vec3d y(0.0, 1.0, 0.0);
    au::vec3d z(0.0, 0.0, 1.0);

    au::rotmatd R(x, y, z);

    BOOST_CHECK_EQUAL(x, au::vec3d(1.0, 0.0, 0.0));
    BOOST_CHECK_EQUAL(y, au::vec3d(0.0, 1.0, 0.0));
    BOOST_CHECK_EQUAL(z, au::vec3d(0.0, 0.0, 1.0));

    au::vec3d v(1.0, 0.0, 0.0);
    BOOST_CHECK_VEC3D_ALMOST_EQUALITY(R * v, v);
}

BOOST_AUTO_TEST_CASE(AxisAngleTest)
{
    au::axis_angled aa(0.5 * au::constants<double>::pi(), au::vec3d(0.0, 0.0, 1.0));
    au::rotmatd R(aa);
    au::vec3d v(1.0, 0.0, 0.0);

    au::vec3d u = R * v;
    BOOST_CHECK_VEC3D_ALMOST_EQUALITY(u, au::vec3d(0.0, 1.0, 0.0));
}
