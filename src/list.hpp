#ifndef CROSS_REFS_LIST
#define CROSS_REFS_LIST

#include <stdexcept>
#include <functional>
#include <initializer_list>

template <typename T>
class List
{

  public:

    class iterator;

    class const_iterator;

    List();

    List(const List & other);

    List(List && other) noexcept;

    List(std::initializer_list<T> list);

    ~List();

    List<T> & operator=(const List & other);

    List<T> & operator=(List && other) noexcept;

    void push_back(const T & value);

    void peek_front();

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

  private:

    struct ListImpl;

    ListImpl impl;

};

namespace list_details
{
  template <typename T>
  struct node_t;

  template <typename T>
  using node_ptr = node_t<T> *;

  template <typename T>
  using const_node_ptr = node_t<const T> *;

  template <typename T>
  struct node_t
  {
    T value;
    node_ptr<T> next;
  };
}

template <typename T>
class List<T>::iterator
{
  public:

    iterator() : node{ nullptr }
    { }

    iterator(list_details::node_ptr<T> node) : node{ node }
    { }

    iterator & operator++()
    {
      node = node->next;
      return *this;
    }

    iterator operator++(int)
    {
      auto t = *this;
      node = node->next;
      return t;
    }

    T & operator*() const
    {
      return node->value;
    }

    bool operator==(const iterator & other) const
    {
      return node == other.node;
    }

    bool operator!=(const iterator & other) const
    {
      return node != other.node;
    }

  private:
    list_details::node_ptr<T> node;
};


template <typename T>
class List<T>::const_iterator
{
  public:

    const_iterator() : node{ nullptr }
    { }

    const_iterator(list_details::const_node_ptr<T> node) : node{ node }
    { }

    const_iterator & operator++()
    {
      node = node->next;
      return *this;
    }

    const_iterator operator++(int)
    {
      auto t = *this;
      node = node->next;
      return t;
    }

    const T & operator*() const
    {
      return node->value;
    }

    bool operator==(const const_iterator & other) const
    {
      return node == other.node;
    }

    bool operator!=(const const_iterator & other) const
    {
      return node != other.node;
    }

  private:
    list_details::const_node_ptr<T> node;
};


template <typename T>
struct List<T>::ListImpl
{
  list_details::node_ptr<T> head;
};

template <typename T>
List<T>::List() :
    impl{ nullptr }
{ }

template <typename T>
List<T>::List(const List & other) :
    impl{ nullptr }
{
  if (!other.impl.head) {
    return;
  }
  auto tail = impl.head = new list_details::node_t<T>{ other.impl.head->value, nullptr };
  for (auto itr = other.impl.head->next; itr; itr = itr->next) {
    tail = tail->next = new list_details::node_t<T>{ itr->value, nullptr };
  }
}

template <typename T>
List<T>::List(List && other) noexcept :
    impl{ other.impl }
{
  other.impl = { nullptr };
}

template <typename T>
List<T>::List(std::initializer_list<T> list) :
    impl{ nullptr }
{
  auto itr = list.begin();
  auto tail = impl.head = new list_details::node_t<T>{ *itr, nullptr };
  for (++itr; itr != list.end(); ++itr) {
    tail = tail->next = new list_details::node_t<T>{ *itr, nullptr };
  }
}

namespace list_details
{
  template <typename T>
  void destructList(node_ptr<T> begin);
}

template <typename T>
List<T>::~List()
{
  if (!impl.head) {
    return;
  }
  list_details::destructList(impl.head);
}

template <typename T>
List<T> & List<T>::operator=(const List & other)
{
  if (this == &other) {
    return *this;
  }
  list_details::destructList(impl.head);
  auto tail = impl.head = new list_details::node_t<T>{ other.impl.head->value, nullptr };
  for (auto itr = other.impl.head->next; itr; itr = itr->next) {
    tail = tail->next = new list_details::node_t<T>{ itr->value, nullptr };
  }
  return *this;
}

template <typename T>
List<T> & List<T>::operator=(List && other) noexcept
{
  list_details::destructList(impl.head);
  impl = other.impl;
  other.impl = { nullptr };
  return *this;
}

template <typename T>
void List<T>::push_back(const T & value)
{
  if (!impl.head) {
    impl.head = new list_details::node_t<T>{ value, nullptr };
    return;
  }
  if (impl.head->value == value) {
    return;
  }
  auto tail = impl.head;
  while (tail->next) {
    if (tail->value == value) {
      return;
    }
    tail = tail->next;
  }
  if (tail->value == value) {
      return;
  }
  tail->next = new list_details::node_t<T>{ value, nullptr };
}

template <typename T>
void List<T>::peek_front()
{
  if (!impl.head) {
    throw std::out_of_range{ "Can't peek at empty list" };
  }
  return impl.head->value;
}

template <typename T>
typename List<T>::iterator List<T>::begin()
{
  return iterator(impl.head);
}

template <typename T>
typename List<T>::iterator List<T>::end()
{
  return iterator();
}

template <typename T>
typename List<T>::const_iterator List<T>::begin() const
{
  return const_iterator(impl.head);
}

template <typename T>
typename List<T>::const_iterator List<T>::end() const
{
  return const_iterator();
}

namespace list_details
{
  template <typename T>
  void destructList(node_ptr<T> begin)
  {
    for (auto r = begin; begin; r = begin) {
      begin = begin->next;
      delete r;
    }
  }
}


#endif
