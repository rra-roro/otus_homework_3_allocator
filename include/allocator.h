#pragma once

#include <memory>
#include <list>
#include <type_traits>

template <typename T>
class memory_chunk
{
  public:
      memory_chunk(size_t count_items) : ptr_start_chunk(static_cast<T*>(malloc(count_items * sizeof(T))), std::free),
                                         size_buffer(count_items),
                                         ptr_next_item_mem(ptr_start_chunk.get()),
                                         size_free_memory(count_items),
                                         count_use_chunk(0){};

      bool contains(T* ptr) const
      {
            return (ptr >= ptr_start_chunk.get() && ptr < ptr_next_item_mem);
      }

      T* allocate_from(size_t n)
      {
            // To Do: сделать assert(ptr_next_item_mem != 0)
            count_use_chunk += n;
            size_free_memory -= n;

            T* ptr_current_item = ptr_next_item_mem;
            ptr_next_item_mem += n;

            return ptr_current_item;
      }

      void deallocate_from(size_t n)
      {
            count_use_chunk -= n;
      }

      size_t get_size_free_memory() const
      {
            return size_free_memory;
      }

      bool is_free_memory() const
      {
            return (size_free_memory != 0);
      }

      bool is_used() const
      {
            return (count_use_chunk != 0);
      }

  private:
      std::unique_ptr<T, decltype(std::free)*> ptr_start_chunk;
      size_t size_buffer;
      T* ptr_next_item_mem;
      size_t size_free_memory;
      size_t count_use_chunk;

      size_t get_size_usedbuffer() const
      {
            return size_buffer - size_free_memory;
      }

#ifdef _TEST
      FRIEND_TEST(CustomAllocatorTest, allocate_deallocate);
      FRIEND_TEST(CustomAllocatorTest, next_reserve);
      FRIEND_TEST(CustomAllocatorTest, chunk_shrink_to_fit);
      FRIEND_TEST(UseCustomAllocatorTest, map);
#endif
};


template <typename T, size_t initial_reservation = 1, size_t next_reservation = 1>
struct custom_allocator
{
      using value_type = T;

      custom_allocator() noexcept
      {
            static_assert(initial_reservation > 0 && next_reservation > 0, "custom_allocator's template args 'initial_reservation' and 'next_reservation' must be greater ZERO");
      }

      custom_allocator(size_t init_reserve, size_t next_reserve) noexcept : m_init_reserve_size(init_reserve), m_next_reserve_size(next_reserve)
      {
            static_assert(initial_reservation > 0 && next_reservation > 0, "custom_allocator's template args 'initial_reservation' and 'next_reservation' must be greater ZERO");
      }

      template <typename U, size_t initial_reservation_, size_t next_reservation_>
      custom_allocator(const custom_allocator<U, initial_reservation_, next_reservation_>& arg_alloc) noexcept : m_init_reserve_size(arg_alloc.m_init_reserve_size),
                                                                                                                 m_ref_next_reserve_size(arg_alloc.m_ref_next_reserve_size)
      {
            // std::cout << __PRETTY_FUNCTION__ << std::endl;
      }

      template <typename U>
      struct rebind
      {
            using other = custom_allocator<U, initial_reservation, next_reservation>;
      };

#ifdef _MSC_VER

      /* 
	   Платформоспецифическая оптимизация для MS VC
	   Специальная ф-ия allocate для выделения памяти под объект _Container_proxy
	   Поскольку _Container_proxy * custom_allocator<_Container_proxy,5,1>::allocate(unsigned __int64) вызывается один раз, 
	   то мы не будем выделять под _Container_proxy память из нашего пулла памяти, а напрямую выделем под него память из кучи.
	 */
      template <typename U,
          typename Fake = typename std::enable_if<std::is_same<T, std::_Container_proxy>::value, void>::type>

      T* allocate(U n)
      {
            // std::cout << __PRETTY_FUNCTION__ << "\nmem_size=" << n * sizeof(T) << std::endl;
            return static_cast<T*>(std::malloc(n * sizeof(T)));
      }

#endif // _MSC_VER

      //  обычная ф-ия выделения памяти для элементов контейнера
      T* allocate(std::size_t n)
      {
            // std::cout << __PRETTY_FUNCTION__ << "\nmem_size=" << n * sizeof(T) << std::endl;

            if (!chunks->empty() && chunks->back().is_free_memory())
            {
                  if (n > chunks->back().get_size_free_memory())
                  {
                        size_t size_chunk = std::max(n, *m_ref_next_reserve_size);
                        chunks->emplace_back(size_chunk);
                  }
                  return chunks->back().allocate_from(n);
            }
            else if (chunks->empty() && m_init_reserve_size == 1)
            {
                  return static_cast<T*>(malloc(n * sizeof(T)));
            }
            else if (chunks->empty())
            {
                  size_t size_chunk = std::max(n, m_init_reserve_size);
                  chunks->emplace_back(size_chunk);

                  return chunks->back().allocate_from(n);
            }
            else if (!chunks->back().is_free_memory() && *m_ref_next_reserve_size == 1)
            {
                  return static_cast<T*>(malloc(n * sizeof(T)));
            }
            else //if (!chunks->back().is_free_memory())
            {
                  size_t size_chunk = std::max(n, *m_ref_next_reserve_size);
                  chunks->emplace_back(size_chunk);

                  return chunks->back().allocate_from(n);
            }
      }


      void deallocate(T* p, std::size_t n)
      {
            for (auto it_chunk = chunks->begin(); it_chunk != chunks->end(); it_chunk++)
            {
                  if (it_chunk->contains(p))
                  {
                        it_chunk->deallocate_from(n);

                        if (!it_chunk->is_used())
                              chunks->erase(it_chunk);

                        return;
                  }
            }

            free(p);
            return;
      }

      void next_reserve(std::size_t count)
      {
            assert(count > 0);
            m_next_reserve_size = count;
      }

      template <typename U, std::size_t initial_reservation_, std::size_t next_reservation_>
      friend struct custom_allocator;

  private:
      std::shared_ptr<std::list<memory_chunk<T>>> chunks = std::make_shared<std::list<memory_chunk<T>>>();

      std::size_t m_init_reserve_size = initial_reservation;

      std::size_t m_next_reserve_size = next_reservation;
      std::size_t* m_ref_next_reserve_size = &m_next_reserve_size;

#ifdef _TEST
      FRIEND_TEST(CustomAllocatorTest, DefaultCtor);
      FRIEND_TEST(CustomAllocatorTest, CustomCtor1);
      FRIEND_TEST(CustomAllocatorTest, CopyCtor);
      FRIEND_TEST(CustomAllocatorTest, allocate_deallocate);
      FRIEND_TEST(CustomAllocatorTest, next_reserve);
      FRIEND_TEST(CustomAllocatorTest, chunk_shrink_to_fit);
      FRIEND_TEST(UseCustomAllocatorTest, map);

      template <typename U, std::size_t initial_reservation_, std::size_t next_reservation_>
      friend struct mock_custom_allocator;
#endif
};

template <typename T, std::size_t reserve_tt, typename U, std::size_t reserve_tu>
constexpr bool operator==(const custom_allocator<T, reserve_tt>&, const custom_allocator<U, reserve_tu>&) noexcept
{
      return true;
}

template <typename T, std::size_t reserve_tt, typename U, std::size_t reserve_tu>
constexpr bool operator!=(const custom_allocator<T, reserve_tt>&, const custom_allocator<U, reserve_tu>&) noexcept
{
      return false;
}
