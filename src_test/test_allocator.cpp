#include <string>
#include "gtest/gtest.h"
#include "gtest/gtest_prod.h"
#include "gmock/gmock.h"

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
      {
            custom_allocator<std::pair<const int, int>> my_alloc;
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);

            std::pair<const int, int>* ptr = my_alloc.allocate(1);
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);

            my_alloc.deallocate(ptr, 1);
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);
      }

      {
            custom_allocator<std::pair<const int, int>, 1, 1> my_alloc;
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);

            std::pair<const int, int>* ptr = my_alloc.allocate(1);
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);

            my_alloc.deallocate(ptr, 1);
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);
      }

      {
            custom_allocator<std::pair<const int, int>, 2, 2> my_alloc;
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);

            std::pair<const int, int>* ptr1 = my_alloc.allocate(1);
            ASSERT_TRUE(my_alloc.chunks->empty() == false);
            ASSERT_TRUE(my_alloc.chunks->size() == 1);
            ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
            ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 1);
            ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == true);
            ASSERT_TRUE(my_alloc.chunks->back().contains(ptr1) == true);
            ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 1);

            std::pair<const int, int>* ptr2 = my_alloc.allocate(1);
            ASSERT_TRUE(my_alloc.chunks->empty() == false);
            ASSERT_TRUE(my_alloc.chunks->size() == 1);
            ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
            ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 2);
            ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == false);
            ASSERT_TRUE(my_alloc.chunks->back().contains(ptr2) == true);
            ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 0);

            std::pair<const int, int>* ptr3 = my_alloc.allocate(1);
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
            ASSERT_TRUE(my_alloc.chunks->empty() == false);
            ASSERT_TRUE(my_alloc.chunks->size() == 1);
            ASSERT_TRUE(my_alloc.chunks->back().is_used() == true);
            ASSERT_TRUE(my_alloc.chunks->back().count_use_chunk == 1);
            ASSERT_TRUE(my_alloc.chunks->back().is_free_memory() == true);
            ASSERT_TRUE(my_alloc.chunks->back().contains(ptr2) == false);
            ASSERT_TRUE(my_alloc.chunks->back().contains(ptr3) == true);
            ASSERT_TRUE(my_alloc.chunks->back().get_size_free_memory() == 1);

            my_alloc.deallocate(ptr3, 1);
            ASSERT_TRUE(my_alloc.chunks->empty() == true);
            ASSERT_TRUE(my_alloc.chunks->size() == 0);
      }
}

TEST_F(CustomAllocatorTest, next_reserve)
{
      custom_allocator<std::pair<const int, int>, 2, 2> my_alloc;
      ASSERT_TRUE(my_alloc.chunks->empty() == true);
      ASSERT_TRUE(my_alloc.chunks->size() == 0);
      ASSERT_TRUE(my_alloc.m_init_reserve_size == 2);
      ASSERT_TRUE(my_alloc.m_next_reserve_size == 2);


      std::pair<const int, int>* ptr1 = my_alloc.allocate(1);
      std::pair<const int, int>* ptr2 = my_alloc.allocate(1);
      ASSERT_TRUE(my_alloc.chunks->empty() == false);
      ASSERT_TRUE(my_alloc.chunks->size() == 1);
      ASSERT_TRUE(my_alloc.chunks->back().size_buffer = 2);

      std::pair<const int, int>* ptr3 = my_alloc.allocate(1);
      std::pair<const int, int>* ptr4 = my_alloc.allocate(1);
      ASSERT_TRUE(my_alloc.chunks->empty() == false);
      ASSERT_TRUE(my_alloc.chunks->size() == 2);
      ASSERT_TRUE(my_alloc.chunks->back().size_buffer = 2);

      my_alloc.next_reserve(4);
      ASSERT_TRUE(my_alloc.m_init_reserve_size == 2);
      ASSERT_TRUE(my_alloc.m_next_reserve_size == 4);

      std::pair<const int, int>* ptr5 = my_alloc.allocate(1);
      ASSERT_TRUE(my_alloc.chunks->empty() == false);
      ASSERT_TRUE(my_alloc.chunks->size() == 3);
      ASSERT_TRUE(my_alloc.chunks->back().size_buffer = 4);

      my_alloc.deallocate(ptr1, 1);
      my_alloc.deallocate(ptr2, 1);
      my_alloc.deallocate(ptr3, 1);
      my_alloc.deallocate(ptr4, 1);
      my_alloc.deallocate(ptr5, 1);
      ASSERT_TRUE(my_alloc.chunks->empty() == true);
      ASSERT_TRUE(my_alloc.chunks->size() == 0);
}


