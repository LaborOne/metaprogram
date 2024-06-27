// Copyright (c) wm. All rights reserved.
// Licensed under the MIT License.
#pragma once
#include <type_traits>
/*
 * define base list and some type_traits about list.
 */
namespace wm {
struct EmptyList;

template <class T> struct is_empty_list_impl : std::false_type {};
template <> struct is_empty_list_impl<EmptyList> : std::true_type {};
template <class T>
inline constexpr bool is_empty_list_v = is_empty_list_impl<T>::value;
template <class T> concept is_empty_list = is_empty_list_v<T>;

template <class Val_, class Next_ = EmptyList> struct List {
  using Next = Next_;
  using Val = Val_;
};

template <class> struct is_list_impl : std::false_type {};
template <class Val_, class Next_>
struct is_list_impl<List<Val_, Next_>> : std::true_type {};
template <class T> inline constexpr bool is_list_v = is_list_impl<T>::value;
template <class T> concept is_list = is_list_v<T> || is_empty_list_v<T>;
} // namespace wm
