#include "buf_pool.h"
#include <gtest/gtest.h>

TEST(sanity_checks, initialization)
{
    ASSERT_EQ(nullptr, alloc(1));
    ASSERT_EQ(0, init(1));
    ASSERT_EQ(-1, init(1));
}

TEST(allocation, simple)
{
    void *buf;
    
    // reset buffer pool
    ASSERT_EQ(0, reset());
    ASSERT_EQ(0, init(10));

    // test allocation of invalid sizes
    buf = alloc(0);
    ASSERT_EQ(nullptr, buf);
    buf = alloc(3);
    ASSERT_EQ(nullptr, buf);

    // test valid sizes and print the pointers
    buf = alloc(5);
    ASSERT_NE(nullptr, buf);
    buf = alloc(5);
    ASSERT_NE(nullptr, buf);

    // test allocating beyond max capacity
    buf = alloc(5);
    ASSERT_EQ(nullptr, buf);

    // print buffer stats
    print_buf_stats();
}

TEST(deallocation, simple)
{
    void *buf1;
    void *buf2;
    void *buf3;
    void *buf4;
    void *buf5;
    
    // reset buffer pool
    ASSERT_EQ(0, reset());
    ASSERT_EQ(0, init(10));

    // allocate memory
    buf1 = alloc(5);
    buf2 = alloc(2);
    buf3 = alloc(2);
    buf4 = alloc(1);

    // print buffer stats
    print_buf_stats();
    std::cout << std::endl;

    // test deallocation of valid buffers
    ASSERT_EQ(0, dealloc(buf2));
    ASSERT_EQ(0, dealloc(buf3));
    ASSERT_EQ(0, dealloc(buf4));

    // print buffer stats
    print_buf_stats();
    std::cout << std::endl;

    // test deallocation of invalid buffers
    ASSERT_EQ(-1, dealloc(buf2));
    ASSERT_EQ(-1, dealloc(buf3));
    ASSERT_EQ(-1, dealloc(buf4));

    // test allocation of another buffer
    buf5 = alloc(2);
    ASSERT_NE(nullptr, buf5);
    
    // print buffer stats
    print_buf_stats();
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
