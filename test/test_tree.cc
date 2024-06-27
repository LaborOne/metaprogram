#include "src/list.hpp"
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
  using list = wm::make_list<int32_t, 0, -1, 5, 5, 4, 1, 2, 3>;
  using node = wm::list2tree<list>;
  wm::print_list<wm::flat_tree<node>>{}();
  std::cout << "#################################################\n";
}

int main() {
  std::cout << "*********begin*********" << std::endl;
  test_tree();
  std::cout << "*********end*********" << std::endl;
}
