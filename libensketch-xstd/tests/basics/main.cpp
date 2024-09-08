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
#include <iomanip>
#include <iostream>
#include <print>
//
#include <ensketch/xstd/functional/match.hpp>
#include <ensketch/xstd/xstd.hpp>

using ensketch::xstd::czstring;
using namespace ensketch::xstd;
using namespace ensketch::xstd::meta;

using namespace std;

// template <typename... types>
// struct std::formatter<meta::type_list<types...>, char> {
//   template <class context>
//   constexpr auto parse(context& ctx) -> context::iterator {
//     return ctx.begin();
//   }
//   template <class context>
//   auto format(meta::type_list<types...> list,
//               context& ctx) const -> context::iterator {
//     std::string str{};
//     for_each(list, [&str]<typename type> { str += typeid(type).name(); });
//     return ranges::copy(str, ctx.out()).out;
//   }
// };

// template <auto... values>
// struct std::formatter<meta::value_list<values...>, char> {
//   template <class context>
//   constexpr auto parse(context& ctx) -> context::iterator {
//     return ctx.begin();
//   }
//   template <class context>
//   auto format(meta::value_list<values...> list,
//               context& ctx) const -> context::iterator {
//     std::string str{};
//     for_each(list, [&str]<auto value> {
//       // str += std::format("{}: {}, ", value, typeid(value).name());
//       str += value;
//       str += ": ";
//       str += typeid(value).name();
//       str += ", ";
//     });
//     return ranges::copy(str, ctx.out()).out;
//   }
// };

template <typename... ts>
struct tuple_detail {
  static constexpr auto types = type_list<ts...>{};
  static constexpr auto indices = index_list_from_iota<size(types)>();

  template <size_t index>
  struct wrapper {
    using type = as_type<element<index>(types)>;
    constexpr auto&& value(this auto&& self) noexcept {
      return std::forward<decltype(self)>(self).data;
    }
    type data;
  };

  template <typename... t>
  struct amalgamate : t... {};

  template <index_list_instance list>
  struct base_selector;
  template <size_t... i>
  struct base_selector<index_list<i...>> {
    using type = amalgamate<wrapper<i>...>;
  };

  using base = typename base_selector<as_type<indices>>::type;
};
template <typename... types>
struct my_tuple : tuple_detail<types...>::base {
  static consteval auto detail() noexcept {
    return as_value<tuple_detail<types...>>;
  }
};
template <typename type>
concept my_tuple_instance =
    matches<type, []<typename... types>(my_tuple<types...>) {
      return as_signature<true>;
    }>;
template <size_t index>
constexpr auto value(my_tuple_instance auto&& t) noexcept {
  using detail = as_type<std::decay_t<decltype(t)>::detail()>;
  return static_cast<detail::template wrapper<index>>(t).value();
}
constexpr auto t = my_tuple<int, float>{-1, 1.23f};
static_assert(value<0>(t) == -1);
static_assert(value<1>(t) == 1.23f);

template <typename... ts>
struct tuple_trait {
  static constexpr auto types = type_list<ts...>{};

  template <size_t index>
  struct wrapper {
    using type = as_type<element<index>(types)>;
    constexpr auto&& value(this auto&& self) noexcept {
      return std::forward<decltype(self)>(self).data;
    }
    type data;
  };

  template <size_t index>
    requires std::is_class_v<as_type<element<index>(types)>> &&
             (!std::is_final_v<as_type<element<index>(types)>>)
  struct wrapper<index> : as_type<element<index>(types)> {
    using type = as_type<element<index>(types)>;
    constexpr auto&& value(this auto&& self) noexcept {
      return static_cast<type&>(self);
    }
  };
};

template <typename type>
struct tuple_type_wrapper {
  constexpr auto&& value(this auto&& self) noexcept {
    return std::forward<decltype(self)>(self).data;
  }
  type data;
};
template <typename type>
  requires std::is_class_v<type> && (!std::is_final_v<type>)
struct tuple_type_wrapper<type> : type {
  // using type::type;
  constexpr auto&& value(this auto&& self) noexcept {
    return static_cast<type&>(self);
  }
};
// template <typename type>
// tuple_type_wrapper(type) -> tuple_type_wrapper<type>;
template <typename... types>
struct regular_tuple : tuple_type_wrapper<types>... {};
template <typename... types>
regular_tuple(types...) -> regular_tuple<types...>;

