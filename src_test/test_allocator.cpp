﻿#include <string>
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

namespace roro_lib
{
      class custom_allocator_test : public testing::Test
      {
        public:
            void SetUp() {}
            void TearDown() {}
      };

      TEST_F(custom_allocator_test, DefaultCtor)
      {
            custom_allocator<std::pair<const int, int>> my_alloc1;
            ASSERT_TRUE(my_alloc1.init_reserve_size == 1);
            ASSERT_TRUE(my_alloc1.next_reserve_size == 1);
            ASSERT_TRUE(my_alloc1.ref_next_reserve_size == &my_alloc1.next_reserve_size);

            custom_allocator<std::pair<const int, int>, 5> my_alloc2;
            ASSERT_TRUE(my_alloc2.init_reserve_size == 5);
            ASSERT_TRUE(my_alloc2.next_reserve_size == 1);
            ASSERT_TRUE(my_alloc2.ref_next_reserve_size == &my_alloc2.next_reserve_size);

            custom_allocator<std::pair<const int, int>, 5, 10> my_alloc3;
            ASSERT_TRUE(my_alloc3.init_reserve_size == 5);
            ASSERT_TRUE(my_alloc3.next_reserve_size == 10);
            ASSERT_TRUE(my_alloc3.ref_next_reserve_size == &my_alloc3.next_reserve_size);
      }

      TEST_F(custom_allocator_test, CustomCtor1)
      {
            custom_allocator<std::pair<const int, int>> my_alloc1(5, 10);
            ASSERT_TRUE(my_alloc1.init_reserve_size == 5);
            ASSERT_TRUE(my_alloc1.next_reserve_size == 10);
            ASSERT_TRUE(my_alloc1.ref_next_reserve_size == &my_alloc1.next_reserve_size);

            custom_allocator<std::pair<const int, int>, 2> my_alloc2(5, 10);
            ASSERT_TRUE(my_alloc2.init_reserve_size == 5);
            ASSERT_TRUE(my_alloc2.next_reserve_size == 10);
            ASSERT_TRUE(my_alloc2.ref_next_reserve_size == &my_alloc2.next_reserve_size);

            custom_allocator<std::pair<const int, int>, 2, 2> my_alloc3(5, 10);
            ASSERT_TRUE(my_alloc3.init_reserve_size == 5);
            ASSERT_TRUE(my_alloc3.next_reserve_size == 10);
            ASSERT_TRUE(my_alloc3.ref_next_reserve_size == &my_alloc3.next_reserve_size);
      }

      TEST_F(custom_allocator_test, CopyCtor)
      {
            custom_allocator<std::pair<const int, int>> my_alloc1;
            custom_allocator<std::pair<const int, int>> ret_alloc1(my_alloc1);
            ASSERT_TRUE(ret_alloc1.init_reserve_size == 1);
            ASSERT_TRUE(ret_alloc1.next_reserve_size == 1);
            ASSERT_TRUE(*ret_alloc1.ref_next_reserve_size == 1);
            ASSERT_TRUE(ret_alloc1.ref_next_reserve_size == &my_alloc1.next_reserve_size);

            custom_allocator<std::pair<const int, int>, 5> my_alloc2;
            custom_allocator<std::pair<const int, int>> ret_alloc2(my_alloc2);
            ASSERT_TRUE(ret_alloc2.init_reserve_size == 5);
            ASSERT_TRUE(ret_alloc2.next_reserve_size == 1);
            ASSERT_TRUE(*ret_alloc2.ref_next_reserve_size == 1);
            ASSERT_TRUE(ret_alloc2.ref_next_reserve_size == &my_alloc2.next_reserve_size);

            custom_allocator<std::pair<const int, int>, 5, 10> my_alloc3;
            custom_allocator<std::pair<const int, int>> ret_alloc3(my_alloc3);
            ASSERT_TRUE(ret_alloc3.init_reserve_size == 5);
            ASSERT_TRUE(ret_alloc3.next_reserve_size == 1);
            ASSERT_TRUE(*ret_alloc3.ref_next_reserve_size == 10);
            ASSERT_TRUE(ret_alloc3.ref_next_reserve_size == &my_alloc3.next_reserve_size);

            custom_allocator<std::pair<const int, int>, 5, 10> my_alloc4(6, 11);
            custom_allocator<std::pair<const int, int>> ret_alloc4(my_alloc4);
            ASSERT_TRUE(ret_alloc4.init_reserve_size == 6);
            ASSERT_TRUE(ret_alloc4.next_reserve_size == 1);
            ASSERT_TRUE(*ret_alloc4.ref_next_reserve_size == 11);
            ASSERT_TRUE(ret_alloc4.ref_next_reserve_size == &my_alloc4.next_reserve_size);
      }

