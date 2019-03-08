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

TEST_F(custom_allocator, DefaultCtor)
{
      custom_allocator<std::pair<const int, int>> my_alloc;
      ASSERT_TRUE(my_alloc.m_init_reserve_size == 1);
      ASSERT_TRUE(my_alloc.m_next_reserve_size == 1);
      ASSERT_TRUE(my_alloc.m_ref_next_reserve_size == &my_alloc.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 5> my_alloc5;
      ASSERT_TRUE(my_alloc5.m_init_reserve_size == 5);
      ASSERT_TRUE(my_alloc5.m_next_reserve_size == 1);
      ASSERT_TRUE(my_alloc5.m_ref_next_reserve_size == &my_alloc.m_next_reserve_size);

      custom_allocator<std::pair<const int, int>, 5, 10> my_alloc510;
      ASSERT_TRUE(my_alloc510.m_init_reserve_size == 5);
      ASSERT_TRUE(my_alloc510.m_next_reserve_size == 10);
      ASSERT_TRUE(my_alloc510.m_ref_next_reserve_size == &my_alloc.m_next_reserve_size);
}
