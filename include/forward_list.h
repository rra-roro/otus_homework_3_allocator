#pragma once

#include <memory>
#include <iterator>

namespace roro_lib
{
      template <typename T, typename TAlloc = std::allocator<T>>
      class custom_forward_list
      {
            template <typename M> class node_iter;
            template <typename K> struct node;

        public:
            using value_type = T;
            using allocator_type = TAlloc;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;
            using reference = T&;
            using const_reference = const T&;
            using pointer = typename std::allocator_traits<TAlloc>::pointer;
            using const_pointer = typename std::allocator_traits<TAlloc>::const_pointer;
            using iterator = node_iter<T>;
            using const_iterator = node_iter<const T>;

            custom_forward_list() noexcept : nodealloc() {}
            custom_forward_list(const TAlloc& Al) noexcept : nodealloc(Al) {}

            ~custom_forward_list() noexcept(false)
            {
                  while (ptr_begin != ptr_end)
                  {
                        node<T>* next = ptr_begin->next;
                        std::allocator_traits<TNodeAlloc>::destroy(nodealloc, ptr_begin);
                        std::allocator_traits<TNodeAlloc>::deallocate(nodealloc, ptr_begin, 1);
                        ptr_begin = next;
                  }
            }

            void push_front(const T& arg)
            {
                  node<T>* ptr_curent = std::allocator_traits<TNodeAlloc>::allocate(nodealloc, 1);
                  std::allocator_traits<TNodeAlloc>::construct(nodealloc, ptr_curent, arg);
                  ptr_curent->next = ptr_begin;
                  ptr_begin = ptr_curent;
            }

            iterator begin() noexcept
            {
                  return iterator(ptr_begin);
            }

            iterator end() noexcept
            {
                  return iterator(ptr_end);
            }

            const_iterator cbegin() noexcept
            {
                  return const_iterator(ptr_begin);
            }

            const_iterator cend() noexcept
            {
                  return const_iterator(ptr_end);
            }

        private:
            using TNodeAlloc = typename std::allocator_traits<TAlloc>::template rebind_alloc<node<T>>;
            TNodeAlloc nodealloc;

            node<T>* ptr_begin = nullptr;
            node<T>* ptr_end = nullptr;

            template <typename K>
            struct node
            {
                  node(const K& arg) : obj(arg){};
                  K obj;
                  node* next = nullptr;
            };

            template <typename M>
            class node_iter
            {
                  node<M>* current_node;

              public:
                  using iterator_category = std::forward_iterator_tag;
                  using value_type = M;
                  using difference_type = ptrdiff_t;
                  using pointer = M*;
                  using reference = T&;

                  node_iter() noexcept : current_node(nullptr) {}
                  node_iter(node<M>* node) noexcept : current_node(node) {}
                  template <typename U>
                  node_iter(const node_iter<U>& iter) noexcept : current_node(iter.current_node) {}

                  template <typename U>
                  node_iter& operator=(const node_iter<U>& iter) noexcept
                  {
                        if (this == &iter)
                        {
                              return *this;
                        }
                        current_node = iter.current_node;
                        return *this;
                  }

                  T* operator->()
                  {
                        return &current_node->obj;
                  }

                  T& operator*() noexcept
                  {
                        return current_node->obj;
                  }

                  template <typename U>
                  bool operator==(const node_iter<U>& iter) const noexcept
                  {
                        return current_node == iter.current_node;
                  }

                  template <typename U>
                  bool operator!=(const node_iter<U>& iter) const noexcept
                  {
                        return current_node != iter.current_node;
                  }

                  node_iter& operator++() noexcept
                  {
                        if (current_node != nullptr)
                              current_node = current_node->next;
                        return *this;
                  }

                  node_iter operator++(int)
                  {
                        node_iter old_iter = *this;
                        if (current_node != nullptr)
                              current_node = current_node->next;
                        return old_iter;
                  }
            };
      };
}
