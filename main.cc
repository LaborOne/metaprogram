#include "src/rbtree.h"
#define INSERT(num1, num2, val)                                                \
  using Node##num1 = ::tmp::Insert_t<Node##num2, Int<val>>;

template <int num> using Int = std::integral_constant<int32_t, num>;
int main() {
  using Node0 = ::tmp::Insert_t<::tmp::Empty, Int<3>>;
  INSERT(1, 0, 2);
  INSERT(2, 1, 1);
  INSERT(3, 2, 4);
  INSERT(4, 3, 5);
  INSERT(5, 4, -2);
  INSERT(6, 5, -1);
  INSERT(7, 6, 89);
  ::tmp::transfer<Node7>();
}
