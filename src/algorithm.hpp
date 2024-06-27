// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include "src/function_operator.hpp"
#include "src/list.hpp"
#include "src/list_base.hpp"
#include "src/tree_base.hpp"
#include <type_traits>
#include <utility>

/*
 * filter :: (list a -> bool) -> list a -> list a
 * map :: (a -> list) -> list a -> list a
 * greater :: (is_val a) => a -> a -> bool @ todo use compare_able and operator
 * <.
 * sort :: list a -> list a @ todo use compare_able.
 * for_each :: (call_able a) => a -> list b
 */
namespace wm {

template <class T> concept cmp_able = true && is_val_type_v<T>;
template <cmp_able target, cmp_able val, class = void>

struct greater : std::false_type {};
template <cmp_able target, cmp_able val>
    struct greater < target,
    val,
    std::void_t<std::enable_if_t<target::value<val::value>>> : std::true_type {
};

template <class op, class = void> struct call_able_impl : std::true_type {};
template <class op>
struct call_able_impl<op, std::void_t<decltype(std::declval<op>()())>>
    : std::true_type {};
template <class op>
constexpr inline bool call_able_v = call_able_impl<op>::value;
template <class op> concept call_able = call_able_v<op>;
namespace detail {

template <template <typename> class T, class val, class = void>
struct map_func : std::false_type {};
template <template <typename> class T, class val>
struct map_func<
    T, val,
    std::void_t<std::enable_if_t<is_list_v<typename T<val>::type> ||
                                 is_empty_v<typename T<val>::type>>>>
    : std::true_type {};
template <template <typename> class T, class val>
concept is_map_func = map_func<T, val>::vaule;

template <template <typename> class T, class val, class = void>
struct one_predicate_impl : std::false_type {};
template <template <typename> class T, class val>
struct one_predicate_impl<
    T, val,
    std::void_t<
        std::enable_if_t<std::is_same_v<bool, typename T<val>::value_type>>>>
    : std::true_type {};
template <template <typename> class T, class val>
inline constexpr bool one_predicate_v = one_predicate_impl<T, val>::value;
template <template <typename> class T, class val>
concept one_predicate = one_predicate_v<T, val>;

template <template <typename> class func, is_list list>
    requires is_empty_list<list> ||
    one_predicate<func, typename list::Val> struct filter_impl;
template <template <typename> class func, class val, is_list next>
struct filter_impl<func, List<val, next>> {
  using type =
      std::conditional_t<func<val>::value,
                         List<val, typename filter_impl<func, next>::type>,
                         typename filter_impl<func, next>::type>;
};
template <template <typename> class func> struct filter_impl<func, EmptyList> {
  using type = EmptyList;
};

template <template <typename> class func, is_list list> struct map_impl;
template <template <typename> class func, class val, is_list next>
struct map_impl<func, List<val, next>> {
  using type =
      join<typename func<val>::type, typename map_impl<func, next>::type>;
};
template <template <typename> class func> struct map_impl<func, EmptyList> {
  using type = EmptyList;
};

template <is_list list> struct sort_impl;
template <> struct sort_impl<EmptyList> { using type = EmptyList; };
template <cmp_able val> struct sort_impl<List<val, EmptyList>> {
  using type = List<val, EmptyList>;
};
template <cmp_able val, is_list next> struct sort_impl<List<val, next>> {
  using g =
      typename filter_impl<curry_op<greater, val>::template type, next>::type;
  using l = typename filter_impl<
      not_op<curry_op<greater, val>::template type>::template type, next>::type;
  using type =
      join<push<typename sort_impl<l>::type, val>, typename sort_impl<g>::type>;
};

template <template <typename> class op, is_list list> struct for_each_impl {
  void operator()() {
    if constexpr (is_empty_list_v<list>) {
      static_assert(call_able_v<op<EmptyList>>);
      op<EmptyList>{}();
    } else {
      static_assert(call_able_v<op<typename list::Val>>);
      op<typename list::Val>{}();
      for_each_impl<op, typename list::Next>{}();
    }
  }
};

} // namespace detail
template <template <typename> class func, is_list list>
using filter = detail::filter_impl<func, list>::type;
template <template <typename> class func, is_list list>
using map = detail::map_impl<func, list>::type;
template <is_list list> using sort = detail::sort_impl<list>::type;
template <template <typename> class op, is_list list>
using for_each = detail::for_each_impl<op, list>;
} // namespace wm
