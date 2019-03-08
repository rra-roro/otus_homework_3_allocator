#include <string>
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"

#include "lib_version.h"

#define _TEST 1

#include "allocator.h"
#include "forward_list.h"
#include "factorial.h"


TEST(version, test1)
{
      ASSERT_TRUE(version() > 0);
}

class CustomAllocatorTest : public testing::Test
{
  public:
      void SetUp() {}
      void TearDown() {}
};

TEST_F(CustomAllocatorTest, DefaultCtor)
{
      custom_allocator<std::pair<const int, int>> my_alloc1;
      ASSERT_TRUE(my_alloc1.m_init_reserve_size == 1);
      ASSERT_TRUE(my_alloc1.m_next_reserve_size == 1);
      ASSERT_TRUE(my_alloc1.m_ref_next_reserve_size == &my_alloc1.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 5> my_alloc2;
      ASSERT_TRUE(my_alloc2.m_init_reserve_size == 5);
      ASSERT_TRUE(my_alloc2.m_next_reserve_size == 1);
      ASSERT_TRUE(my_alloc2.m_ref_next_reserve_size == &my_alloc2.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 5, 10> my_alloc3;
      ASSERT_TRUE(my_alloc3.m_init_reserve_size == 5);
      ASSERT_TRUE(my_alloc3.m_next_reserve_size == 10);
      ASSERT_TRUE(my_alloc3.m_ref_next_reserve_size == &my_alloc3.m_next_reserve_size);
}

TEST_F(CustomAllocatorTest, CustomCtor1)
{
      custom_allocator<std::pair<const int, int>> my_alloc1(5, 10);
      ASSERT_TRUE(my_alloc1.m_init_reserve_size == 5);
      ASSERT_TRUE(my_alloc1.m_next_reserve_size == 10);
      ASSERT_TRUE(my_alloc1.m_ref_next_reserve_size == &my_alloc1.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 2> my_alloc2(5, 10);
      ASSERT_TRUE(my_alloc2.m_init_reserve_size == 5);
      ASSERT_TRUE(my_alloc2.m_next_reserve_size == 10);
      ASSERT_TRUE(my_alloc2.m_ref_next_reserve_size == &my_alloc2.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 2, 2> my_alloc3(5, 10);
      ASSERT_TRUE(my_alloc3.m_init_reserve_size == 5);
      ASSERT_TRUE(my_alloc3.m_next_reserve_size == 10);
      ASSERT_TRUE(my_alloc3.m_ref_next_reserve_size == &my_alloc3.m_next_reserve_size);
}

TEST_F(CustomAllocatorTest, CopyCtor)
{
      custom_allocator<std::pair<const int, int>> my_alloc1;
      custom_allocator<std::pair<const int, int>> ret_alloc1(my_alloc1);
      ASSERT_TRUE(ret_alloc1.m_init_reserve_size == 1);
      ASSERT_TRUE(ret_alloc1.m_next_reserve_size == 1);
      ASSERT_TRUE(*ret_alloc1.m_ref_next_reserve_size == 1);
      ASSERT_TRUE(ret_alloc1.m_ref_next_reserve_size == &my_alloc1.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 5> my_alloc2;
      custom_allocator<std::pair<const int, int>> ret_alloc2(my_alloc2);
      ASSERT_TRUE(ret_alloc2.m_init_reserve_size == 5);
      ASSERT_TRUE(ret_alloc2.m_next_reserve_size == 1);
      ASSERT_TRUE(*ret_alloc2.m_ref_next_reserve_size == 1);
      ASSERT_TRUE(ret_alloc2.m_ref_next_reserve_size == &my_alloc2.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 5, 10> my_alloc3;
      custom_allocator<std::pair<const int, int>> ret_alloc3(my_alloc3);
      ASSERT_TRUE(ret_alloc3.m_init_reserve_size == 5);
      ASSERT_TRUE(ret_alloc3.m_next_reserve_size == 1);
      ASSERT_TRUE(*ret_alloc3.m_ref_next_reserve_size == 10);
      ASSERT_TRUE(ret_alloc3.m_ref_next_reserve_size == &my_alloc3.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 5, 10> my_alloc4(6, 11);
      custom_allocator<std::pair<const int, int>> ret_alloc4(my_alloc4);
      ASSERT_TRUE(ret_alloc4.m_init_reserve_size == 6);
      ASSERT_TRUE(ret_alloc4.m_next_reserve_size == 1);
      ASSERT_TRUE(*ret_alloc4.m_ref_next_reserve_size == 11);
      ASSERT_TRUE(ret_alloc4.m_ref_next_reserve_size == &my_alloc4.m_next_reserve_size);
}

TEST_F(CustomAllocatorTest, allocate_deallocate)
{
      custom_allocator<std::pair<const int, int>> my_alloc1;
      std::pair<const int, int>* ptr1 = my_alloc1.allocate(1);
      ASSERT_TRUE(my_alloc1.chunks->empty() == true);


}
