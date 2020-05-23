#ifndef CROSS_REFS_MAP
#define CROSS_REFS_MAP

#include <functional>

template <typename K, typename V, typename Comparator = std::less<K>>
class Map
{

  public:

    class iterator;

    class const_iterator;

    explicit Map(const Comparator & cmp = Comparator());

    Map(const Map & other) = delete;

    Map(Map && other) noexcept;

    Map & operator=(const Map & other) = delete;

    Map & operator=(Map && other) noexcept;

    ~Map();

    void insert(const K & key, const V & value);

    bool contains(const K & key);

    V & operator[](const K & key);

    const V & operator[](const K & key) const;

    iterator begin();

    iterator end();

    const_iterator begin() const;

    const_iterator end() const;

  private:

    struct MapImpl;

    MapImpl impl_;

};


namespace map_details
{

  template <typename K, typename V>
  struct node_t;

  template <typename K, typename V>
  using node_ptr = node_t<K, V> *;

  template <typename K, typename V>
  using const_node_ptr = const node_t<K, V> *;

  enum color_t
  {
    RED = false, BLACK = true
  };

  template <typename K, typename V>
  struct node_t
  {
    K key;
    V value;
    color_t color;
    map_details::node_ptr<K, V> parent;
    map_details::node_ptr<K, V> left;
    map_details::node_ptr<K, V> right;
  };

}

template <typename K, typename V, typename Comparator>
class Map<K, V, Comparator>::iterator
{

  public:

    explicit iterator(map_details::node_ptr<K, V> node) : node_{ node }
    { }

    iterator & operator++()
    {
      move_next();
      return *this;
    }

    iterator operator++(int)
    {
      auto t = *this;
      move_next();
      return t;
    }

    bool operator==(const iterator & rhs) const
    {
      return node_ == rhs.node_;
    }

    bool operator!=(const iterator & rhs) const
    {
      return node_ != rhs.node_;
    }

    K & key()
    {
      return node_->key;
    }

    V & value()
    {
      return node_->value;
    }

  private:

