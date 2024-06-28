// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include "algorithm.hpp"
#include "src/list_base.hpp"
#include "src/rbtree_base.hpp"
#include <iostream>
#include <tuple>

/*
 * define the printer of class.
 * print_list
 * print_tuple
 */
namespace wm {
template <class T> struct print;
template <> struct print<Red> {
  void operator()() { std::cout << "r "; }
};
template <> struct print<Black> {
  void operator()() { std::cout << "b "; }
};
template <is_val val, rb_tag tag, is_tree left, is_tree right>
struct print<Node<val, tag, left, right>> {
  void operator()() {
    std::cout << val::value << "-";
    print<tag>{}();
  }
};
template <> struct print<Empty> {
  void operator()() {
    std::cout << "nil-";
    print<Black>{}();
  }
};
template <is_val val> struct print<val> {
  void operator()() { std::cout << val::value << " "; }
};
template <is_tree val> struct print<val> {
  void operator()() { print<val>{}(); }
};
template <class node, is_list next> struct print<List<node, next>> {
  void operator()() { print<node>{}(); }
};
template <> struct print<EmptyList> {
  void operator()() { std::cout << "\n"; }
};
struct Enter;
template <> struct print<Enter> {
  void operator()() { std::cout << "\n"; }
};

template <is_list list> struct print_list {
  void operator()() { for_each<print, list>{}(); }
};
// print tuple
template <class... T> struct print_tuple;
template <class head> struct print_tuple<std::tuple<head>> {
  void operator()() {
    print<head>{}();
    print<Enter>{}();
  }
};

template <class head, class... res>
struct print_tuple<std::tuple<head, res...>> {
  void operator()() {
    print<head>{}();
    print_tuple<std::tuple<res...>>{}();
  }
};
} // namespace wm
