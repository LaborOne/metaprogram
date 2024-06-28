// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include "src/list.hpp"
#include "src/rbtree_base.hpp"
#include <stdint.h>

namespace wm {
/*
 * define rbtree  opeator
 * insert :: (is_val a) => rbtree a -> a -> node a
 * insert_node :: (is_val a) => rbtree a -> node a -> node a
 * height :: (is_tree a) => a ->uint32_t
 * rotate_left :: node a -> node a
 * rotate_right :: node a -> node a
 * list2tree :: list a => node a
 * flat_tree :: (is_tree a) => a -> list a // return  a list as the result of
 * bfs of a rbtree. the empty node would be return too.
 */

template <is_tree node> struct height_impl {
  constexpr static uint32_t val =
      1 + std::max(height_impl<typename node::Left>::val,
                   height_impl<typename node::Right>::val);
};
template <> struct height_impl<Empty> { constexpr static uint32_t val = 0; };

template <is_tree node>
inline constexpr uint32_t height = height_impl<node>::val;
namespace detail {
template <is_tree T> struct unpack_impl {
  using type = List<typename T::Left, List<typename T::Right>>;
};
template <> struct unpack_impl<Empty> {
  using type = List<Empty, List<Empty>>;
};

// unpack_impl :: (Node a) => a -> [a]
template <is_tree T> using unpack = unpack_impl<T>::type;

template <is_list list, uint32_t num> struct flat_tree_impl;
template <is_tree val, is_list next, uint32_t num>
struct flat_tree_impl<List<val, next>, num> {
  using type = join<List<val, next>, List<Enter>,
                    typename flat_tree_impl<map<unpack_impl, List<val, next>>,
                                            num - 1>::type>;
};

template <is_tree val, is_list next> struct flat_tree_impl<List<val, next>, 0> {
  using type = EmptyList;
};

/****************************** insert *************************************/
template <is_tree root, rb_tag tag> struct set_color_impl {
  using type =
      Node<typename root::Val, tag, typename root::Left, typename root::Right>;
};
template <rb_tag tag> struct set_color_impl<Empty, tag> { using type = Empty; };

template <is_tree root, rb_tag tag>
using set_color = set_color_impl<root, tag>::type;

template <is_node> struct rotate_left_impl;
template <is_val val, rb_tag tag, is_tree left, is_node right>
struct rotate_left_impl<Node<val, tag, left, right>> {
  using type =
      Node<typename right::Val, typename right::RB,
           Node<val, tag, left, typename right::Left>, typename right::Right>;
};

template <is_node> struct rotate_right_impl;
template <is_val val, rb_tag tag, is_node left, is_tree right>
struct rotate_right_impl<Node<val, tag, left, right>> {
  using type = Node<typename left::Val, typename left::RB, typename left::Left,
                    Node<val, tag, typename left::Right, right>>;
};
template <is_node node>
using rotate_left = detail::rotate_left_impl<node>::type;
template <is_node node>
using rotate_right = detail::rotate_right_impl<node>::type;

template <bool is_left, bool is_right, is_node root> struct fix_up_detail;
/*
 * case LL 11: set root.color = red, set root.left.color = black, then
 * rotate_right root.
 */
template <is_node root> struct fix_up_detail<true, true, root> {
  using type = rotate_right<
      Node<typename root::Val, Red, set_color<typename root::Left, Black>,
           typename root::Right>>;
};
/*
 * case LR 10: rotate_left root.left to case LL then handle case LL.
 */
template <is_node root> struct fix_up_detail<true, false, root> {
  using type = typename fix_up_detail<
      true, true,
      Node<typename root::Val, typename root::RB,
           rotate_left<typename root::Left>, typename root::Right>>::type;
};
/*
 * clase RR 00: set root.color = red, set root.right.color = black, then
 * rotate_left root.
 */
template <is_node root> struct fix_up_detail<false, false, root> {
  using type = rotate_left<Node<typename root::Val, Red, typename root::Left,
                                set_color<typename root::Right, Black>>>;
};
/*
 * case RL 01: rotate_right root.right to case RR then handle case RR.
 */
template <is_node root> struct fix_up_detail<false, true, root> {
  using type = typename fix_up_detail<
      false, false,
      Node<typename root::Val, typename root::RB, typename root::Left,
           rotate_right<typename root::Right>>>::type;
};

template <bool is_left, is_node root> struct get_node;
template <is_node root> struct get_node<true, root> {
  using type = typename root::Left;
};
template <is_node root> struct get_node<false, root> {
  using type = typename root::Right;
};
template <bool is_left, is_node root> struct fixup_dispath {
  using node = typename get_node<is_left, root>::type;
  static_assert(is_red<node>);
  static_assert(is_red<typename node::Left> ^ is_red<typename node::Right>);
  using type =
      typename fix_up_detail<is_left, is_red<typename node::Left>, root>::type;
};

template <bool fix_left, bool fix_right, bool case_1, is_node root>
struct fixup_impl;
/*
 * case need not fixup : return root,
 */
template <bool case_1, is_node root>
struct fixup_impl<false, false, case_1, root> {
  using type = root;
};
/*
 * case brother node is red : set root.color = red, set root.left.color = black,
 * set root.right.color = red.
 */
template <is_node root> struct fixup_impl<false, true, true, root> {
  using type =
      Node<typename root::Val, Red, set_color<typename root::Left, Black>,
           set_color<typename root::Right, Black>>;
};
/*
 * case brother node is red : set root.color = red, set root.left.color = black,
 * set root.right.color = red.
 */
template <is_node root> struct fixup_impl<true, false, true, root> {
  using type =
      Node<typename root::Val, Red, set_color<typename root::Left, Black>,
           set_color<typename root::Right, Black>>;
};
/*
 * case LL or LR.
 */
template <is_node root> struct fixup_impl<true, false, false, root> {
  using type = typename fixup_dispath<true, root>::type;
};
/*
 * case RR or RL.
 */
template <is_node root> struct fixup_impl<false, true, false, root> {
  using type = typename fixup_dispath<false, root>::type;
};
template <bool fix_left, bool fix_right, bool case_1, is_node root>
using fixup = fixup_impl<fix_left, fix_right, case_1, root>::type;

template <bool is_greater, is_tree root, is_node node> struct insert_detail;
template <is_tree root, is_node node> struct insert_node_impl {
  using ret_type = typename insert_detail<(root::Val::value < node::Val::value),
                                          root, node>::type;
  using type =
      fixup<need_fix<typename ret_type::Left>,
            need_fix<typename ret_type::Right>,
            is_red<typename ret_type::Left> && is_red<typename ret_type::Right>,
            ret_type>;
};
template <is_node node> struct insert_node_impl<Empty, node> {
  using type = node;
};

template <is_tree root, is_node node> struct insert_detail<true, root, node> {
  using type =
      Node<typename root::Val, typename root::RB, typename root::Left,
           typename insert_node_impl<typename root::Right, node>::type>;
};
template <is_tree root, is_node node> struct insert_detail<false, root, node> {
  using type = Node<typename root::Val, typename root::RB,
                    typename insert_node_impl<typename root::Left, node>::type,
                    typename root::Right>;
};
/******************************************************************************/

} // namespace detail

template <is_tree root>
using flat_tree = detail::flat_tree_impl<List<root>, height<root>>::type;

template <is_tree root, is_val val>
using insert = detail::set_color<
    typename detail::insert_node_impl<root, Node<val, Red>>::type, Black>;

template <is_list> struct list2tree_impl;
template <is_val val, is_list next> struct list2tree_impl<List<val, next>> {
  using type = insert<typename list2tree_impl<next>::type, val>;
};
template <is_val val> struct list2tree_impl<List<val, EmptyList>> {
  using type = insert<Empty, val>;
};
template <is_list list> using list2tree = list2tree_impl<list>::type;
} // namespace wm
