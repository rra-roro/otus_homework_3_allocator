﻿#pragma once

#include <memory>
#include <new>
#include <list>
#include <type_traits>
#include <cassert>

namespace roro_lib
{
      template <typename T, size_t initial_reservation = 1, size_t next_reservation = 1>
      struct custom_allocator
      {
            using value_type = T;

            custom_allocator() noexcept = default;

            custom_allocator(size_t init_reserve, size_t next_reserve) noexcept : init_reserve_size((init_reserve == 0) ? 1 : init_reserve),
                                                                                  next_reserve_size((next_reserve == 0) ? 1 : next_reserve){};

            template <typename U, size_t initial_reservation_, size_t next_reservation_>
            custom_allocator(const custom_allocator<U, initial_reservation_, next_reservation_>& arg_alloc) noexcept : init_reserve_size(arg_alloc.init_reserve_size),
                                                                                                                       ref_next_reserve_size(arg_alloc.ref_next_reserve_size){}

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
                  return static_cast<T*>(::operator new[](n * sizeof(T)));
            };

#endif // _MSC_VER

            //  обычная ф-ия выделения памяти для элементов контейнера
            T* allocate(std::size_t n)
            {
                  if (chunks == nullptr)
                  {
                        chunks = std::make_shared<std::list<memory_chunk>>();
                  }

                  if (!chunks->empty() && chunks->back().is_free_memory())
                  {
                        if (n > chunks->back().get_size_free_memory())
                        {
                              size_t size_chunk = std::max(n, *ref_next_reserve_size);
                              chunks->emplace_back(size_chunk);
                        }
                        return chunks->back().allocate_from(n);
                  }
                  else if (init_reserve_size == 1 || (!chunks->empty() && !chunks->back().is_free_memory() && *ref_next_reserve_size == 1))
                  {
                        return static_cast<T*>(::operator new[](n * sizeof(T)));
                  }
                  else if (chunks->empty())
                  {
                        size_t size_chunk = std::max(n, init_reserve_size);
                        chunks->emplace_back(size_chunk);

                        return chunks->back().allocate_from(n);
                  }
                  else // if (!chunks->back().is_free_memory())
                  {
                        size_t size_chunk = std::max(n, *ref_next_reserve_size);
                        chunks->emplace_back(size_chunk);

                        return chunks->back().allocate_from(n);
                  }
            };


            void deallocate(T* p, std::size_t n)
            {
                  if (chunks != nullptr)
                  {
                        for (auto it_chunk = chunks->begin(); it_chunk != chunks->end(); ++it_chunk)
                        {
                              if (it_chunk->contains(p))
                              {
                                    it_chunk->deallocate_from(n);

                                    if (!it_chunk->is_used())
                                          chunks->erase(it_chunk);

                                    return;
                              }
                        }
                  }

                  ::operator delete[](p);
                  return;
            };

            void next_reserve(std::size_t count)
            {
                  assert(count != 0);
                  next_reserve_size = count;
            };

            template <typename U, std::size_t initial_reservation_, std::size_t next_reservation_>
            friend struct custom_allocator;

        private:
            class memory_chunk;
            std::shared_ptr<std::list<memory_chunk>> chunks = nullptr;

            std::size_t init_reserve_size = (initial_reservation == 0) ? 1 : initial_reservation;

            std::size_t next_reserve_size = (next_reservation == 0) ? 1 : next_reservation;
            std::size_t* ref_next_reserve_size = &next_reserve_size;

            class memory_chunk
            {
              public:
                  memory_chunk(size_t count_items) : ptr_start_chunk(static_cast<T*>(::operator new[](count_items * sizeof(T)))),
                                                     ptr_next_item_mem(ptr_start_chunk.get()),
                                                     size_buffer(count_items),
                                                     size_free_memory(count_items),
                                                     count_use_chunk(0)
                  {
                  };

                  bool contains(T* ptr) const noexcept
                  {
                        assert(ptr_next_item_mem != nullptr);
                        return (ptr >= ptr_start_chunk.get() && ptr < ptr_next_item_mem);
                  }

                  T* allocate_from(size_t n) noexcept
                  {
                        assert(ptr_next_item_mem != nullptr);
                        assert((count_use_chunk + n) <= size_buffer);
                        assert(size_free_memory >= n);

                        count_use_chunk += n;
                        size_free_memory -= n;

                        T* ptr_current_item = ptr_next_item_mem;
                        ptr_next_item_mem += n;

                        return ptr_current_item;
                  }

                  void deallocate_from(size_t n) noexcept
                  {
                        assert((count_use_chunk - n) < count_use_chunk);
                        count_use_chunk -= n;
                  }

                  size_t get_size_free_memory() const noexcept
                  {
                        return size_free_memory;
                  }

                  bool is_free_memory() const noexcept
                  {
                        return (size_free_memory != 0);
                  }

                  bool is_used() const noexcept
                  {
                        return (count_use_chunk != 0);
                  }

              private:
                  std::unique_ptr<T[]> ptr_start_chunk;
                  T* ptr_next_item_mem;

                  size_t size_buffer;
                  size_t size_free_memory;
                  size_t count_use_chunk;

                  size_t get_size_usedbuffer() const noexcept
                  {
                        return size_buffer - size_free_memory;
                  }

#ifdef _TEST
                  FRIEND_TEST(custom_allocator_test, allocate_deallocate);
                  FRIEND_TEST(custom_allocator_test, next_reserve);
#endif
            };


#ifdef _TEST
            FRIEND_TEST(custom_allocator_test, DefaultCtor);
            FRIEND_TEST(custom_allocator_test, CustomCtor1);
            FRIEND_TEST(custom_allocator_test, CopyCtor);
            FRIEND_TEST(custom_allocator_test, allocate_deallocate);
            FRIEND_TEST(custom_allocator_test, next_reserve);
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

}