    void move_next()
    {
      if (node_->right) {
        node_ = node_->right;
        while (node_->left) {
          node_ = node_->left;
        }
      } else if (node_->parent && (node_->parent->left == node_)) {
        node_ = node_->parent;
      } else {
        while (node_->parent && (node_->parent->right == node_)) {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
    }

    map_details::node_ptr<K, V> node_;

};

template <typename K, typename V, typename Comparator>
class Map<K, V, Comparator>::const_iterator
{

  public:

    explicit const_iterator(map_details::const_node_ptr<K, V> node) : node_{ node }
    { }

    const_iterator & operator++()
    {
      move_next();
      return *this;
    }

    const_iterator operator++(int)
    {
      auto t = *this;
      move_next();
      return t;
    }

    bool operator==(const const_iterator & rhs) const
    {
      return node_ == rhs.node_;
    }

    bool operator!=(const const_iterator & rhs) const
    {
      return node_ != rhs.node_;
    }

    const K & key()
    {
      return node_->key;
    }

    const V & value()
    {
      return node_->value;
    }

  private:

    void move_next()
    {
      if (node_->right) {
        node_ = node_->right;
        while (node_->left) {
          node_ = node_->left;
        }
      } else if (node_->parent && (node_->parent->left == node_)) {
        node_ = node_->parent;
      } else {
        while (node_->parent && (node_->parent->right == node_)) {
          node_ = node_->parent;
        }
        node_ = node_->parent;
      }
    }

    map_details::const_node_ptr<K, V> node_;

};

template <typename K, typename V, typename Comparator>
struct Map<K, V, Comparator>::MapImpl
{
  map_details::node_ptr<K, V> root;
  Comparator cmp;
};


template <typename K, typename V, typename Comparator>
Map<K, V, Comparator>::Map(const Comparator & cmp) : impl_{ nullptr, cmp }
{ }

template <typename K, typename V, typename Comparator>
Map<K, V, Comparator>::Map(Map && other) noexcept : impl_{ other.impl_ }
{
  other.impl_.root = nullptr;
}

template <typename K, typename V, typename Comparator>
Map<K, V, Comparator> & Map<K, V, Comparator>::operator=(Map && other) noexcept
{
  impl_ = other.impl_;
  other.impl_.root = nullptr;
  return *this;
}

namespace map_details
{
  template <typename K, typename V>
  void recursive_delete(map_details::node_ptr<K, V> node);
}

template <typename K, typename V, typename Comparator>
Map<K, V, Comparator>::~Map()
{
  if (impl_.root) {
    map_details::recursive_delete(impl_.root);
  }
}

namespace map_details
{
  template <typename K, typename V>
  void insert_node(map_details::node_ptr<K, V> node);
}

template <typename K, typename V, typename Comparator>
void Map<K, V, Comparator>::insert(const K & key, const V & value)
{
  auto current = impl_.root;
  auto prev = map_details::node_ptr<K, V>{ nullptr };
  while (current && (current->key != key)) {
    prev = current;
    current = impl_.cmp(current->key, key) ? current->right : current->left;
  }
  if (current) {
    current->value = value;
    return;
  }
  current = new map_details::node_t<K, V>{ key, value, map_details::RED, prev, nullptr, nullptr };
  if (!impl_.root) {
    impl_.root = current;
  } else if (impl_.cmp(prev->key, key)) {
    prev->right = current;
  } else {
    prev->left = current;
  }
  map_details::insert_node(current);
  while (impl_.root->parent) {
    impl_.root = impl_.root->parent;
  }
}

namespace map_details
{
  template <typename K, typename V, typename Comparator>
  map_details::node_ptr<K, V>
  find(const K & key, map_details::node_ptr<K, V> root, const Comparator & cmp = Comparator{ });
}

template <typename K, typename V, typename Comparator>
bool Map<K, V, Comparator>::contains(const K & key)
{
  return map_details::find(key, impl_.root, impl_.cmp);
}

template <typename K, typename V, typename Comparator>
V & Map<K, V, Comparator>::operator[](const K & key)
{
  auto node = map_details::find(key, impl_.root, impl_.cmp);
  if (node) {
    return node->value;
  }
  throw std::invalid_argument{ "No such key in map!" };
}

template <typename K, typename V, typename Comparator>
const V & Map<K, V, Comparator>::operator[](const K & key) const
{
  auto node = map_details::find(key, impl_.root, impl_.cmp);
  if (node) {
    return node->value;
  }
  throw std::invalid_argument{ "No such key in map!" };
}

template <typename K, typename V, typename Comparator>
typename Map<K, V, Comparator>::iterator Map<K, V, Comparator>::begin()
{
  auto c = impl_.root;
  while (c->left) {
    c = c->left;
  }
  return iterator(c);
}

template <typename K, typename V, typename Comparator>
typename Map<K, V, Comparator>::iterator Map<K, V, Comparator>::end()
{
  return iterator(nullptr);
}

template <typename K, typename V, typename Comparator>
typename Map<K, V, Comparator>::const_iterator Map<K, V, Comparator>::begin() const
{
  auto c = impl_.root;
  while (c->left) {
    c = c->left;
  }
  return const_iterator(c);
}

template <typename K, typename V, typename Comparator>
typename Map<K, V, Comparator>::const_iterator Map<K, V, Comparator>::end() const
{
  return const_iterator(nullptr);
}

namespace map_details
{

  template <typename K, typename V>
  void recursive_delete(map_details::node_ptr<K, V> node)
  {
    if (node->left) {
      recursive_delete(node->left);
    }
    if (node->right) {
      recursive_delete(node->right);
    }
    delete node;
  }

  template <typename K, typename V, typename Comparator>
  map_details::node_ptr<K, V> find(const K & key, map_details::node_ptr<K, V> root, const Comparator & cmp)
  {
    auto current = root;
    while (current && (current->key != key)) {
      current = cmp(current->key, key) ? current->right : current->left;
    }
    return current;
  }

  template <typename K, typename V>
  void insert_case_1(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void insert_node(map_details::node_ptr<K, V> node)
  {
    insert_case_1(node);
  }

  template <typename K, typename V>
  void insert_case_2(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void insert_case_1(map_details::node_ptr<K, V> n)
  {
    if (!n->parent) {
      n->color = BLACK;
    } else {
      insert_case_2(n);
    }
  }

  template <typename K, typename V>
  void insert_case_3(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void insert_case_2(map_details::node_ptr<K, V> n)
  {
    if (n->parent->color != BLACK) {
      insert_case_3(n);
    }
  }

  template <typename K, typename V>
  map_details::node_ptr<K, V> grandparent(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  map_details::node_ptr<K, V> uncle(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void insert_case_4(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void insert_case_3(map_details::node_ptr<K, V> n)
  {
    auto u = uncle(n);
    if (u && (u->color == RED)) {
      n->parent->color = BLACK;
      u->color = BLACK;
      auto gp = grandparent(n);
      gp->color = RED;
      insert_case_1(gp);
    } else {
      insert_case_4(n);
    }
  }

  template <typename K, typename V>
  void rotate_left(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void rotate_right(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void insert_case_5(map_details::node_ptr<K, V> n);

  template <typename K, typename V>
  void insert_case_4(map_details::node_ptr<K, V> n)
  {
    auto p = n->parent;
    auto gp = grandparent(n);
    if ((n == p->right) && (p == gp->left)) {
      rotate_left(p);
      n = n->left;
    } else if ((n == p->left) && (p == gp->right)) {
      rotate_right(p);
      n = n->right;
    }
    insert_case_5(n);
  }

  template <typename K, typename V>
  void insert_case_5(map_details::node_ptr<K, V> n)
  {
    auto p = n->parent;
    auto gp = grandparent(n);

    if ((n == p->left) && (p == gp->left)) {
      rotate_right(gp);
    } else {
      rotate_left(gp);
    }

    p->color = BLACK;
    gp->color = RED;
  }

  template <typename K, typename V>
  map_details::node_ptr<K, V> grandparent(map_details::node_ptr<K, V> n)
  {
    return (n && n->parent) ? n->parent->parent : nullptr;
  }

  template <typename K, typename V>
  map_details::node_ptr<K, V> uncle(map_details::node_ptr<K, V> n)
  {
    auto gp = grandparent(n);
    return gp ?
        (gp->left == n->parent) ? gp->right : gp->left
        : nullptr;
  }

  template <typename K, typename V>
  void rotate_left(map_details::node_ptr<K, V> n)
  {
    auto pivot = n->right;
    auto p = n->parent;

    n->right = pivot->left;
    pivot->left = n;
    n->parent = pivot;

    if (n->right) {
      n->right->parent = n;
    }

    if (p) {
      if (p->left == n) {
        p->left = pivot;
      } else {
        p->right = pivot;
      }
    }

    pivot->parent = p;
  }

  template <typename K, typename V>
  void rotate_right(map_details::node_ptr<K, V> n)
  {
    auto pivot = n->left;
    auto p = n->parent;

    n->left = pivot->right;
    pivot->right = n;
    n->parent = pivot;

    if (n->left) {
      n->left->parent = n;
    }

    if (p) {
      if (p->left == n) {
        p->left = pivot;
      } else {
        p->right = pivot;
      }
    }

    pivot->parent = p;
  }

}

#endif
