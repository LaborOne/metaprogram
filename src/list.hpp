// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include "src/list_base.hpp"
#include "src/tuple_helper.hpp"
#include <stdint.h>
#include <tuple>

/*
 * define operator about list
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

template <is_list root, class val> struct push_impl {
  using type = List<typename root::Val,
                    typename push_impl<typename root::Next, val>::type>;
};
template <class val> struct push_impl<EmptyList, val> {
  using type = List<val>;
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

template <class> struct list2tuple_impl;
template <class val, is_list next> struct list2tuple_impl<List<val, next>> {
  using type = tuple_cat<std::tuple<val>, typename list2tuple_impl<next>::type>;
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

template <is_list list> struct reverse_impl {
  using type =
      typename join_impl<typename reverse_impl<typename list::Next>::type,
                         List<typename list::Val>>::type;
};

template <> struct reverse_impl<EmptyList> { using type = EmptyList; };
} // namespace detail

template <is_list root, class Val>
using push = detail::push_impl<root, Val>::type;

template <is_list lhs, is_list... list>
using join = detail::join_impl<lhs, list...>::type;

template <class T> using list2tuple = detail::list2tuple_impl<T>::type;

template <class Tp, Tp... args>
using make_list = detail::make_list_impl<Tp, args...>::type;
template <is_list list> using reverse = detail::reverse_impl<list>::type;
} // namespace wm
