#pragma once
#include "src/list_base.hpp"
#include "src/tuple_helper.hpp"
#include <stdint.h>
#include <tuple>

/*
 * define operator about list
 * len :: a => list a -> int32_t
 * push :: a => list a -> a -> list a
 * join :: a => list a -> list a -> list a
 * to_tuple :: a => list a -> tuple a
 * head :: a => list a -> a @todo
 * tail :: a => list a -> a @todo
 * pop :: a => list a -> list a @todo
 */
namespace wm {
namespace detail {

template <is_list Root> struct len_impl {
  constexpr static uint32_t val = 1 + len_impl<typename Root::Next>::val;
};
template <> struct len_impl<EmptyList> { constexpr static uint32_t val = 0; };

template <is_list Root, class Val> struct push_impl {
  using type = List<typename Root::Val,
                    typename push_impl<typename Root::Next, Val>::type>;
};
template <class Val> struct push_impl<EmptyList, Val> {
  using type = List<Val>;
};

template <is_list lhs, is_list rhs> struct join_impl {
  using type = List<typename lhs::Val,
                    typename join_impl<typename lhs::Next, rhs>::type>;
};

template <> struct join_impl<EmptyList, EmptyList> { using type = EmptyList; };
template <is_list lhs> struct join_impl<lhs, EmptyList> { using type = lhs; };
template <is_list rhs> struct join_impl<EmptyList, rhs> { using type = rhs; };

template <class T> struct to_tuple_impl;
template <class Val, is_list Next> struct to_tuple_impl<List<Val, Next>> {
  using type = tuple_cat<std::tuple<Val>, typename to_tuple_impl<Next>::type>;
};
template <> struct to_tuple_impl<EmptyList> { using type = std::tuple<>; };
;
} // namespace detail

template <is_list Root> constexpr uint32_t len = detail::len_impl<Root>::val;

template <is_list Root, class Val>
using push = detail::push_impl<Root, Val>::type;

template <is_list lhs, is_list rhs>
using join = detail::join_impl<lhs, rhs>::type;

template <class T> using to_tuple = detail::to_tuple_impl<T>::type;
} // namespace wm
