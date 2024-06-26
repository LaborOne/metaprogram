#pragma once
#include "src/list_base.hpp"
#include "src/tree_base.hpp"
#include <iostream>
#include <tuple>
/*
 * define the printer of class.
 */
namespace wm {
template <class T> struct print;
template <is_val val, bool is_black, is_node left, is_node right>
struct print<Node<val, is_black, left, right>> {
  void operator()() { std::cout << val::value << " "; }
};
template <> struct print<Empty> {
  void operator()() { std::cout << "nil "; }
};
template <is_val val> struct print<val> {
  void operator()() { std::cout << val::value << " "; }
};
template <class node, is_list next> struct print<List<node, next>> {
  void operator()() { print<node>{}(); }
};
template <> struct print<EmptyList> {
  void operator()() { std::cout << "emptylist "; }
};
struct Enter;
template <> struct print<Enter> {
  void operator()() { std::cout << "\n"; }
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
