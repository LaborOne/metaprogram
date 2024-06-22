#include <stdio.h>

#include <cassert>
#include <concepts>
#include <iostream>
#include <string>
#include <type_traits>
namespace tmp {
struct Empty;
namespace detail {
// val traits
template <class T, class = void> struct is_val_type : std::false_type {};
template <class T>
struct is_val_type<
    T, std::void_t<decltype(std::declval<T>().value), typename T::value_type>>
    : std::true_type {};
template <class T> inline constexpr bool is_val_type_v = is_val_type<T>::value;
template <class T> concept Val_ = is_val_type_v<T>;

// black_tag
template <bool> struct black_tag : std::false_type {};
template <> struct black_tag<true> : std::true_type {};

// empty traits
template <class T> struct is_empty : std::false_type {};
template <> struct is_empty<Empty> : std::true_type {};
template <class T> inline constexpr bool is_empty_v = is_empty<T>::value;

// node traits
template <Val_, bool, class, class> struct Node;
template <class T> struct is_node : std::false_type {};
template <Val_ val, bool is_black, class L, class R>
struct is_node<Node<val, is_black, L, R>> : std::true_type {};
template <class T> inline constexpr bool is_node_v = is_node<T>::value;
template <class T> concept Node_ = is_node_v<T> || is_empty_v<T> || true;

// data Tree a = Empty | Tree {left = Tree a, right = Tree a, val = a} 
template <Val_ val_, bool is_black = true, class Left_ = Empty,
          class Right_ = Empty>
struct Node {
  using Val = val_;
  using Left = Left_;
  using Right = Right_;
  using Black = black_tag<is_black>;
};

// black tag traits
template <Node_ Root, typename = void> struct is_black : std::false_type {};
template <Node_ Root>
struct is_black<Root, std::void_t<typename Root::Black::type>>
    : std::true_type {};
template <Node_ Root>
inline constexpr bool is_black_v = is_black<Root>::value || is_empty_v<Root>;
template <class Root> concept IsBlack = is_black_v<Root>;
template <class Root> concept IsRed = !IsBlack<Root>;

template <Val_ T> struct PrimeNode { using type = Node<T>; };
template <Val_ T> using PrimeNode_t = typename PrimeNode<T>::type;

// tree insert opeator
template <Node_ Root, Val_ Val> requires IsBlack<Root> struct Insert {
  using type = std::conditional_t<
      Val::value <= Root::Val::value,
      Node<typename Root::Val, true,
           typename Insert<typename Root::Left, Val>::type,
           typename Root::Right>,
      Node<typename Root::Val, true, typename Root::Left,
           typename Insert<typename Root::Right, Val>::type>>;
};
template <Val_ Val> struct Insert<Empty, Val> {
  using type = PrimeNode_t<Val>;
};

// print  node
template <Node_ T> requires IsBlack<T> void print() {
  std::cout << T::Val::value << " ";
}
template <> void print<Empty>() { std::cout << "empty "; }

template <Node_ T> requires IsBlack<T> void transfer_impl() {
  if constexpr (!is_empty_v<typename T::Left>) {
    transfer_impl<typename T::Left>();
  }
  print<T>();
  if constexpr (!is_empty_v<typename T::Right>) {
    transfer_impl<typename T::Right>();
  }
}
} // namespace detail
template <detail::Node_ Root, detail::Val_ Val>
using Insert_t = detail::Insert<Root, Val>::type;

// mid transfer opeator
template <detail::Node_ T> requires detail::IsBlack<T> void transfer() {
  if constexpr (!detail::is_empty_v<typename T::Left>) {
    detail::transfer_impl<typename T::Left>();
  }
  detail::print<T>();
  if constexpr (!detail::is_empty_v<typename T::Right>) {
    detail::transfer_impl<typename T::Right>();
  }
  std::cout << "\n";
}
} // namespace tmp
