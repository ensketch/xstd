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
#include <doctest/doctest.h>
//
#include <ensketch/xstd/functional/record.hpp>

using namespace ensketch::xstd;

namespace lib {
struct no_record {
  static int value;
};
int no_record::value = 0;
struct my_record {};
auto record_size(const my_record&) -> std::integral_constant<size_t, 1>;
auto record_size(const no_record&) -> no_record;

template <meta::string name>
  requires(name == meta::string{"name"})
auto record_element(const my_record&) -> std::string;

auto record_elements(const lib::my_record&) -> meta::map<  //
    record_field<"name", int>,
    record_field<"number", float>>;

template <meta::string key>
  requires(key == meta::string{"name"})
auto get(const my_record&) -> int;
template <meta::string key>
  requires(key == meta::string{"number"})
auto get(const my_record&) -> float;
}  // namespace lib

// namespace {
struct X {};
struct B {};
// }  // namespace

template <typename type>
concept forward_test = requires(type t) {
  { std::forward<type>(t) } -> std::same_as<type&&>;
};

SCENARIO("") {
  static_assert(forward_test<int>);
  static_assert(forward_test<int&>);
  static_assert(forward_test<int&&>);

  // meta::breakpoint(std::forward<float>(std::string{}));

  // static_assert(record_like<lib::my_record>);
  // static_assert(record_map_instance<meta::map<record_field<"a", int>>>);

  // static_assert(record_gettable_for<lib::my_record, "name", int>);
  // static_assert(record_gettable_for<lib::my_record&, "name", int&>);

  static_assert(
      std::same_as<X&&, decltype(std::forward_like<B>(std::declval<X>()))>);
  static_assert(
      std::same_as<X&, decltype(std::forward_like<B&>(std::declval<X>()))>);
  static_assert(std::same_as<const X&, decltype(std::forward_like<const B&>(
                                           std::declval<X>()))>);
  static_assert(
      std::same_as<X&&, decltype(std::forward_like<B&&>(std::declval<X>()))>);
  static_assert(std::same_as<const X&&, decltype(std::forward_like<const B&&>(
                                            std::declval<X>()))>);
  //
  static_assert(
      std::same_as<X&&, decltype(std::forward_like<B>(std::declval<X&>()))>);
  static_assert(
      std::same_as<X&, decltype(std::forward_like<B&>(std::declval<X&>()))>);
  static_assert(std::same_as<const X&, decltype(std::forward_like<const B&>(
                                           std::declval<X&>()))>);
  static_assert(
      std::same_as<X&&, decltype(std::forward_like<B&&>(std::declval<X&>()))>);
  static_assert(std::same_as<const X&&, decltype(std::forward_like<const B&&>(
                                            std::declval<X&>()))>);
  //
  static_assert(
      std::same_as<X&&, decltype(std::forward_like<B>(std::declval<X&&>()))>);
  static_assert(
      std::same_as<X&, decltype(std::forward_like<B&>(std::declval<X&&>()))>);
  static_assert(std::same_as<const X&, decltype(std::forward_like<const B&>(
                                           std::declval<X&&>()))>);
  static_assert(
      std::same_as<X&&, decltype(std::forward_like<B&&>(std::declval<X&&>()))>);
  static_assert(std::same_as<const X&&, decltype(std::forward_like<const B&&>(
                                            std::declval<X&&>()))>);
  //
  static_assert(std::same_as<const X&&, decltype(std::forward_like<B>(
                                            std::declval<const X&>()))>);
  static_assert(std::same_as<const X&, decltype(std::forward_like<B&>(
                                           std::declval<const X&>()))>);
  static_assert(std::same_as<const X&, decltype(std::forward_like<const B&>(
                                           std::declval<const X&>()))>);
  static_assert(std::same_as<const X&&, decltype(std::forward_like<B&&>(
                                            std::declval<const X&>()))>);
  static_assert(std::same_as<const X&&, decltype(std::forward_like<const B&&>(
                                            std::declval<const X&>()))>);
  //
  static_assert(std::same_as<const X&&, decltype(std::forward_like<B>(
                                            std::declval<const X&&>()))>);
  static_assert(std::same_as<const X&, decltype(std::forward_like<B&>(
                                           std::declval<const X&&>()))>);
  static_assert(std::same_as<const X&, decltype(std::forward_like<const B&>(
                                           std::declval<const X&&>()))>);
  static_assert(std::same_as<const X&&, decltype(std::forward_like<B&&>(
                                            std::declval<const X&&>()))>);
  static_assert(std::same_as<const X&&, decltype(std::forward_like<const B&&>(
                                            std::declval<const X&&>()))>);
}
