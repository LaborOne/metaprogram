// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#include "src/algorithm.hpp"
#include "src/function_operator.hpp"
#include "src/printer.hpp"
#include "src/rbtree.hpp"
#include <cassert>
#include <iostream>
#include <type_traits>


#define INSERT(num1, num2, val)                                                \
  using Node##num1 = ::wm::insert<Node##num2, Int<val>>;

template <int num> using Int = std::integral_constant<int32_t, num>;

void test_sort() {
  using list =
      wm::make_list<uint32_t, 3, 0, 8, 5, 5, 4, 1, 2, 3, 4, 4, 6, 63, 2, 3, 234,
                    325, 2, 2, 2, 4, 234, 325, 3, 3242, 1, 2, 3, 21, 32, 2>;
  using ret = wm::sort<list>;
  wm::print_list<ret>{}();
  std::cout << "list len = " << wm::size<ret> << std::endl;
}

void rb_insert() {
  using list = wm::reverse<wm::make_list<int32_t, 41, 38, 31, 12, 19, 8>>;
  using t = wm::list2tree<list>;
  wm::print_list<wm::flat_tree<t>>{}();
  std::cout << " rbtree height = " << wm::height<t> << ", size = "
            << wm::size<t> << "\n";
}

void test_tuple_flat() {
  using t1 = std::tuple<int, double, int, double>;
  using t2 = std::tuple<int, std::tuple<std::tuple<double>, int>, double>;
  t2 v{1, {{2.0}, 3}, 4.0};
  t1 r{1, 2.0, 3, 4.0};
  assert(r == wm::tuple_flat(v));
}

int main() {
  test_sort();
  rb_insert();
}
