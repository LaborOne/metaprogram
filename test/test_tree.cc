#include "src/algorithm.hpp"
#include "src/list.hpp"
#include "src/printer.hpp"
#include "src/rbtree.hpp"
#include <iostream>
#include <type_traits>

#define INSERT(num1, num2, val)                                                \
  using Node##num1 = ::wm::insert<Node##num2, Int<val>>;
using wm::Black;
using wm::Empty;
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
  using N10 = wm::Node<Int<2>, wm::Black, N00, N01>;
  using N11 = wm::Node<Int<3>, wm::Black, N01, N00>;
  using N30 = wm::Node<Int<4>, wm::Black, N10, N11>;
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

void test_rotate1() {
  using a = wm::Node<Int<1>>;
  using b = wm::Node<Int<3>>;
  using c = wm::Node<Int<5>>;
  using xl = wm::Node<Int<2>, Black, a, b>;
  using yl = wm::Node<Int<4>, Black, xl, c>;
  using yr = wm::Node<Int<4>, Black, b, c>;
  using xr = wm::Node<Int<2>, Black, a, yr>;
  using rotate_left_xr = wm::detail::rotate_left<xr>;
  using rotate_right_yl = wm::detail::rotate_right<yl>;
  static_assert(std::is_same_v<rotate_left_xr, yl>);
  static_assert(std::is_same_v<rotate_right_yl, xr>);
  std::cout << "***********left rbtree*********\n";
  wm::print_list<wm::flat_tree<yl>>{}();
  std::cout << "************rotate_left******\n";
  wm::print_list<wm::flat_tree<rotate_left_xr>>{}();
  std::cout << "***********right rbtree*********\n";
  wm::print_list<wm::flat_tree<xr>>{}();
  std::cout << "************rotate_right******\n";
  wm::print_list<wm::flat_tree<rotate_right_yl>>{}();
}

void test_rotate2() {
  using a = Empty;
  using b = Empty;
  using c = Empty;
  using xl = wm::Node<Int<2>, Black, a, b>;
  using yl = wm::Node<Int<4>, Black, xl, c>;
  using yr = wm::Node<Int<4>, Black, b, c>;
  using xr = wm::Node<Int<2>, Black, a, yr>;
  using rotate_left_xr = wm::detail::rotate_left<xr>;
  using rotate_right_yl = wm::detail::rotate_right<yl>;
  static_assert(std::is_same_v<rotate_left_xr, yl>);
  static_assert(std::is_same_v<rotate_right_yl, xr>);
  std::cout << "***********left rbtree*********\n";
  wm::print_list<wm::flat_tree<yl>>{}();
  std::cout << "************rotate_left******\n";
  wm::print_list<wm::flat_tree<rotate_left_xr>>{}();
  std::cout << "***********right rbtree*********\n";
  wm::print_list<wm::flat_tree<xr>>{}();
  std::cout << "************rotate_right******\n";
  wm::print_list<wm::flat_tree<rotate_right_yl>>{}();
}

#define RBINSERT(num1, num2, val)                                              \
  using node##num1 = wm::insert<node##num2, Int<val>>
void test_rb_tree_insert() {
  using node0 = wm::insert<wm::Empty, Int<41>>;
  using node1 = wm::insert<node0, Int<38>>;
  using node2 = wm::insert<node1, Int<31>>;
  RBINSERT(3, 2, 12);
  RBINSERT(4, 3, 19);
  RBINSERT(5, 4, 8);
  // RBINSERT(6, 5, 7);
  // RBINSERT(7, 6, 8);
  // RBINSERT(8, 7, 9);
  using ret = node5;
  wm::print_list<wm::flat_tree<ret>>{}();
}
int main() {
  std::cout << "*********begin*********" << std::endl;
  // std::cout << " ########test rbtree insert, flat, print #####\n";
  // test_tree();
  // std::cout << " ########test rbtree rotate #####\n";
  // test_rotate1();
  // test_rotate2();
  test_rb_tree_insert();
  std::cout << "*********end*********" << std::endl;
}
