#pragma once
#include "src/list.hpp"
#include "src/tree_base.hpp"
#include <stdint.h>

namespace wm {
/*
 * define tree  opeator
 * insert :: (is_val a) => node a -> a -> node a
 * height :: (is_node a) => a ->uint32_t
 * list2tree :: list a => node a
 * flat_tree :: (is_node a) => a -> tuple a // return  a tuple as the result of
 * bfs of a tree. the empty node would be return.
 */

template <is_node T> constexpr uint32_t height() {
  if constexpr (is_empty_v<T>) {
    return 0;
  } else {
    return std::max(height<typename T::Left>(), height<typename T::Right>()) +
           1;
  }
}
namespace detail {
template <is_val T> struct PrimeNode { using type = Node<T>; };
template <is_val T> using Primeis_nodet = typename PrimeNode<T>::type;
template <is_node Root, is_val Val> struct insert_impl {
  using type = std::conditional_t<
      Val::value <= Root::Val::value,
      Node<typename Root::Val, true,
           typename insert_impl<typename Root::Left, Val>::type,
           typename Root::Right>,
      Node<typename Root::Val, true, typename Root::Left,
           typename insert_impl<typename Root::Right, Val>::type>>;
};
template <is_val Val> struct insert_impl<Empty, Val> {
  using type = Primeis_nodet<Val>;
};

template <is_node T> struct unpack_impl {
  using type = List<typename T::Left, List<typename T::Right>>;
};
template <> struct unpack_impl<Empty> {
  using type = List<Empty, List<Empty>>;
};
// unpack_impl :: (Node a) => a -> [a]
template <is_node T> using unpack = unpack_impl<T>::type;

template <class T> struct unpack_list_of_node_impl;
template <> struct unpack_list_of_node_impl<EmptyList> {
  using type = EmptyList;
};
template <is_node val, is_list next>
struct unpack_list_of_node_impl<List<val, next>> {
  using root = List<val, next>;
  using init = unpack<val>;
  using type = join<init, typename unpack_list_of_node_impl<next>::type>;
};
template <class T>
using unpack_list_of_node = unpack_list_of_node_impl<T>::type;

template <class, uint32_t> struct flat_tree_impl_detail;
template <is_node val, is_list next>
struct flat_tree_impl_detail<List<val, next>, 0> {
  using type = std::tuple<>;
};
template <is_node val, is_list next, uint32_t Num>
struct flat_tree_impl_detail<List<val, next>, Num> {
  using type =
      tuple_cat<list2tuple<List<val, next>>, std::tuple<Enter>,
                typename flat_tree_impl_detail<
                    unpack_list_of_node<List<val, next>>, Num - 1>::type>;
};
template <class T, uint32_t Num>
using flat_tree_impl_t = flat_tree_impl_detail<T, Num>::type;
template <is_node root> struct flat_tree_impl {
  using type =
      typename detail::flat_tree_impl_detail<List<root>, height<root>()>::type;
};

template <is_list> struct list2tree_impl;
template <is_val val, is_list next> struct list2tree_impl<List<val, next>> {
  using type =
      typename insert_impl<typename list2tree_impl<next>::type, val>::type;
};

template <is_val val> struct list2tree_impl<List<val, EmptyList>> {
  using type = Node<val>;
};
} // namespace detail

template <is_node Root, is_val Val>
using insert = detail::insert_impl<Root, Val>::type;

template <is_node root> using flat_tree = detail::flat_tree_impl<root>::type;

template <is_list T> using list2tree = detail::list2tree_impl<T>::type;
} // namespace wm
