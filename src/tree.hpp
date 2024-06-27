// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include "src/algorithm.hpp"
#include "src/list.hpp"
#include "src/tree_base.hpp"
#include <stdint.h>

namespace wm {
/*
 * define tree  opeator
 * insert :: (is_val a) => node a -> a -> node a
 * height :: (is_node a) => a ->uint32_t
 * list2tree :: list a => node a
 * flat_tree :: (is_node a) => a -> list a // return  a list as the result of
 * bfs of a tree. the empty node would be return too.
 */

template <is_node node> struct height_impl {
  constexpr static uint32_t val =
      1 + std::max(height_impl<typename node::Left>::val,
                   height_impl<typename node::Right>::val);
};
template <> struct height_impl<Empty> { constexpr static uint32_t val = 0; };

template <is_node node>
inline constexpr uint32_t height = height_impl<node>::val;
namespace detail {
template <is_val T> struct PrimeNode { using type = Node<T>; };
template <is_val T> using Primeis_node_t = typename PrimeNode<T>::type;
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
  using type = Primeis_node_t<Val>;
};

template <is_node T> struct unpack_impl {
  using type = List<typename T::Left, List<typename T::Right>>;
};
template <> struct unpack_impl<Empty> {
  using type = List<Empty, List<Empty>>;
};
// unpack_impl :: (Node a) => a -> [a]
template <is_node T> using unpack = unpack_impl<T>::type;

template <is_list list, uint32_t num> struct flat_tree_impl;
template <is_node val, is_list next, uint32_t num>
struct flat_tree_impl<List<val, next>, num> {
  using type = join<List<val, next>, List<Enter>,
                    typename flat_tree_impl<map<unpack_impl, List<val, next>>,
                                            num - 1>::type>;
};

template <is_node val, is_list next> struct flat_tree_impl<List<val, next>, 0> {
  using type = EmptyList;
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

template <is_node root>
using flat_tree = detail::flat_tree_impl<List<root>, height<root>>::type;
template <is_list T> using list2tree = detail::list2tree_impl<T>::type;
} // namespace wm
