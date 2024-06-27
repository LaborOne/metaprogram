// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#include "src/algorithm.hpp"
#include "src/function_operator.hpp"
#include "src/printer.hpp"
#include "src/tree.hpp"
#include <iostream>
#include <type_traits>

#define INSERT(num1, num2, val)                                                \
  using Node##num1 = ::wm::insert<Node##num2, Int<val>>;

template <int num> using Int = std::integral_constant<int32_t, num>;
void test_tree() {
  using Node0 = wm::insert<wm::Empty, Int<3>>;
  INSERT(1, 0, 2);
  INSERT(2, 1, 1);
  INSERT(3, 2, 4);
  INSERT(4, 3, 5);
  INSERT(5, 4, 5);
  INSERT(6, 5, -1);
  INSERT(7, 6, 0);
  using N00 = wm::Node<Int<0>>;
  using N01 = wm::Node<Int<1>>;
  using N10 = wm::Node<Int<2>, true, N00, N01>;
  using N11 = wm::Node<Int<3>, true, N01, N00>;
  using N30 = wm::Node<Int<4>, true, N10, N11>;
  std::cout << "#################################################\n";
  wm::print_list<wm::flat_tree<Node7>>{}();
  std::cout << "#################################################\n";
  wm::print_list<wm::flat_tree<N30>>{}();
  std::cout << "#################################################\n";
}

void test_make_list() {
  using list = wm::make_list<uint32_t, 0, 8, 5, 5, 4, 1, 2, 3>;
  wm::print_tuple<wm::list2tuple<list>>{}();
  wm::print_list<list>{}();
  using node = wm::list2tree<list>;
  wm::print_list<wm::flat_tree<node>>{}();
}

void test_algorithm() {
  using list = wm::make_list<uint32_t, 3, 0, 8, 5, 5, 4, 1, 2, 3>;
  using less =
      wm::filter<wm::not_op<wm::curry_op<wm::greater, list::Val>::type>::type,
                 list>;
  using great = wm ::filter<wm::curry_op<wm::greater, list::Val>::type, list>;
  wm::print_list<great>{}();
  wm::print_list<less>{}();
}

void test_sort() {
  using list =
      wm::make_list<uint32_t, 3, 0, 8, 5, 5, 4, 1, 2, 3, 4, 4, 6, 63, 2, 3, 234,
                    325, 2, 2, 2, 4, 234, 325, 3, 3242, 1, 2, 3, 21, 32, 2>;
  using list2 = wm::join<list, list, list>;
  using list3 = wm::join<list2, list2>;
  using list4 = wm::join<list3, list3>;
  using list5 = wm::join<list4, list4>;
  using list6 = wm::join<list5, list5>;
  using list7 = wm::join<list6, list6>;
  using ret = wm::sort<list7>;
  wm::print_list<ret>{}();
  std::cout << "list len = " << wm::length<ret> << std::endl;
}

int main() {
  test_tree();
  test_make_list();
  test_sort();
}
