#include <stdlib.h>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE StackAllocatorTests
#include <boost/test/unit_test.hpp>

#include <memory/mempool.h>
#include <memory/StackAllocator.h>

BOOST_AUTO_TEST_CASE(TestMethod1)
{
    BOOST_CHECK_EQUAL(1, 1);
}

BOOST_AUTO_TEST_CASE(TestConstructor)
{
    const size_t pool_size = 1024;
    char* buff = new char[pool_size];
    au::mempool pool(buff, pool_size);

    au::StackAllocator allocator(pool);

    BOOST_CHECK_EQUAL((size_t)0, allocator.size());
    BOOST_CHECK_EQUAL((size_t)1024, allocator.capacity());
    
    delete [] buff;
}

BOOST_AUTO_TEST_CASE(TestRawAllocate)
{
    const size_t pool_size = 1024;
    char* buff = new char[pool_size];
    au::mempool pool(buff, pool_size);

    au::StackAllocator allocator(pool);

    int* ip1 = (int*)allocator.alloc(0);
    int* ip2 = (int*)allocator.alloc(10 * sizeof(int));
    int* ip3 = (int*)allocator.alloc(pool_size - 10 * sizeof(int));

    BOOST_CHECK(ip1 != nullptr);
    BOOST_CHECK(ip2 != nullptr);
    BOOST_CHECK(ip3 != nullptr);

    BOOST_CHECK_EQUAL(ip1, ip2);
    BOOST_CHECK_EQUAL(ip3, ip2 + 10);

    delete [] buff;
}

BOOST_AUTO_TEST_CASE(TestAllocateObject)
{
    const size_t pool_size = 1024;
    char* buff = new char[pool_size];
    au::mempool pool(buff, pool_size);

    au::StackAllocator allocator(pool);

    struct MyObject
    {
        MyObject(int* d) : d_(d)
        {
            ++(*d_);
        }

        int* d_;
    };

    int counter = 0;

    for (int i = 0; i < 10; ++i) {
        MyObject* mo = allocator.alloc_object<MyObject>(&counter);
        BOOST_CHECK(mo != nullptr);
    }

    BOOST_CHECK_EQUAL(10, counter);

    delete [] buff;
}

BOOST_AUTO_TEST_CASE(TestAllocateObjectArray)
{
    const size_t pool_size = 1024;
    char* buff = new char[pool_size];
    au::mempool pool(buff, pool_size);

    au::StackAllocator allocator(pool);

    struct MyObject
    {
        MyObject(int* d) : d_(d)
        {
            ++(*d_);
        }

        int* d_;
    };

    int counter = 0;

    MyObject* mos = allocator.alloc_objects<MyObject>(10, &counter);
    BOOST_CHECK_EQUAL(10, counter);

    delete [] buff;
}
