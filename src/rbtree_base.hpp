// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include <type_traits>

/*
 *define base rbtree node and some type_traits about rbtree node.
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
struct Black {};
struct Red {};
template <class tag>
concept rb_tag = std::is_same_v<tag, Red> || std::is_same_v<tag, Black>;

template <bool> struct black_tag : std::false_type {};
template <> struct black_tag<true> : std::true_type {};

// empty traits
template <class T> struct is_empty : std::false_type {};
template <> struct is_empty<Empty> : std::true_type {};
template <class T> inline constexpr bool is_empty_v = is_empty<T>::value;

// data Tree a = Empty | Tree {left = Tree a, right = Tree a, val = a}
template <is_val val_, rb_tag tag = Black, class Left_ = Empty,
          class Right_ = Empty>
struct Node {
  using Val = val_;
  using RB = tag;
  using Left = Left_;
  using Right = Right_;
};

// node traits
template <class T> struct is_node_impl : std::false_type {};
template <is_val val, rb_tag tag, class L, class R>
struct is_node_impl<Node<val, tag, L, R>> : std::true_type {};
template <class T> inline constexpr bool is_node_v = is_node_impl<T>::value;
template <class T> concept is_tree = is_node_v<T> || is_empty_v<T>;
template <class T> concept is_node = is_node_v<T>;

template <is_tree root>
inline constexpr bool is_black = std::is_same_v<typename root::RB, Black>;
template <> inline constexpr bool is_black<Empty> = true;
template <is_tree root> inline constexpr bool is_red = !is_black<root>;

template <is_tree root>
inline constexpr bool need_fix = is_red<root> &&
                                 (is_red<typename root::Right> ||
                                  is_red<typename root::Left>);
template <> inline constexpr bool need_fix<Empty> = false;
} // namespace wm
