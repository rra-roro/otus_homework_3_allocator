#pragma once

#include <memory>
#include <iterator>

template <typename T>
struct Node
{
      Node(const T& arg) : obj(arg){};
      T obj;
      Node* next = nullptr;
};

template <typename T>
class node_iter
{
      Node<T>* current_node;

  public:
      using iterator_category = std::forward_iterator_tag;
      using value_type = T;
      using difference_type = ptrdiff_t;
      using pointer = T*;
      using reference = T&;

      node_iter() noexcept : current_node(nullptr) {}
      node_iter(Node<T>* node) noexcept : current_node(node) {}
      template <typename U>
      node_iter(const node_iter<U>& It) noexcept : current_node(It.current_node) {}

      template <typename U>
      node_iter& operator=(const node_iter<U>& It)
      {
            if (this == &It)
            {
                  return *this;
            }
            current_node = It.current_node;
            return *this;
      }

      T* operator->()
      {
            return &current_node->obj;
      }

      T& operator*()
      {
            return current_node->obj;
      }

      template <typename U>
      bool operator==(const node_iter<U>& It)
      {
            return current_node == It.current_node;
      }

      template <typename U>
      bool operator!=(const node_iter<U>& It)
      {
            return current_node != It.current_node;
      }

      node_iter& operator++()
      {
            if (current_node != nullptr)
                  current_node = current_node->next;
            return *this;
      }

      node_iter operator++(int)
      {
            node_iter oldIt = *this;
            if (current_node != nullptr)
                  current_node = current_node->next;
            return oldIt;
      }
};

template <typename T, typename TAlloc = std::allocator<T>>
class custom_forward_list
{
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

  private:
      using TNodeAlloc = typename std::allocator_traits<TAlloc>::template rebind_alloc<Node<T>>;
      TNodeAlloc nodealloc;

      Node<T>* ptr_begin = nullptr;
      Node<T>* ptr_end = nullptr;

  public:
      custom_forward_list() noexcept : nodealloc() {}
      custom_forward_list(const TAlloc& Al) noexcept : nodealloc(Al) {}

      ~custom_forward_list()
      {
            while(ptr_begin != ptr_end)
            {
                  Node<T>* next = ptr_begin->next;
                  std::allocator_traits<TNodeAlloc>::destroy(nodealloc, ptr_begin);
                  std::allocator_traits<TNodeAlloc>::deallocate(nodealloc, ptr_begin, 1);
                  ptr_begin = next;
            }
      }

      void push_front(const T& arg)
      {
            Node<T>* ptr_curent = std::allocator_traits<TNodeAlloc>::allocate(nodealloc, 1);
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
};
