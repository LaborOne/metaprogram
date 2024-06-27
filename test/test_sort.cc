#include "src/algorithm.hpp"
#include "src/list.hpp"
#include "src/printer.hpp"
#include <iostream>
#include <type_traits>

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
  std::cout << "*********begin*********" << std::endl;
  test_sort();
  std::cout << "*********end*********" << std::endl;
}
