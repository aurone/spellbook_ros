#include <iostream>
#include <stdexcept>
#include <utility>

#define BOOST_TEST_MODULE GridTest
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <spellbook/grid/grid.h>

BOOST_AUTO_TEST_CASE(GridDefaultConstructorTest)
{
    au::grid<2, int> g;
    BOOST_CHECK_EQUAL(g.size(0), 0);
    BOOST_CHECK_EQUAL(g.size(1), 0);
    BOOST_CHECK_EQUAL(g.total_size(), 0);
    BOOST_CHECK(!g.data());
    BOOST_CHECK_EQUAL(g.begin(), g.end());
}

BOOST_AUTO_TEST_CASE(GridOneDimensionalTest)
{
    au::grid<1, int> g(8);
    BOOST_CHECK_EQUAL(g.size(0), 8);
    BOOST_CHECK_EQUAL(g.total_size(), 8);
    BOOST_CHECK(g.data());
}

BOOST_AUTO_TEST_CASE(GridSizeConstructorTest)
{
    au::grid<2, int> g(5, 5);

    BOOST_CHECK_EQUAL(g.size(0), 5);
    BOOST_CHECK_EQUAL(g.size(1), 5);
    BOOST_CHECK_EQUAL(g.total_size(), 25);
    BOOST_CHECK(g.data());
}

BOOST_AUTO_TEST_CASE(GridIndexAssignmentTest)
{
    au::grid<2, int> g(5, 5);

    const int gkern[] = {
        1, 4,   6,  4, 1,
        4, 16, 24, 16, 4,
        6, 24, 36, 24, 6,
        4, 16, 24, 16, 4,
        1, 4,   6,  4, 1,
    };

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            g(y, x) = gkern[y * 5 + x];
        }
    }

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            BOOST_CHECK_EQUAL(g(y, x), gkern[y * 5 + x]);
        }
    }
}

BOOST_AUTO_TEST_CASE(GridMoveConstructorTest)
{
    // create and initialize g1 with known values
    au::grid<2, int> g1(5, 5);

    const int gkern[] = {
        1, 4,   6,  4, 1,
        4, 16, 24, 16, 4,
        6, 24, 36, 24, 6,
        4, 16, 24, 16, 4,
        1, 4,   6,  4, 1,
    };

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            g1(x, y) = gkern[y * 5 + x];
        }
    }

    // move into g2
    au::grid<2, int> g2(std::move(g1));

    BOOST_CHECK_EQUAL(g1.total_size(), 0);
    BOOST_CHECK(!g1.data());

    BOOST_CHECK_EQUAL(g2.size(0), 5);
    BOOST_CHECK_EQUAL(g2.size(1), 5);
    BOOST_CHECK_EQUAL(g2.total_size(), 25);
    BOOST_CHECK(g2.data());

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            BOOST_CHECK_EQUAL(g2(x, y), gkern[y * 5 + x]);
        }
    }
}

BOOST_AUTO_TEST_CASE(GridMoveAssignmentTest)
{
    // create and initialize g1 with known values
    au::grid<2, int> g1(5, 5);

    const int gkern[] = {
        1, 4,   6,  4, 1,
        4, 16, 24, 16, 4,
        6, 24, 36, 24, 6,
        4, 16, 24, 16, 4,
        1, 4,   6,  4, 1,
    };

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            g1(x, y) = gkern[y * 5 + x];
        }
    }

    // move into g2
    au::grid<2, int> g2;
    g2 = std::move(g1);

    BOOST_CHECK_EQUAL(g1.total_size(), 0);
    BOOST_CHECK(!g1.data());

    BOOST_CHECK_EQUAL(g2.size(0), 5);
    BOOST_CHECK_EQUAL(g2.size(1), 5);
    BOOST_CHECK_EQUAL(g2.total_size(), 25);
    BOOST_CHECK(g2.data());

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            BOOST_CHECK_EQUAL(g2(x, y), gkern[y * 5 + x]);
        }
    }

    au::grid<2, int> g3(2, 2);
    g3 = std::move(g2);

    BOOST_CHECK_EQUAL(g2.total_size(), 0);
    BOOST_CHECK(!g2.data());

    BOOST_CHECK_EQUAL(g3.size(0), 5);
    BOOST_CHECK_EQUAL(g3.size(1), 5);
    BOOST_CHECK_EQUAL(g3.total_size(), 25);
    BOOST_CHECK(g3.data());

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            BOOST_CHECK_EQUAL(g3(x, y), gkern[y * 5 + x]);
        }
    }
}

