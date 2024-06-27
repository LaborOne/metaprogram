// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include "src/list_base.hpp"
#include "src/tuple_helper.hpp"
#include <stdint.h>
#include <tuple>

/*
 * define operator about list
 * length :: list a -> int32_t
 * push :: list a -> a -> list a
 * join :: list a -> list a -> list a
 * list2tuple :: list a -> tuple a
 * make_list :: a -> list a
 * head :: list a -> a @todo
 * tail :: list a -> a @todo
 * pop :: list a -> list a @todo
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

template <is_list lhs, is_list... res> struct join_impl {
  using type = List<typename lhs::Val,
                    typename join_impl<typename lhs::Next, res...>::type>;
};
template <> struct join_impl<EmptyList> { using type = EmptyList; };
template <is_list lhs> struct join_impl<lhs> { using type = lhs; };
template <is_list... res> struct join_impl<EmptyList, res...> {
  using type = typename join_impl<res...>::type;
};

template <class T> struct list2tuple_impl;
template <class Val, is_list Next> struct list2tuple_impl<List<Val, Next>> {
  using type = tuple_cat<std::tuple<Val>, typename list2tuple_impl<Next>::type>;
};
template <> struct list2tuple_impl<EmptyList> { using type = std::tuple<>; };
;
template <class Tp, Tp arg, Tp... res> struct make_list_impl {
  using type =
      typename join_impl<typename make_list_impl<Tp, arg>::type,
                         typename make_list_impl<Tp, res...>::type>::type;
};
template <class Tp, Tp arg> struct make_list_impl<Tp, arg> {
  using type = List<std::integral_constant<Tp, arg>>;
};
} // namespace detail

template <is_list Root>
inline constexpr uint32_t length = detail::len_impl<Root>::val;

template <is_list Root, class Val>
using push = detail::push_impl<Root, Val>::type;

template <is_list lhs, is_list... list>
using join = detail::join_impl<lhs, list...>::type;

template <class T> using list2tuple = detail::list2tuple_impl<T>::type;

template <class Tp, Tp... args>
using make_list = detail::make_list_impl<Tp, args...>::type;
} // namespace wm
