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
template <IsTuple T, IsTuple... Args> struct tuple_cat_impl {
  using type =
      typename tuple_cat_impl<T, typename tuple_cat_impl<Args...>::type>::type;
};
template <class... lhs, class... rhs>
struct tuple_cat_impl<std::tuple<lhs...>, std::tuple<rhs...>> {
  using type = std::tuple<lhs..., rhs...>;
};
template <IsTuple T> struct tuple_cat_impl<T> { using type = T; };
} // namespace detail

template <IsTuple... Args>
using tuple_cat = detail::tuple_cat_impl<Args...>::type;
} // namespace wm