BOOST_AUTO_TEST_CASE(GridOutOfBoundsTest)
{
    au::grid<2, int> g1(5, 5);
    for (int x = 0; x < 5; ++x) {
        for (int y = 0; y < 5; ++y) {
            BOOST_CHECK_NO_THROW(g1.at(x, y));
            BOOST_CHECK(g1.within_bounds(x, y));
        }
    }
    BOOST_CHECK_THROW(g1.at(5, 5), std::out_of_range);
    BOOST_CHECK(!g1.within_bounds(5, 5));
}

BOOST_AUTO_TEST_CASE(GridIteratorTest)
{
    au::grid<2, int> empty;
    BOOST_CHECK_EQUAL(empty.begin(), empty.end());

    au::grid<2, int> g(5, 5);

    const int gkern[] = {
        1, 4,   6,  4, 1,
        4, 16, 24, 16, 4,
        6, 24, 36, 24, 6,
        4, 16, 24, 16, 4,
        1, 4,   6,  4, 1,
    };

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            g(y, x) = gkern[y * 5 + x];
        }
    }

    int i;

    // prefix
    i = 0;
    for (auto it = g.begin(); it != g.end(); ++it) {
        BOOST_CHECK_EQUAL(gkern[i], *it);
        ++i;
    }

    // postfix
    i = 0;
    for (auto it = g.begin(); it != g.end(); it++) {
        BOOST_CHECK_EQUAL(gkern[i], *it);
        ++i;
    }

    // bounds check on iterator coordinates
    for (auto it = g.begin(); it != g.end(); it++) {
        BOOST_CHECK(g.within_bounds(it.coord(0), it.coord(1)));
        ++i;
    }
}

BOOST_AUTO_TEST_CASE(GridGridIteratorTest)
{
    au::grid<2, int> g(5, 5);

    const int gkern[] = {
        1, 4,   6,  4, 1,
        4, 16, 24, 16, 4,
        6, 24, 36, 24, 6,
        4, 16, 24, 16, 4,
        1, 4,   6,  4, 1,
    };

    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            g(y, x) = gkern[y * 5 + x];
        }
    }

    const int expected[] = {
        16, 24, 16,
        24, 36, 24,
        16, 24, 16
    };

    au::grid<2, int>::index start(1, 1), end(3, 3);
    int i;

    // prefix
    i = 0;
    for (auto it = g.gbegin(start, end); it != g.gend(start, end); ++it) {
        BOOST_CHECK_EQUAL(*it, expected[i]);
        ++i;
    }

    // postfix
    i = 0;
    for (auto it = g.gbegin(start, end); it != g.gend(start, end); it++) {
        BOOST_CHECK_EQUAL(*it, expected[i]);
        ++i;
    }
}

BOOST_AUTO_TEST_CASE(GridResizeTest)
{
    au::grid<2, int> g;
    g.resize(2, 2);
    g(0, 0) = 1;
    g(0, 1) = 1;
    g(1, 0) = 2;
    g(1, 1) = 3;
    for (size_t i = 0; i < g.size(0); ++i) {
        for (size_t j = 0; j < g.size(1); ++j) {
            std::cout << g(i, j) << ' ';
        }
        std::cout << std::endl;
    }

    g.resize(5, 5);
    for (size_t i = 0; i < g.size(0); ++i) {
        for (size_t j = 0; j < g.size(1); ++j) {
            std::cout << g(i, j) << ' ';
        }
        std::cout << std::endl;
    }

    g.resize(1, 1);

    for (size_t i = 0; i < g.size(0); ++i) {
        for (size_t j = 0; j < g.size(1); ++j) {
            std::cout << g(i, j) << ' ';
        }
        std::cout << std::endl;
    }
}
