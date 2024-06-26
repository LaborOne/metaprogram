#pragma once
#include "src/function_operator.hpp"
#include "src/list.hpp"
#include "src/list_base.hpp"
#include "src/tree_base.hpp"
#include <type_traits>

/*
 * filter :: (list a -> bool) -> list a -> list a
 * greater :: (is_val a) => a -> a -> bool @ todo use compare_able and operator
 * <.
 * sort :: list a -> list a @ todo use compare_able.
 */
namespace wm {

template <class T> concept is_compare_able = true && is_val_type_v<T>;
template <is_compare_able target, is_compare_able val, class = void>
struct greater : std::false_type {};
template <is_compare_able target, is_compare_able val>
    struct greater < target,
    val,
    std::void_t<std::enable_if_t<target::value<val::value>>> : std::true_type {
};

namespace detail {
template <template <typename> class T, class val, class = void>
struct is_func_impl : std::false_type {};
template <template <typename> class T, class val>
struct is_func_impl<T, val,
                    std::void_t<std::enable_if_t<
                        std::is_same_v<bool, typename T<val>::value_type>>>>
    : std::true_type {};
template <template <typename> class T, class val>
inline constexpr bool is_func_v = is_func_impl<T, val>::value;
template <template <typename> class T, class val>
concept is_func = is_func_v<T, val>;

template <template <typename> class func, is_list list>
    requires is_empty_list<list> ||
    is_func<func, typename list::Val> struct filter_impl;
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
template <is_list list> struct sort_impl;
template <> struct sort_impl<EmptyList> { using type = EmptyList; };
template <is_compare_able val> struct sort_impl<List<val, EmptyList>> {
  using type = List<val, EmptyList>;
};
template <is_compare_able val, is_list next> struct sort_impl<List<val, next>> {
  using g =
      typename filter_impl<curry_op<greater, val>::template type, next>::type;
  using l = typename filter_impl<
      not_op<curry_op<greater, val>::template type>::template type, next>::type;
  using type =
      join<push<typename sort_impl<l>::type, val>, typename sort_impl<g>::type>;
};
} // namespace detail
template <template <typename> class func, is_list list>
using filter = detail::filter_impl<func, list>::type;
template <is_list list> using sort = detail::sort_impl<list>::type;
} // namespace wm
