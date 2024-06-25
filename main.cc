#include "src/printer.hpp"
#include "src/tree.hpp"
#include <iostream>
#include <type_traits>

#define INSERT(num1, num2, val)                                                \
  using Node##num1 = ::wm::insert<Node##num2, Int<val>>;

template <int num> using Int = std::integral_constant<int32_t, num>;
void test() {
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
  wm::print_tuple<wm::flat_tree<Node7>>{}();
  wm::print_tuple<wm::flat_tree<N30>>{}();
  std::cout << "#################################################\n";
}

int main() { test(); }