      TEST_F(custom_allocator_test, allocate_deallocate)
      {
            {
                  custom_allocator<std::pair<const int, int>> my_alloc;
                  ASSERT_TRUE(my_alloc.chunks == nullptr);

                  std::pair<const int, int>* ptr = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);

                  my_alloc.deallocate(ptr, 1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);
            }

            {
                  custom_allocator<std::pair<const int, int>, 0, 2> my_alloc;
                  ASSERT_TRUE(my_alloc.chunks == nullptr);

                  std::pair<const int, int>* ptr1 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);

                  std::pair<const int, int>* ptr2 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);

                  my_alloc.deallocate(ptr1, 1);
                  my_alloc.deallocate(ptr2, 1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);
            }

            {
                  custom_allocator<std::pair<const int, int>, 2, 0> my_alloc;
                  ASSERT_TRUE(my_alloc.chunks == nullptr);

                  std::pair<const int, int>* ptr1 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 1);
                  std::pair<const int, int>* ptr2 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 1);
                  std::pair<const int, int>* ptr3 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 1);

                  my_alloc.deallocate(ptr1, 1);
                  my_alloc.deallocate(ptr2, 1);
                  my_alloc.deallocate(ptr3, 1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);
            }

            {
                  custom_allocator<std::pair<const int, int>, 1, 1> my_alloc;
                  ASSERT_TRUE(my_alloc.chunks == nullptr);

                  std::pair<const int, int>* ptr = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);

                  my_alloc.deallocate(ptr, 1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);
            }

            {
                  custom_allocator<std::pair<const int, int>, 2, 2> my_alloc;
                  ASSERT_TRUE(my_alloc.chunks == nullptr);

                  std::pair<const int, int>* ptr1 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 1);
                  ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 1);
                  ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().contains(ptr1) == true);
                  ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 1);

                  std::pair<const int, int>* ptr2 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 1);
                  ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 2);
                  ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == false);
                  ASSERT_TRUE(my_alloc.chunks->back().contains(ptr2) == true);
                  ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 0);

                  std::pair<const int, int>* ptr3 = my_alloc.allocate(1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 2);
                  ASSERT_TRUE(my_alloc.chunks->front().is_used() == true);
                  ASSERT_TRUE(my_alloc.chunks->front().count_use_chunk == 2);
                  ASSERT_TRUE(my_alloc.chunks->front().is_free_memory() == false);
                  ASSERT_TRUE(my_alloc.chunks->front().contains(ptr3) == false);
                  ASSERT_TRUE(my_alloc.chunks->front().get_size_free_memory() == 0);
                  ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 1);
                  ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().contains(ptr3) == true);
                  ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 1);

                  my_alloc.deallocate(ptr1, 1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 2);
                  ASSERT_TRUE(my_alloc.chunks->front().is_used() == true);
                  ASSERT_TRUE(my_alloc.chunks->front().count_use_chunk == 1);
                  ASSERT_TRUE(my_alloc.chunks->front().is_free_memory() == false);
                  ASSERT_TRUE(my_alloc.chunks->front().contains(ptr1) == true);
                  ASSERT_TRUE(my_alloc.chunks->front().get_size_free_memory() == 0);
                  ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 1);
                  ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().contains(ptr1) == false);
                  ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 1);

                  my_alloc.deallocate(ptr2, 1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == false);
                  ASSERT_TRUE(my_alloc.chunks->size() == 1);
                  ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 1);
                  ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == true);
                  ASSERT_TRUE(my_alloc.chunks->back().contains(ptr2) == false);
                  ASSERT_TRUE(my_alloc.chunks->back().contains(ptr3) == true);
                  ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 1);

                  my_alloc.deallocate(ptr3, 1);
                  ASSERT_TRUE(my_alloc.chunks != nullptr);
                  ASSERT_TRUE(my_alloc.chunks->empty() == true);
                  ASSERT_TRUE(my_alloc.chunks->size() == 0);
            }
      }

      TEST_F(custom_allocator_test, next_reserve)
      {
            custom_allocator<std::pair<const int, int>, 2, 2> my_alloc;
            ASSERT_TRUE(my_alloc.chunks == nullptr);
            ASSERT_TRUE(my_alloc.init_reserve_size == 2);
            ASSERT_TRUE(my_alloc.next_reserve_size == 2);


            std::pair<const int, int>* ptr1 = my_alloc.allocate(1);
            std::pair<const int, int>* ptr2 = my_alloc.allocate(1);
            ASSERT_TRUE(my_alloc.chunks != nullptr);
            ASSERT_TRUE(my_alloc.chunks->empty() == false);
            ASSERT_TRUE(my_alloc.chunks->size() == 1);
            ASSERT_TRUE(my_alloc.chunks->back().size_buffer = 2);

            std::pair<const int, int>* ptr3 = my_alloc.allocate(1);
            std::pair<const int, int>* ptr4 = my_alloc.allocate(1);
            ASSERT_TRUE(my_alloc.chunks != nullptr);
            ASSERT_TRUE(my_alloc.chunks->empty() == false);
            ASSERT_TRUE(my_alloc.chunks->size() == 2);
            ASSERT_TRUE(my_alloc.chunks->back().size_buffer = 2);

            my_alloc.next_reserve(4);
            ASSERT_TRUE(my_alloc.chunks != nullptr);
            ASSERT_TRUE(my_alloc.init_reserve_size == 2);
            ASSERT_TRUE(my_alloc.next_reserve_size == 4);

            std::pair<const int, int>* ptr5 = my_alloc.allocate(1);
            ASSERT_TRUE(my_alloc.chunks != nullptr);
            ASSERT_TRUE(my_alloc.chunks->empty() == false);
            ASSERT_TRUE(my_alloc.chunks->size() == 3);
            ASSERT_TRUE(my_alloc.chunks->back().size_buffer = 4);

            my_alloc.deallocate(ptr1, 1);
            my_alloc.deallocate(ptr2, 1);
            my_alloc.deallocate(ptr3, 1);
            my_alloc.deallocate(ptr4, 1);
            my_alloc.deallocate(ptr5, 1);
            ASSERT_TRUE(my_alloc.chunks != nullptr);
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);
      }

      TEST(custom_forward_list, test1)
      {
            std::vector<int> expected = { 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };


            custom_forward_list<int, custom_allocator<int, 10>> list_my_allocator;
            for (int i = 0; i < 14; ++i)
            {
                  list_my_allocator.push_front(i);
            }

            auto it_expected = expected.cbegin();

            for (auto& obj : list_my_allocator)
            {
                  ASSERT_TRUE(*it_expected == obj);
                  it_expected++;
            }
      }

      TEST(custom_forward_list, test2)
      {
            std::vector<int> expected = { 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };

            custom_allocator<int> my_al(10, 5);

            custom_forward_list<int, custom_allocator<int>> list_my_allocator(my_al);
            for (int i = 0; i < 14; ++i)
            {
                  list_my_allocator.push_front(i);
            }

            auto it_expected = expected.cbegin();

            for (auto& obj : list_my_allocator)
            {
                  ASSERT_TRUE(*it_expected == obj);
                  it_expected++;
            }
      }

      TEST(custom_forward_list, test3)
      {
            std::vector<int> expected = { 8, 7, 6, 5, 4, 3, 2, 1, 0, 4, 3, 2, 1, 0 };

            custom_allocator<int> my_al(5, 2);

            custom_forward_list<int, custom_allocator<int>> list_my_allocator(my_al);
            for (int i = 0; i < 5; ++i)
            {
                  list_my_allocator.push_front(i);
            }

            my_al.next_reserve(20);

            for (int i = 0; i < 9; ++i)
            {
                  list_my_allocator.push_front(i);
            }

            auto it_expected = expected.cbegin();

            for (auto& obj : list_my_allocator)
            {
                  ASSERT_TRUE(*it_expected == obj);
                  it_expected++;
            }
      }
}
