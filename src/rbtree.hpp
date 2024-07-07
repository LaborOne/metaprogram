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

template <is_tree root, rb_tag tag> struct set_color_impl {
  using type =
      Node<typename root::Val, tag, typename root::Left, typename root::Right>;
};
template <rb_tag tag> struct set_color_impl<Empty, tag> { using type = Empty; };

template <is_tree root, rb_tag tag>
using set_color = set_color_impl<root, tag>::type;

/****************************** insert *************************************/

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
template <is_node node> using rotate_left = rotate_left_impl<node>::type;
template <is_node node> using rotate_right = rotate_right_impl<node>::type;

template <bool is_left, bool is_right, is_node root> struct insert_fixup_detail;
/*
 * case LL 11: set root.color = red, set root.left.color = black, then
 * rotate_right root.
 */
template <is_node root> struct insert_fixup_detail<true, true, root> {
  using type = rotate_right<
      Node<typename root::Val, Red, set_color<typename root::Left, Black>,
           typename root::Right>>;
};
/*
 * case LR 10: rotate_left root.left to case LL then handle case LL.
 */
template <is_node root> struct insert_fixup_detail<true, false, root> {
  using type = typename insert_fixup_detail<
      true, true,
      Node<typename root::Val, typename root::RB,
           rotate_left<typename root::Left>, typename root::Right>>::type;
};
/*
 * clase RR 00: set root.color = red, set root.right.color = black, then
 * rotate_left root.
 */
template <is_node root> struct insert_fixup_detail<false, false, root> {
  using type = rotate_left<Node<typename root::Val, Red, typename root::Left,
                                set_color<typename root::Right, Black>>>;
};
/*
 * case RL 01: rotate_right root.right to case RR then handle case RR.
 */
