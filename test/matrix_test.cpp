#include <array>

#define BOOST_TEST_MODULE MatrixTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <spellbook/matrix/matrix.h>

#define BOOST_CHECK_MATRIX_VALUES(m, v11, v12, v13, v21, v22, v23, v31, v32, v33) \
{\
    BOOST_CHECK_EQUAL(m(0, 0), v11);\
    BOOST_CHECK_EQUAL(m(0, 1), v12);\
    BOOST_CHECK_EQUAL(m(0, 2), v13);\
    BOOST_CHECK_EQUAL(m(1, 0), v21);\
    BOOST_CHECK_EQUAL(m(1, 1), v22);\
    BOOST_CHECK_EQUAL(m(1, 2), v23);\
    BOOST_CHECK_EQUAL(m(2, 0), v31);\
    BOOST_CHECK_EQUAL(m(2, 1), v32);\
    BOOST_CHECK_EQUAL(m(2, 2), v33);\
}

#define BOOST_CHECK_MATRIX_EQUALITY(a, b)\
{\
    BOOST_CHECK_EQUAL(a(0, 0), b(0, 0));\
    BOOST_CHECK_EQUAL(a(0, 1), b(0, 1));\
    BOOST_CHECK_EQUAL(a(0, 2), b(0, 2));\
    BOOST_CHECK_EQUAL(a(1, 0), b(1, 0));\
    BOOST_CHECK_EQUAL(a(1, 1), b(1, 1));\
    BOOST_CHECK_EQUAL(a(1, 2), b(1, 2));\
    BOOST_CHECK_EQUAL(a(2, 0), b(2, 0));\
    BOOST_CHECK_EQUAL(a(2, 1), b(2, 1));\
    BOOST_CHECK_EQUAL(a(2, 2), b(2, 2));\
}

#define BOOST_CHECK_MATRIX_ALMOST_EQUALITY(a, b)\
{\
    BOOST_CHECK(fabs(a(0, 0) - b(0, 0)) < 1e-6);\
    BOOST_CHECK(fabs(a(0, 1) - b(0, 1)) < 1e-6);\
    BOOST_CHECK(fabs(a(0, 2) - b(0, 2)) < 1e-6);\
    BOOST_CHECK(fabs(a(1, 0) - b(1, 0)) < 1e-6);\
    BOOST_CHECK(fabs(a(1, 1) - b(1, 1)) < 1e-6);\
    BOOST_CHECK(fabs(a(1, 2) - b(1, 2)) < 1e-6);\
    BOOST_CHECK(fabs(a(2, 0) - b(2, 0)) < 1e-6);\
    BOOST_CHECK(fabs(a(2, 1) - b(2, 1)) < 1e-6);\
    BOOST_CHECK(fabs(a(2, 2) - b(2, 2)) < 1e-6);\
}

BOOST_AUTO_TEST_CASE(MatrixConstructionTest)
{
    // not a whole llot to test here, but this should force instantiation
    au::matrix3d A;
    BOOST_CHECK(A.data() != nullptr);

    au::matrix3d B = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
    BOOST_CHECK_MATRIX_VALUES(B, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);

    double C_d[9] = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
    au::matrix3d C(C_d, C_d + 9);
    BOOST_CHECK_MATRIX_VALUES(C, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0);
}

BOOST_AUTO_TEST_CASE(DefaultCopyConstructionTest)
{
    au::matrix3d A = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
    au::matrix3d B(A);
    BOOST_CHECK_MATRIX_EQUALITY(A, B);
}

BOOST_AUTO_TEST_CASE(DefaultCopyAssignmentTest)
{
    au::matrix3d A = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0 };
    au::matrix3d B;
    B = A;
    BOOST_CHECK_MATRIX_EQUALITY(A, B);

    au::matrix3d C;
    C = B = A;
    BOOST_CHECK_MATRIX_EQUALITY(A, C);
}

