// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include <tuple>
/*
 * define tuple operator.
 * tuple_cat :: a -> tuple a -> tuple a
 */
namespace wm {
template <class T> struct is_tuple : std::false_type {};
template <class... T> struct is_tuple<std::tuple<T...>> : std::true_type {};
template <class T> inline constexpr bool is_tuple_v = is_tuple<T>::value;
template <class T> concept IsTuple = is_tuple_v<T>;

namespace detail {
template <IsTuple T, IsTuple... Args>
struct tuple_cat_impl
    : tuple_cat_impl<T, typename tuple_cat_impl<Args...>::type> {};
template <class... lhs, class... rhs>
struct tuple_cat_impl<std::tuple<lhs...>, std::tuple<rhs...>> {
  using type = std::tuple<lhs..., rhs...>;
};
template <IsTuple T> struct tuple_cat_impl<T> { using type = T; };

template <class...> struct tuple_flat_rt_impl;
template <class... ret, class... args, class... res>
struct tuple_flat_rt_impl<std::tuple<ret...>, std::tuple<args...>, res...>
    : tuple_flat_rt_impl<std::tuple<ret...>, args..., res...> {};
template <class... ret, class T, class... res>
struct tuple_flat_rt_impl<std::tuple<ret...>, T, res...>
    : tuple_flat_rt_impl<std::tuple<ret..., T>, res...> {};
template <class... ret> struct tuple_flat_rt_impl<std::tuple<ret...>> {
  using type = std::tuple<ret...>;
};
template <class Args>
using tuple_flat_rt =
    typename tuple_flat_rt_impl<std::tuple<>, std::decay_t<Args>>::type;

template <class ret_type, class...> struct tuple_flatter;
template <class ret_type, class... Ret, class... args, class... Res>
struct tuple_flatter<ret_type, std::tuple<Ret...>, std::tuple<args...>,
                     Res...> {
  using Arg = std::tuple<args...>;
  using PreArg = std::tuple<Ret...>;
  template <class Tp, class... U, std::size_t... Idx>
  static ret_type do_flat_impl(std::index_sequence<Idx...>, Ret... pre_arg,
                               Tp &&tp, U &&...u) {
    using next_flatter =
        tuple_flatter<ret_type, std::tuple<Ret...>, args..., Res...>;
    return next_flatter::do_flat(std::forward<Ret>(pre_arg)...,
                                 std::get<Idx>(std::forward<Tp>(tp))...,
                                 std::forward<U>(u)...);
  }

  static ret_type do_flat(Ret... pre_arg, Arg arg, Res... res) {
    return do_flat_impl(std::make_index_sequence<std::tuple_size_v<Arg>>{},
                        std::forward<Ret>(pre_arg)..., std::forward<Arg>(arg),
                        std::forward<Res>(res)...);
  }
};

template <class ret_type, class... ret, class arg, class... res>
struct tuple_flatter<ret_type, std::tuple<ret...>, arg, res...>
    : tuple_flatter<ret_type, std::tuple<ret..., arg>, res...> {};

template <class ret_type, class... ret>
struct tuple_flatter<ret_type, std::tuple<ret...>> {
  template <class... U> static ret_type do_flat(U &&...u) {
    return ret_type(std::forward<U>(u)...);
  }
};
} // namespace detail

template <class T> detail::tuple_flat_rt<T> tuple_flat(T &&t) {
  using flatter = detail::tuple_flatter<detail::tuple_flat_rt<T>, std::tuple<>,
                                        std::decay_t<T>>;
  return flatter::do_flat(std::forward<T>(t));
}

template <IsTuple... Args>
using tuple_cat = detail::tuple_cat_impl<Args...>::type;
} // namespace wm