template <is_node root> struct insert_fixup_detail<false, true, root> {
  using type = typename insert_fixup_detail<
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
template <bool is_left, is_node root> struct insert_fixup_dispatch {
  using node = typename get_node<is_left, root>::type;
  static_assert(is_red<node>);
  static_assert(is_red<typename node::Left> ^ is_red<typename node::Right>);
  using type =
      typename insert_fixup_detail<is_left, is_red<typename node::Left>,
                                   root>::type;
};

template <bool fix_left, bool fix_right, bool case_1, is_node root>
struct insert_fixup_impl;
/*
 * case need not fixup : return root,
 */
template <bool case_1, is_node root>
struct insert_fixup_impl<false, false, case_1, root> {
  using type = root;
};
/*
 * case brother node is red : set root.color = red, set root.left.color = black,
 * set root.right.color = red.
 */
template <is_node root> struct insert_fixup_impl<false, true, true, root> {
  using type =
      Node<typename root::Val, Red, set_color<typename root::Left, Black>,
           set_color<typename root::Right, Black>>;
};
/*
 * case brother node is red : set root.color = red, set root.left.color = black,
 * set root.right.color = red.
 */
template <is_node root> struct insert_fixup_impl<true, false, true, root> {
  using type =
      Node<typename root::Val, Red, set_color<typename root::Left, Black>,
           set_color<typename root::Right, Black>>;
};
/*
 * case LL or LR.
 */
template <is_node root> struct insert_fixup_impl<true, false, false, root> {
  using type = typename insert_fixup_dispatch<true, root>::type;
};
/*
 * case RR or RL.
 */
template <is_node root> struct insert_fixup_impl<false, true, false, root> {
  using type = typename insert_fixup_dispatch<false, root>::type;
};
template <bool fix_left, bool fix_right, bool case_1, is_node root>
using fixup = insert_fixup_impl<fix_left, fix_right, case_1, root>::type;

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
/********************************* delete *************************************/
template <class> constexpr bool dependent_false = false;

template <bool is_left, bool is_w_left_red, bool is_w_right_red, is_node root>
struct delete_fixup_impl;
/*
 * case 2 x(left) is black and w(right) is black. w(right).left is black,
 * w(right).right is black then w(right).color = red. p.color += black.
 */
template <is_node root> struct delete_fixup_impl<true, false, false, root> {
  using color = Black;
  using node = Node<typename root::Val, typename root::RB, typename root::Left,
                    set_color<typename root::Right, Red>>;
};
/*
 * case 2 x(right) is black and w(left) is black. w(left).left is black,
 * w(left).right is black then w(left).color = red. p.color += black.
 */
template <is_node root> struct delete_fixup_impl<false, false, false, root> {
  using node = Node<typename root::Val, typename root::RB,
                    set_color<typename root::Left, Red>, typename root::Right>;
  using color = Black;
};
/*
 * case 3 x(root.left) is black, w(root.right) is black, w(root.right).left is
 * red, w(root.right).right is black. then w(root.right).color = red,
 * w(root.right).left.color = black., rotate_right w(root.right), turn to
 * case 4.
 */
template <is_node root> struct delete_fixup_impl<true, true, false, root> {
  using new_w_left = set_color<typename root::Right::Left, Black>;
  using new_w = rotate_right<Node<typename root::Right::Val, Red, new_w_left,
                                  typename root::Right::Right>>;
  static_assert(is_black<new_w> && is_red<typename new_w::Right>,
                "before turn to case 4, root.right should be black, "
                "root.right.right should be red");
  using new_root =
      Node<typename root::Val, typename root::RB, typename root::Left, new_w>;
  using fix = delete_fixup_impl<true, is_red<typename root::Right::Left>,
                                is_red<typename root::Right::Right>, new_root>;
  using node = typename fix::node;
  using color = Red;
};
/*
 * case 3 x(root.right) is black, w(root.left) is black, w(root.left).right is
 * red, w(root.left).left is black. then w(root.left).color = red,
 * w(root.left).right.color = black., rotate_left w(root.left),turn to case 4.
 */
template <is_node root> struct delete_fixup_impl<false, false, true, root> {
  using new_w_right = set_color<typename root::Left::Right, Black>;
  using new_w = rotate_right<Node<typename root::Left::Val, Red,
                                  typename root::Left::Left, new_w_right>>;
  static_assert(is_black<new_w> && is_red<typename new_w::Left>,
                "before turn to case 4, root.left should be black, "
                "root.left.left should be red");
  using new_root =
      Node<typename root::Val, typename root::RB, new_w, typename root::Right>;
  using fix = delete_fixup_impl<false, is_red<typename root::Left::Left>,
                                is_red<typename root::Left::Right>, new_root>;
  using node = typename fix::node;
  using color = Red;
};
/*
 * case 4 x(root.left) is black, w(root.right) is black, w(root.right).right is
 * red. then swap_color root w(root.right), w(root.right).right = black,
 * rotate_left root, set color = Red.
 */
template <is_node root> struct delete_fixup_impl<true, true, true, root> {
  using new_w = Node<typename root::Right::Val, typename root::RB,
                     typename root::Right::Left,
                     set_color<typename root::Right::Right, Black>>;
  using new_root = Node<typename root::Val, Black, typename root::Left, new_w>;
  using node = rotate_left<new_root>;
  using color = Red;
};
/*
 * case 4 x(root.left) is black, w(root.right) is black, w(root.right).right is
 * red. then swap_color root w(root.right), w(root.right).right = black,
 * rotate_left root, set color = Red.
 */
template <is_node root> struct delete_fixup_impl<true, false, true, root> {
  using new_w = Node<typename root::Right::Val, typename root::RB,
                     typename root::Right::Left,
                     set_color<typename root::Right::Right, Black>>;
  using new_root = Node<typename root::Val, Black, typename root::Left, new_w>;
  using node = rotate_left<new_root>;
  using color = Red;
};
/*
 * case 4 x(root.right) is black, w(root.left) is black, w(root.left).left is
 * red. then swap_color root w(root.left), w(root.left).left = black,
 * rotate_right root, set color = Red.
 */
template <is_node root> struct delete_fixup_impl<false, true, false, root> {
  using new_w = Node<typename root::Left::Val, typename root::RB,
                     set_color<typename root::Left::Left, Black>,
                     typename root::Left::Right>;
  using new_root = Node<typename root::Val, Black, new_w, typename root::Right>;
  using node = rotate_right<new_root>;
  using color = Red;
};
/*
 * case 4 x(root.right) is black, w(root.left) is black, w(root.left).left is
 * red. then swap_color root w(root.left), w(root.left).left = black,
 * rotate_right root, set color = Red.
 */
template <is_node root> struct delete_fixup_impl<false, true, true, root> {
  using new_w = Node<typename root::Left::Val, typename root::RB,
                     set_color<typename root::Left::Left, Black>,
                     typename root::Left::Right>;
  using new_root = Node<typename root::Val, Black, new_w, typename root::Right>;
  using node = rotate_right<new_root>;
  using color = Red;
};

template <bool is_left, bool is_x_red, bool is_w_red, is_node root>
struct delete_fixup_dispatch;

template <bool is_left, rb_tag tag, is_node root> struct delete_fixup;
template <is_node root> struct delete_fixup<true, Red, root> {
  using color = Red;
  using node = root;
};
template <is_node root> struct delete_fixup<false, Red, root> {
  using color = Red;
  using node = root;
};

template <bool is_left, bool is_next_red, is_node root>
struct delete_fix_when_next_is_red;
template <is_node root> struct delete_fix_when_next_is_red<true, true, root> {
  using node =
      Node<typename root::Val, typename root::RB,
           set_color<typename root::Left, Black>, typename root::Right>;
  using color = Red;
};
template <is_node root> struct delete_fix_when_next_is_red<false, true, root> {
  using node = Node<typename root::Val, typename root::RB, typename root::Left,
                    set_color<typename root::Right, Black>>;
  using color = Red;
};
template <is_node root> struct delete_fix_when_next_is_red<true, false, root> {
  using res = delete_fixup_dispatch<true, is_red<typename root::Left>,
                                    is_red<typename root::Right>, root>;
  using color = typename res::color;
  using node = typename res::node;
};
template <is_node root> struct delete_fix_when_next_is_red<false, false, root> {
  using res = delete_fixup_dispatch<false, is_red<typename root::Left>,
                                    is_red<typename root::Right>, root>;
  using color = typename res::color;
  using node = typename res::node;
};
template <is_node root> struct delete_fixup<true, Black, root> {
  using res =
      delete_fix_when_next_is_red<true, is_red<typename root::Left>, root>;
  using color = typename res::color;
  using node = typename res::node;
};
template <is_node root> struct delete_fixup<false, Black, root> {
  using res =
      delete_fix_when_next_is_red<false, is_red<typename root::Right>, root>;
  using color = typename res::color;
  using node = typename res::node;
};

template <is_node root> struct delete_fixup_dispatch<true, true, true, root> {
  using node =
      Node<typename root::Val, typename root::RB,
           set_color<typename root::Left, Black>, typename root::Right>;
  using color = Red;
};
template <is_node root> struct delete_fixup_dispatch<true, true, false, root> {
  using node =
      Node<typename root::Val, typename root::RB,
           set_color<typename root::Left, Black>, typename root::Right>;
  using color = Red;
};

template <is_node root> struct delete_fixup_dispatch<false, true, true, root> {
  using node = Node<typename root::Val, typename root::RB, typename root::Left,
                    set_color<typename root::Right, Black>>;
  using color = Red;
};
template <is_node root> struct delete_fixup_dispatch<false, true, false, root> {
  using node = Node<typename root::Val, typename root::RB, typename root::Left,
                    set_color<typename root::Right, Black>>;
  using color = Red;
};
/*
 * case 1 x is black and w is red. x is root.left, w is root.right.
 * fisrtly, rotate_left root; secondly, fixupdelete new_root.left(the new parent
 * of x), thirdly, fixupdelete new_new_root.
 */
template <is_node root> struct delete_fixup_dispatch<true, false, true, root> {
  static_assert(is_black<root>, "when root.right is red, root must be black");
  static_assert(is_black<typename root::Right::Left> &&
                    is_black<typename root::Right::Right>,
                "when left is black, right is red, right.left.color must be "
                "black, right.right.color must be black.");
  using new_w = set_color<typename root::Right, Black>;
  using rotate_root =
      rotate_left<Node<typename root::Val, Red, typename root::Left, new_w>>;
  using new_root = delete_fixup<true, Black, typename rotate_root::Left>;
  using new_new_root =
      Node<typename rotate_root::Val, typename rotate_root::RB,
           typename new_root::node, typename rotate_root::Right>;
  using fix = delete_fixup<true, typename new_root::color, new_new_root>;
  using node = typename fix::node;
  using color = node::color;
};
/*
 * case 1 x is black and w is red.
 */
template <is_node root> struct delete_fixup_dispatch<false, false, true, root> {
  static_assert(is_black<root>, "when root.left is red, root must be black");
  static_assert(is_black<typename root::Left::Left> &&
                    is_black<typename root::Left::Right>,
                "when right is black, left is red, left.left.color must be "
                "black, left.right.color must be black.");
  using new_w = set_color<typename root::Left, Black>;
  using rotate_root =
      rotate_right<Node<typename root::Val, Red, new_w, typename root::Right>>;
  using new_root = delete_fixup<false, Black, typename rotate_root::Right>;
  using new_new_root =
      Node<typename rotate_root::Val, typename rotate_root::RB,
           typename rotate_root::Left, typename new_root::node>;
  using fix = delete_fixup<false, typename new_root::color, new_new_root>;
  using node = typename fix::node;
  using color = fix::color;
};

template <is_node root> struct delete_fixup_dispatch<true, false, false, root> {
  static_assert(!is_empty_v<typename root::Right>,
                "when x is black, w cannot be empty");
  using res = delete_fixup_impl<true, is_red<typename root::Right::Left>,
                                is_red<typename root::Right::Right>, root>;
  using node = typename res::node;
  using color = typename res::color;
};

template <is_node root>
struct delete_fixup_dispatch<false, false, false, root> {
  static_assert(!is_empty_v<typename root::Left>,
                "when x is black, w cannot be empty");
  using res = delete_fixup_impl<false, is_red<typename root::Left::Left>,
                                is_red<typename root::Left::Right>, root>;
  using node = typename res::node;
  using color = typename res::color;
};

template <is_node root, is_tree left> struct get_successor_impl {
  using successor = get_successor_impl<left, typename left::Left>;
  using y = typename successor::y;
  using new_root = Node<typename root::Val, typename root::RB,
                        typename successor::node, typename root::Right>;
  using after_fix = delete_fixup<true, typename successor::color, new_root>;
  using node = typename after_fix::node;
  using color = typename after_fix::color;
};

template <is_node root> struct get_successor_impl<root, Empty> {
  using y = root;
  using node = typename root::Right;
  using color = typename root::RB;
};
template <is_node root> struct get_successor {
  using node = get_successor_impl<root, typename root::Left>;
};
template <is_node root, is_tree left, is_tree right> struct delete_node_impl {
  static_assert(!is_empty_v<right>,
                "root.right is empty when get successor of root.");
  using successor = typename get_successor<right>::node;
  using y = typename successor::y;
  using new_root =
      Node<typename y::Val, typename root::RB, left, typename successor::node>;
  using after_fix = delete_fixup<false, typename successor::color, new_root>;
  using color = typename after_fix::color;
  using node = typename after_fix::node;
};

template <is_node root> struct delete_node_impl<root, Empty, Empty> {
  using node = Empty;
  using color = root::RB;
};
template <is_node root, is_tree left>
struct delete_node_impl<root, left, Empty> {
  using node = left;
  using color = root::RB;
};
template <is_node root, is_tree right>
struct delete_node_impl<root, Empty, right> {
  using node = right;
  using color = root::RB;
};

template <bool greater, bool less> struct get_cmp_tag;
template <> struct get_cmp_tag<false, false> {
  constexpr static int32_t tag = 0;
};
template <> struct get_cmp_tag<true, false> {
  constexpr static int32_t tag = 1;
};
template <> struct get_cmp_tag<false, true> {
  constexpr static int32_t tag = -1;
};

// template<> struct get_cmp_tag<true, true>{static_assert(false, "ARB is true
// && BRA is true, cannot cmpare.");};
template <int32_t cmp_tag, is_tree root, is_val val>
struct delete_node_dispatch;
template <is_tree root, is_val val> struct delete_node_detail {
  using node_data =
      delete_node_dispatch<get_cmp_tag<(root::Val::value < val::value),
                                       (root::Val::value > val::value)>::tag,
                           root, val>;
  using type = typename node_data::node;
};
template <is_val val> struct delete_node_detail<Empty, val> {
  static_assert(dependent_false<val>, "delete an val from empty node.");
};
template <is_tree root, is_val val> struct delete_node_dispatch<1, root, val> {
  using after_delete_node =
      typename delete_node_detail<typename root::Right, val>::node_data;
  using new_root = Node<typename root::Val, typename root::RB,
                        typename root::Left, typename after_delete_node::node>;
  // delete_fixup
  using after_fixup =
      delete_fixup<false, typename after_delete_node::color, new_root>;
  using node = typename after_fixup::node;
  using color = typename after_fixup::color;
};
template <is_tree root, is_val val> struct delete_node_dispatch<-1, root, val> {
  using after_delete_node =
      typename delete_node_detail<typename root::Left, val>::node_data;
  using new_root = Node<typename root::Val, typename root::RB,
                        typename after_delete_node::node, typename root::Right>;
  // delete_fixup
  using after_fixup =
      delete_fixup<true, typename after_delete_node::color, new_root>;
  using node = typename after_fixup::node;
  using color = typename after_fixup::color;
};

template <is_tree root, is_val val> struct delete_node_dispatch<0, root, val> {
  using after_delete_node =
      delete_node_impl<root, typename root::Left, typename root::Right>;
  using node = typename after_delete_node::node;
  using color = typename after_delete_node::color;
};
/******************************************************************************/
} // namespace detail

template <is_tree root>
using flat_tree = detail::flat_tree_impl<List<root>, height<root>>::type;

template <is_tree root, is_val val>
using insert = detail::set_color<
    typename detail::insert_node_impl<root, Node<val, Red>>::type, Black>;

template <is_tree root, is_val val>
using delete_node =
    detail::set_color<typename detail::delete_node_detail<root, val>::type,
                      Black>;
template <is_list> struct list2tree_impl;
template <is_val val, is_list next> struct list2tree_impl<List<val, next>> {
  using type = insert<typename list2tree_impl<next>::type, val>;
};
template <is_val val> struct list2tree_impl<List<val, EmptyList>> {
  using type = insert<Empty, val>;
};
template <is_list list> using list2tree = list2tree_impl<list>::type;
} // namespace wm
