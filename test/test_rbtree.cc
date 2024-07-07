#include "src/list.hpp"
#include "src/printer.hpp"
#include "src/rbtree.hpp"
#include <iostream>
#include <type_traits>

template <int num> using Int = std::integral_constant<int32_t, num>;
#define RBINSERT(num1, num2, val)                                              \
  using node##num1 = wm::insert<node##num2, Int<val>>
void test_rb_tree_insert() {
  using list = wm::make_list<uint32_t, 1, 2, 3, 4, 5, 6, 7, 8>;
  using rlist = wm::reverse<list>;
  using r1 = wm::join<rlist, rlist>; // 6-16
  using t1 = wm::list2tree<r1>;
  std::cout << " height = " << wm::height<t1> << ", size = "
            << wm::size<t1> << "\n";
  using r2 = wm::join<r1, r1>; // 8-32
  using t2 = wm::list2tree<r2>;
  std::cout << " height = " << wm::height<t2> << ", size = "
            << wm::size<t2> << "\n";
  using r3 = wm::join<r2, r2>; // 10-64
  using t3 = wm::list2tree<r3>;
  std::cout << " height = " << wm::height<t3> << ", size = "
            << wm::size<t3> << "\n";
  using r4 = wm::join<r3, r3>;
  using t4 = wm::list2tree<r4>;
  std::cout << " height = " << wm::height<t4> << ", size = "
            << wm::size<t4> << "\n";
  using r5 = wm::join<r4, r4>;
  using t5 = wm::list2tree<r5>;
  std::cout << " height = " << wm::height<t5> << ", size = "
            << wm::size<t5> << "\n";
  using r6 = wm::join<r5, r5>;
  using t6 = wm::list2tree<r6>;
  std::cout << " height = " << wm::height<t6> << ", size = "
            << wm::size<t6> << "\n";
  using r7 = wm::join<r6, r6>;
  using t7 = wm::list2tree<r7>;
  std::cout << " height = " << wm::height<t7> << ", size = "
            << wm::size<t7> << "\n";
  using r8 = wm::join<r7, r7>;
  using t8 = wm::list2tree<r8>;
  std::cout << " height = " << wm::height<t8> << ", size = "
            << wm::size<t8> << "\n";
  using r9 = wm::join<r8, r8>;
  using t9 = wm::list2tree<r9>;
  std::cout << " height = " << wm::height<t9> << ", size = "
            << wm::size<t9> << "\n";
  // using t1 = wm::list2tree<r3>;
  // wm::print_list<wm::flat_tree<t1>>{}();
  // std::cout << " height = " << wm::height<t1> << ", size = "
  //           << wm::size<t1> << "\n";
}

void test_rb_tree_delete() {
  using node0 = wm::insert<wm::Empty, Int<41>>;
  RBINSERT(1, 0, 38);
  RBINSERT(2, 1, 31);
  RBINSERT(3, 2, 12);
  RBINSERT(4, 3, 19);
  RBINSERT(5, 4, 8);
  wm::print_list<wm::flat_tree<node5>>{}();
  std::cout << "*********delete 8*********" << std::endl;
  using d1 = wm::delete_node<node5, Int<8>>;
  wm::print_list<wm::flat_tree<d1>>{}();
  std::cout << "*********delete 12*********" << std::endl;
  using d2 = wm::delete_node<d1, Int<12>>;
  wm::print_list<wm::flat_tree<d2>>{}();
  std::cout << "*********delete 19*********" << std::endl;
  using d3 = wm::delete_node<d2, Int<19>>;
  wm::print_list<wm::flat_tree<d3>>{}();
  std::cout << "*********delete 31*********" << std::endl;
  using d4 = wm::delete_node<d3, Int<31>>;
  wm::print_list<wm::flat_tree<d4>>{}();
  std::cout << "*********delete 38*********" << std::endl;
  using d5 = wm::delete_node<d4, Int<38>>;
  wm::print_list<wm::flat_tree<d5>>{}();
  std::cout << "*********delete 41*********" << std::endl;
  using d6 = wm::delete_node<d5, Int<41>>;
  wm::print_list<wm::flat_tree<d6>>{}();
}
int main() {
  std::cout << "*********************begin***********************" << std::endl;
  test_rb_tree_insert();
  test_rb_tree_delete();
  std::cout << "*********************end*************************" << std::endl;
}
