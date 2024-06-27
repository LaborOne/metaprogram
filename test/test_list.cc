#include "src/list.hpp"
#include "src/printer.hpp"
#include <iostream>
#include <type_traits>

template <int num> using Int = std::integral_constant<int32_t, num>;

void test_make_list() {
  using list = wm::make_list<uint32_t, 0, 8, 5, 5, 4, 1, 2, 3>;
  wm::print_list<list>{}();
  std::cout << "list len = " << wm::length<list> << std::endl;
}

int main() {
  std::cout << "*********begin*********" << std::endl;
  test_make_list();
  std::cout << "*********end*********" << std::endl;
}