class UseCustomAllocatorTest : public testing::Test
{
  public:
      void SetUp() {}
      void TearDown() {}
};

using ::testing::_;
using ::testing::Eq;
using ::testing::AnyOf;
using ::testing::Invoke;
using ::testing::AtMost;


template <typename T, size_t initial_reservation = 1, size_t next_reservation = 1>
struct mock_custom_allocator
{
      using value_type = T;

      template <typename U>
      struct rebind
      {
            using other = mock_custom_allocator<U, initial_reservation, next_reservation>;
      };

      mock_custom_allocator() noexcept : Al()
      {
            DelegateToReal();
      }

      mock_custom_allocator(size_t init_reserve, size_t next_reserve) noexcept : Al(init_reserve, next_reserve)
      {
            DelegateToReal();
      }

      template <typename U, size_t initial_reservation_, size_t next_reservation_>
      mock_custom_allocator(const mock_custom_allocator<U, initial_reservation_, next_reservation_>& arg_alloc) noexcept : Al(arg_alloc.Al)
      {
            DelegateToReal();
      };

      template <typename U, size_t initial_reservation_, size_t next_reservation_>
      mock_custom_allocator(mock_custom_allocator<U, initial_reservation_, next_reservation_>& arg_alloc) noexcept : Al(arg_alloc.Al)
      {
            DelegateToReal();
      };


      void DelegateToReal()
      {
            ON_CALL(*this, next_reserve(_))
                .WillByDefault(Invoke(&Al, &custom_allocator<T, initial_reservation, next_reservation>::next_reserve));
            ON_CALL(*this, deallocate(_, Eq(1)))
                .WillByDefault(Invoke(&Al, &custom_allocator<T, initial_reservation, next_reservation>::deallocate));

            if constexpr (std::is_same<T, std::_Container_proxy>::value)
            {
                  ON_CALL(*this, allocate(Eq(1)))
                      .WillByDefault(Invoke(&Al, &custom_allocator<std::_Container_proxy, initial_reservation, next_reservation>::template allocate<size_t, bool>));

                  EXPECT_CALL(*this, allocate(1)).Times(AtMost(1));
                  EXPECT_CALL(*this, deallocate(_, 1)).Times(AtMost(1));
            }
            else
            {
                  ON_CALL(*this, allocate(Eq(1)))
                      .WillByDefault(Invoke(&Al, &custom_allocator<T, initial_reservation, next_reservation>::allocate));

                  EXPECT_CALL(*this, allocate(1)).Times(AtMost(4));
                  EXPECT_CALL(*this, deallocate(_, 1)).Times(AtMost(4));
            }
      }


      //  обычная ф-ия выделения памяти для элементов контейнера
      MOCK_METHOD1_T(allocate, T*(std::size_t n));
      MOCK_METHOD2_T(deallocate, void(T* p, std::size_t n));
      MOCK_METHOD1(next_reserve, void(std::size_t count));

      template <typename U, std::size_t initial_reservation_, std::size_t next_reservation_>
      friend struct mock_custom_allocator;

  private:
      custom_allocator<T, initial_reservation, next_reservation> Al;
};

TEST_F(UseCustomAllocatorTest, map)
{
      // auto al = mock_custom_allocator<std::pair<const int, int>>();

      auto m = std::map<int, int, std::less<int>, mock_custom_allocator<std::pair<const int, int>>>();
      m[0] = 0;
      m[1] = 1;
      m[2] = 2;
      m.clear();

      //mock_custom_allocator<std::pair<const int, int>, 2, 2> my_alloc;
      //my_alloc.expect();
      //auto m = std::map<int, int, std::less<int>, mock_custom_allocator<std::pair<const int, int>>>(my_alloc);
      //m[0] = 0;
      //m[1] = 1;
      //m[2] = 2;
      //m.clear();


      /* {
            auto m = std::map<int, int, std::less<int>, mock_custom_allocator<std::pair<const int, int>, 2, 2>>();
            m[0] = 0;
            m[1] = 1;
            m[2] = 2;
            m.clear();
      }
      {
            custom_allocator<std::pair<const int, int>, 2, 2> my_alloc;
            auto m = std::map<int, int, std::less<int>, mock_custom_allocator<std::pair<const int, int>>>(my_alloc);
            m[0] = 0;
            m[1] = 1;
            m[2] = 2;
            m.clear();
      }*/
}
