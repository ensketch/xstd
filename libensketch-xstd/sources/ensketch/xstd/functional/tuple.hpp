// Copyright © 2024 Markus Pawellek
//
// This file is part of `xstd`.
//
// `xstd` is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// `xstd` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `xstd`. If not, see <https://www.gnu.org/licenses/>.
//
#pragma once
#include <ensketch/xstd/functional/defaults.hpp>
//
#include <ensketch/xstd/meta/index_list.hpp>
#include <ensketch/xstd/meta/type_list.hpp>

namespace ensketch::xstd {

// Multiple tuple structures can exist for various reasons,
// each significant in different situations.
// A concept for generic tuples is introduced,
// utilizing structured bindings requirements for generalization.

namespace detail {

template <typename tuple_type, size_t index>
concept tuple_get_access = requires(tuple_type value) {
  {
    get<index>(value)
  }
  -> std::convertible_to<typename std::tuple_element<index, tuple_type>::type>;
};

template <typename tuple_type, size_t index>
concept xstd_tuple_value_access = requires(tuple_type tuple) {
  {
    value<index>(tuple)
  } -> std::convertible_to<typename tuple_type::template type<index>>;
  // Only ask for specific type.
  // For an array, you would not want to provide a type list.
};

}  // namespace detail

/// Checks whether a given type fulfills the requirements of a generic tuple.
///
template <typename tuple_type>
concept generic_tuple = all_of(
    meta::index_list_from_iota<std::tuple_size<tuple_type>::value>(),
    []<size_t index> { return detail::tuple_get_access<tuple_type, index>; });

///
///
template <typename T>
concept generic_reducible_tuple = generic_tuple<std::decay_t<T>>;

///
///
template <typename tuple_type>
concept generic_xstd_tuple =
    all_of(meta::index_list_from_iota<tuple_type::size()>(),  //
           []<size_t index> {
             return detail::xstd_tuple_value_access<tuple_type, index>;
           });

///
///
template <typename tuple_type>
concept generic_reducible_xstd_tuple =
    generic_xstd_tuple<std::decay_t<tuple_type>>;

///
///
// A tuple type is not a tag type and as such should be given as template argument.
template <generic_tuple tuple_type, size_t... indices>
consteval auto type_list_from(meta::index_list<indices...>) {
  return meta::type_list<
      typename std::tuple_element<indices, tuple_type>::type...>{};
}
//
template <generic_tuple tuple_type>
consteval auto type_list_from() {
  return type_list_from<tuple_type>(
      meta::index_list_from_iota<std::tuple_size<tuple_type>::value>());
}

///
///
template <size_t... indices>
constexpr auto for_each(generic_reducible_tuple auto&& tuple,
                        auto&& f,
                        meta::index_list<indices...>) {
  (f(get<indices>(std::forward<decltype(tuple)>(tuple))), ...);
}
//
constexpr auto for_each(generic_reducible_tuple auto&& tuple, auto&& f) {
  using tuple_type = std::decay_t<decltype(tuple)>;
  for_each(std::forward<decltype(tuple)>(tuple), std::forward<decltype(f)>(f),
           meta::index_list_from_iota<std::tuple_size<tuple_type>::value>());
}

}  // namespace ensketch::xstd