BOOST_AUTO_TEST_CASE(IdentityMatrixTest)
{
    au::matrix3d A = au::matrix3d::identity();
    BOOST_CHECK_MATRIX_VALUES(A, 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
}

BOOST_AUTO_TEST_CASE(ZeroMatrixTest)
{
    au::matrix3d A = au::matrix3d::zeros();
    BOOST_CHECK_MATRIX_VALUES(A, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

BOOST_AUTO_TEST_CASE(OneMatrixTest)
{
    au::matrix3d A = au::matrix3d::ones();
    BOOST_CHECK_MATRIX_VALUES(A, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0);
}

BOOST_AUTO_TEST_CASE(AdditionAssignmentTest)
{
    au::matrix3d A = au::matrix3d::identity();
    A += 3.0;
    BOOST_CHECK_MATRIX_VALUES(A, 4.0, 3.0, 3.0, 3.0, 4.0, 3.0, 3.0, 3.0, 4.0);
}

BOOST_AUTO_TEST_CASE(MatrixAdditionAssignmentTest)
{
    au::matrix3d A = au::matrix3d::identity();
    au::matrix3d B = au::matrix3d::identity();
    A += B;
    BOOST_CHECK_MATRIX_VALUES(A, 2.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 2.0);

    // check self assignments
    A += A;
    BOOST_CHECK_MATRIX_VALUES(A, 4.0, 0.0, 0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 4.0);
}

BOOST_AUTO_TEST_CASE(SubtractionAssignmentTest)
{
    au::matrix3d A = au::matrix3d::identity();
    A -= 3.0;
    BOOST_CHECK_MATRIX_VALUES(A, -2.0, -3.0, -3.0, -3.0, -2.0, -3.0, -3.0, -3.0, -2.0);
}

BOOST_AUTO_TEST_CASE(MatrixSubstractionAssignmentTest)
{
    au::matrix3d A = au::matrix3d::identity();
    au::matrix3d B = au::matrix3d::identity();
    A -= B;
    BOOST_CHECK_MATRIX_EQUALITY(A, au::matrix3d::zeros());

    // check self assignment
    A -= A;
    BOOST_CHECK_MATRIX_EQUALITY(A, au::matrix3d::zeros());
}

BOOST_AUTO_TEST_CASE(MultiplicationAssignmentTest)
{
    au::matrix3d A = au::matrix3d::identity();
    A *= 3.0;
    BOOST_CHECK_MATRIX_VALUES(A, 3.0, 0.0, 0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 3.0);
}

BOOST_AUTO_TEST_CASE(MatrixMultiplicationAssignmentTest)
{
    au::matrix3d A = au::matrix3d::ones();
    au::matrix3d B = au::matrix3d::zeros();
    A *= B;
    BOOST_CHECK_MATRIX_EQUALITY(A, au::matrix3d::zeros());

    // check self assignment
    // note: could use scalar multiplication here, but haven't tested that yet
    A = au::matrix3d::ones();
    for (au::matrix_index i = 0; i < 9; ++i) {
        A[i] = 2.0;
    }

    A *= A;
    BOOST_CHECK_MATRIX_VALUES(A, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0, 12.0);
}

BOOST_AUTO_TEST_CASE(MatrixDivisionAssignmentTest)
{
    au::matrix3d A = au::matrix3d::ones();
    A /= 3.0;
    double e = 1.0 / 3.0;
    BOOST_CHECK_MATRIX_VALUES(A, e, e, e, e, e, e, e, e, e);
}

BOOST_AUTO_TEST_CASE(UnaryPlusTest)
{
    au::matrix3d A = au::matrix3d::identity();
    A = +A;
    BOOST_CHECK_MATRIX_EQUALITY(A, au::matrix3d::identity());
}

BOOST_AUTO_TEST_CASE(UnaryMinusTest)
{
    au::matrix3d A = au::matrix3d::identity();
    A = -A;
    BOOST_CHECK_MATRIX_VALUES(A, -1.0, 0.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0, -1.0);
}

BOOST_AUTO_TEST_CASE(AdditionTest)
{
    au::matrix3d A = au::matrix3d::identity();
    au::matrix3d B = au::matrix3d::identity();
    au::matrix3d C = A + B;
    BOOST_CHECK_MATRIX_VALUES(C, 2.0, 0.0, 0.0, 0.0, 2.0, 0.0, 0.0, 0.0, 2.0);
}

BOOST_AUTO_TEST_CASE(SubtractionTest)
{
    au::matrix3d A = au::matrix3d::identity();
    au::matrix3d B = au::matrix3d::identity();
    au::matrix3d C = A - B;
    BOOST_CHECK_MATRIX_EQUALITY(C, au::matrix3d::zeros());
}

BOOST_AUTO_TEST_CASE(MultiplicationTest)
{
    // TODO: this test and later
    au::matrix<double, 3, 2> A = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    au::matrix<double, 2, 3> B = { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 };
    au::matrix3d C = A * B;
    BOOST_CHECK_MATRIX_VALUES(C, 9.0, 12.0, 15.0, 19.0, 26.0, 33.0, 29.0, 40.0, 51.0);
}

BOOST_AUTO_TEST_CASE(TransposeTest)
{
}

BOOST_AUTO_TEST_CASE(InverseTest)
{
    au::matrix4d A = {
        1.0, -2.0, 0.0, 2.0,
        4.0, 1.0, -1.0, -1.0,
        -8.0, -1.0, 2.0, 1.0,
        -4.0, -1.0, 1.0, 2.0
    };

    au::matrix4d Ainv = au::inverse(A);

    au::matrix4d I = Ainv * A;
    BOOST_CHECK_MATRIX_ALMOST_EQUALITY(I, au::matrix4d::identity());

    I = A * Ainv;
    BOOST_CHECK_MATRIX_ALMOST_EQUALITY(I, au::matrix4d::identity());
}

BOOST_AUTO_TEST_CASE(LinsolveTest)
{
    au::matrix3d A = { 2, 6, 1, 0, 2, 1, 1, 1, 4 };
    au::vector3d b = { 7, 2, 3 };
    au::vector3d x = au::linsolve(A, b);
    BOOST_CHECK(fabs(x[0] - (5.0 / 6.0)) < 1e-6);
    BOOST_CHECK(fabs(x[1] - (5.0 / 6.0)) < 1e-6);
    BOOST_CHECK(fabs(x[2] - (1.0 / 3.0)) < 1e-6);
}
