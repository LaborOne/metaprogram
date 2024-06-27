#include "src/algorithm.hpp"
#include "src/function_operator.hpp"
#include "src/printer.hpp"
#include <iostream>
#include <type_traits>

void test_algorithm() {
  using list = wm::make_list<uint32_t, 3, 0, 8, 5, 5, 4, 1, 2, 3>;
  using less =
      wm::filter<wm::not_op<wm::curry_op<wm::greater, list::Val>::type>::type,
                 list>;
  using great = wm ::filter<wm::curry_op<wm::greater, list::Val>::type, list>;
  wm::print_list<great>{}();
  wm::print_list<less>{}();
}

int main() {
  std::cout << "*********begin*********" << std::endl;
  test_algorithm();
  std::cout << "*********end*********" << std::endl;
}
