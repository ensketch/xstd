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
#include <cstdint>
#include <print>
#include <thread>
//
#include <ensketch/xstd/functional/match.hpp>
#include <ensketch/xstd/meta/type_list.hpp>

using namespace ensketch::xstd::meta;
using ensketch::xstd::match;
using ensketch::xstd::matches;

/// Provide simple `test_list` template and `test_list_match` concept.
/// `test_list` is a variadic template and `test_list_match`
/// returns `true` for any given `test_list` instance.
///
template <typename... types>
struct test_list {};
//
template <typename type>
concept test_list_match =
    matches<type, []<typename... types>(test_list<types...>) {
      return as_signature<true>;
    }>;

/// Test valid and invalid matches for `test_list_match`.
///
static_assert(test_list_match<test_list<>>);
static_assert(test_list_match<test_list<int>>);
static_assert(test_list_match<test_list<int, float>>);
static_assert(test_list_match<test_list<char, int, float>>);
//
static_assert(test_list_match<test_list<>&>);
static_assert(test_list_match<const test_list<>&>);
static_assert(test_list_match<test_list<>&&>);
static_assert(test_list_match<const test_list<>&&>);
//
static_assert(test_list_match<test_list<int>&>);
static_assert(test_list_match<const test_list<int>&>);
static_assert(test_list_match<test_list<int>&&>);
static_assert(test_list_match<const test_list<int>&&>);
//
static_assert(test_list_match<test_list<int, float>&>);
static_assert(test_list_match<const test_list<int, float>&>);
static_assert(test_list_match<test_list<int, float>&&>);
static_assert(test_list_match<const test_list<int, float>&&>);
//
static_assert(!test_list_match<type_list<>>);
static_assert(!test_list_match<type_list<int>>);
static_assert(!test_list_match<type_list<int, float>>);
static_assert(!test_list_match<type_list<char, int, float>>);
//
static_assert(!test_list_match<std::string>);
static_assert(!test_list_match<std::thread>);
//
static_assert(!test_list_match<int>);
static_assert(!test_list_match<float>);
static_assert(!test_list_match<char>);

struct non_copyable {
  non_copyable() = default;
  non_copyable(const non_copyable&) = delete;
  non_copyable& operator=(const non_copyable&) = delete;
  non_copyable(non_copyable&&) = default;
  non_copyable& operator=(non_copyable&&) = default;
};
struct non_movable {
  non_movable() = default;
  non_movable(const non_movable&) = delete;
  non_movable& operator=(const non_movable&) = delete;
  non_movable(non_movable&&) = delete;
  non_movable& operator=(non_movable&&) = delete;
};

// Prepared Matcher
//
constexpr auto test_list_matcher = match{
    // Allow every instance of `test_list`.
    []<typename... types>(test_list<types...>) { return as_signature<true>; },
    // Remove empty instances of `test_list`.
    [](test_list<>) { return as_signature<false>; },
    // Also, remove `test_list` instances with one type.
    []<typename x>(test_list<x>) { return as_signature<false>; },
};

// Another Prepared Matcher
//
constexpr auto std_type_matcher = match{
    // Allow lvalue references of `std::string`.
    [](std::string&) { return as_signature<true>; },
    // Allow rvalue references of `std::thread`.
    [](std::thread&&) { return as_signature<true>; },
};

/// Use a very complicated test matcher.
///
template <typename type>
concept test_match =
    matches<type,
            // Prepared Matcher
            //
            test_list_matcher,

            // In-Place Lambda Expressions
            //
            // Allow all instances of `type_list` with concepts.
            [](type_list_instance auto list) { return as_signature<true>; },
            // But remove `type_list` instances with two elements.
            []<type_list_instance list>(list)
              requires(size(list{}) == 2)
            { return as_signature<false>; },

            // Another Prepared Matcher
            //
            std_type_matcher,

            // In-Place Matcher
            match{
                // Allow const-lvalue references of `float`.
                [](const float&) { return as_signature<true>; },
                // Allow values of type `int`.
                [](int) { return as_signature<true>; },
                //
                [](non_movable) { return as_signature<true>; },
                //
                [](non_copyable) { return as_signature<true>; },
            }>;