// tuple_type_wrapper wrap{[] {}};
constexpr auto test_tuple = regular_tuple{-1, 1.23f, [] {}};
static_assert(static_cast<tuple_type_wrapper<int>>(test_tuple).value() == -1);
static_assert(static_cast<tuple_type_wrapper<float>>(test_tuple).value() ==
              1.23f);
// static_assert(std::invoke(
//     static_cast<tuple_type_wrapper<decltype([] {})>>(test_tuple).value()));

static_assert(sizeof(test_tuple) == 8);
static_assert(alignof(decltype(test_tuple)) == 4);

struct A {};
struct B {
  char data;
};
struct C {};
struct D {
  int data;
};
// struct E : A, A {}; // compile error: duplicate base type
struct inheritance_test1 : A, B, C, D {};
struct inheritance_test2 : A, D {};
struct inheritance_test3 : B, C {};
struct inheritance_test4 : inheritance_test3, inheritance_test2 {};
static_assert(sizeof(A) == 1);
static_assert(alignof(A) == 1);
static_assert(sizeof(B) == sizeof(char));
static_assert(alignof(B) == alignof(char));
static_assert(sizeof(C) == 1);
static_assert(alignof(C) == 1);
static_assert(sizeof(D) == sizeof(int));
static_assert(alignof(D) == alignof(int));
static_assert(sizeof(inheritance_test1) == 2 * sizeof(D));
static_assert(alignof(inheritance_test1) == alignof(D));
static_assert(sizeof(inheritance_test2) == sizeof(D));
static_assert(alignof(inheritance_test2) == alignof(D));
static_assert(sizeof(inheritance_test3) == 1);
static_assert(alignof(inheritance_test3) == alignof(char));
static_assert(sizeof(inheritance_test4) == 2 * sizeof(D));
static_assert(alignof(inheritance_test4) == alignof(D));

constexpr auto test = inheritance_test1{{}, {1}, {}, {2}};
static_assert(static_cast<const B&>(test).data == 1);
static_assert(static_cast<const D&>(test).data == 2);
static_assert(
    std::same_as<decltype(static_cast<B>(inheritance_test1{}).data), char>);

int main() {
  // constexpr auto list = type_list<int, float, std::string, type_list<>>{};
  // print("{}\n", list);
  // constexpr auto vlist = value_list<1, 'c', 1.23f, "hello"_xs>{};
  // print("{}\n", vlist);
  // meta::print_type(list);

  //
  static_assert(as_value<int> == as_value<int>);
  static_assert(as_value<int&> == as_value<int&>);
  static_assert(as_value<int&&> == as_value<int&&>);
  static_assert(as_value<const int&> == as_value<const int&>);
  static_assert(as_value<const int&&> == as_value<const int&&>);
  //
  static_assert(as_value<int> != as_value<int&>);
  static_assert(as_value<int> != as_value<int&&>);
  static_assert(as_value<int> != as_value<const int&>);
  static_assert(as_value<int> != as_value<const int&&>);
  //
  static_assert(as_value<int&> != as_value<int>);
  static_assert(as_value<int&> != as_value<int&&>);
  static_assert(as_value<int&> != as_value<const int&>);
  static_assert(as_value<int&> != as_value<const int&&>);
  //
  static_assert(as_value<int&&> != as_value<int&>);
  static_assert(as_value<int&&> != as_value<int>);
  static_assert(as_value<int&&> != as_value<const int&>);
  static_assert(as_value<int&&> != as_value<const int&&>);
  //
  static_assert(as_value<const int&> != as_value<int&>);
  static_assert(as_value<const int&> != as_value<int&&>);
  static_assert(as_value<const int&> != as_value<int>);
  static_assert(as_value<const int&> != as_value<const int&&>);
  //
  int x = 10;
  int& y = x;
  // ensketch::xstd::meta::print_type<decltype(std::forward<char>(y))>();
  // ensketch::xstd::meta::print_type<decltype(std::forward<char&>(y))>();
  // ensketch::xstd::meta::print_type<decltype(std::forward<char&&>(y))>();
  // ensketch::xstd::meta::print_type<decltype(std::forward<const char&>(y))>();
  // ensketch::xstd::meta::print_type<decltype(std::forward<const char&&>(y))>();
  static_assert(as_value<decltype(x)> == as_value<int>);
}
