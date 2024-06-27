// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include <type_traits>

/*
 *define base tree node and some type_traits about tree node.
 */
namespace wm {
struct Empty;

// val traits
template <class T, class = void> struct is_val_impl : std::false_type {};
template <class T>
struct is_val_impl<
    T, std::void_t<decltype(std::declval<T>().value), typename T::value_type>>
    : std::true_type {};
template <class T> inline constexpr bool is_val_type_v = is_val_impl<T>::value;
template <class T> concept is_val = is_val_type_v<T>;
// black_tag define
template <bool> struct black_tag : std::false_type {};
template <> struct black_tag<true> : std::true_type {};

// empty traits
template <class T> struct is_empty : std::false_type {};
template <> struct is_empty<Empty> : std::true_type {};
template <class T> inline constexpr bool is_empty_v = is_empty<T>::value;

// node traits
template <is_val, bool, class, class> struct Node;
template <class T> struct is_node_impl : std::false_type {};
template <is_val val, bool is_black, class L, class R>
struct is_node_impl<Node<val, is_black, L, R>> : std::true_type {};
template <class T> inline constexpr bool is_node_v = is_node_impl<T>::value;
template <class T> concept is_node = is_node_v<T> || is_empty_v<T>;

// data Tree a = Empty | Tree {left = Tree a, right = Tree a, val = a}
template <is_val val_, bool is_black = true, class Left_ = Empty,
          class Right_ = Empty>
struct Node {
  using Val = val_;
  using Left = Left_;
  using Right = Right_;
  using Black = black_tag<is_black>;
};

template <is_node Root>
inline constexpr bool is_black_v = Root::Black::value || is_empty_v<Root>;
template <class Root> concept is_black = is_black_v<Root>;
template <class Root> concept is_red = !is_black<Root>;

} // namespace wm