/// Test valid and invalid matches for `test_match`.
/// Remember that functions will be decayed first.
///
static_assert(!test_match<test_list<>>);
static_assert(!test_match<test_list<int>>);
static_assert(!test_match<test_list<float>>);
static_assert(!test_match<test_list<char>>);
static_assert(test_match<test_list<int, float>>);
static_assert(test_match<test_list<char, int, float>>);
//
static_assert(test_match<type_list<>>);
static_assert(test_match<type_list<int>>);
static_assert(!test_match<type_list<int, float>>);
static_assert(test_match<type_list<char, int, float>>);
//
static_assert(!test_match<std::string>);
static_assert(test_match<std::string&>);
static_assert(!test_match<std::string&&>);
static_assert(!test_match<const std::string&>);
static_assert(!test_match<const std::string&&>);
//
static_assert(test_match<std::thread>);
static_assert(!test_match<std::thread&>);
static_assert(test_match<std::thread&&>);
static_assert(!test_match<const std::thread&>);
static_assert(!test_match<const std::thread&&>);
//
static_assert(test_match<int>);
static_assert(test_match<int&>);
static_assert(test_match<int&&>);
static_assert(test_match<const int&>);
static_assert(test_match<const int&&>);
//
static_assert(test_match<float>);
static_assert(test_match<float&>);
static_assert(test_match<float&&>);
static_assert(test_match<const float&>);
static_assert(test_match<const float&&>);
//
static_assert(!test_match<char>);
static_assert(!test_match<char&>);
static_assert(!test_match<char&&>);
static_assert(!test_match<const char&>);
static_assert(!test_match<const char&&>);
//
// Non-Movable Types cannot be matched by value.
//
static_assert(!test_match<non_movable>);
static_assert(!test_match<non_movable&>);
static_assert(!test_match<non_movable&&>);
static_assert(!test_match<const non_movable&>);
static_assert(!test_match<const non_movable&&>);
//
// For non-copyable but movable types,
// only values and rvalues can be captured.
//
static_assert(test_match<non_copyable>);
static_assert(!test_match<non_copyable&>);
static_assert(test_match<non_copyable&&>);
static_assert(!test_match<const non_copyable&>);
static_assert(!test_match<const non_copyable&&>);

// Non-movable and non-copyable types can
// be captured by using a const reference.
//
template <typename type>
concept matches_all =
    matches<type,
            [](const non_copyable&) { return as_signature<true>; },
            [](const non_movable&) { return as_signature<true>; }>;
//
static_assert(matches_all<non_movable>);
static_assert(matches_all<non_movable&>);
static_assert(matches_all<non_movable&&>);
static_assert(matches_all<const non_movable&>);
static_assert(matches_all<const non_movable&&>);
//
static_assert(matches_all<non_copyable>);
static_assert(matches_all<non_copyable&>);
static_assert(matches_all<non_copyable&&>);
static_assert(matches_all<const non_copyable&>);
static_assert(matches_all<const non_copyable&&>);

// enum class error { negative };
// using result = std::variant<float, error>;
// auto calc(float x) -> result {
//   if (x < 0.0f) return error::negative;
//   return x;
// }

// int main() {
// calc(1.0f) | match{[](error e) {
//                      std::print("ERROR: The given number was negative.\n");
//                    },
//                    [](float x) {
//                      std::print("The given number is {} and valid.\n", x);
//                    }};

// calc(-1.0f) |
//     match{
//         [](error e) {
//           std::print("ERROR: The given number was negative.\n");
//         },
//         [](float x) { std::print("The given number is {} and valid.\n", x); },
//     };
// }
