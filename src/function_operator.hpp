// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include <type_traits>

/*
 * not_op :: (a -> bool) -> (a -> bool)
 * curry_op :: (a -> b -> c) -> b -> (a -> c)
 */
namespace wm {
template <template <typename...> class func, class... U> struct curry_op {
  template <class... V> using type = func<U..., V...>;
};

template <template <typename...> class func> struct not_op {
  template <class... T>
  using type =
      std::conditional_t<func<T...>::value, std::false_type, std::true_type>;
};

}; // namespace wm
