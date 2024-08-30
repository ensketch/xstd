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
#include <ensketch/xstd/match.hpp>
#include <ensketch/xstd/tuple.hpp>

namespace ensketch::xstd {

template <typename... ts>
struct regular_tuple_traits {
  static consteval auto types() noexcept { return meta::type_list<ts...>{}; }
  static consteval auto indices() noexcept {
    return meta::index_list_from_iota<size(types())>();
  }

  template <size_t index>
  struct wrapper {
    using type = meta::as_type<element<index>(types)>;
    constexpr auto&& value(this auto&& self) noexcept {
      return std::forward<decltype(self)>(self).data;
    }
    type data;
  };

  template <typename... t>
  struct amalgamate : t... {};

  template <meta::index_list_instance list>
  struct base_selector;
  //
  template <size_t... i>
  struct base_selector<meta::index_list<i...>> {
    using type = amalgamate<wrapper<i>...>;
  };
  //
  using base = typename base_selector<meta::as_type<indices>>::type;
};

template <typename... ts>
struct regular_tuple : regular_tuple_traits<ts...>::base {
  static consteval auto types() noexcept { return meta::type_list<ts...>{}; }
  static consteval auto detail() noexcept {
    return meta::as_value<regular_tuple_traits<ts...>>;
  }
};

template <typename type>
concept regular_tuple_instance =
    matches<type, []<typename... types>(regular_tuple<types...>) {
      return meta::as_signature<true>;
    }>;

template <size_t index>
constexpr auto value(regular_tuple_instance auto&& t) noexcept {
  using detail = meta::as_type<std::decay_t<decltype(t)>::detail()>;
  return static_cast<detail::template wrapper<index>>(t).value();
}

}  // namespace ensketch::xstd
