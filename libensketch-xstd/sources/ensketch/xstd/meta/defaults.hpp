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
#include <ensketch/xstd/defaults.hpp>
//
#include <compare>
#include <concepts>
#include <functional>
#include <type_traits>
#include <typeinfo>

namespace ensketch::xstd {

namespace meta {

/// These two functions result in a compiler
/// error but also show the instantiated types.
/// This approach offers insights into compile-time issues,
/// so they are useful during development and debugging.
///
template <typename type>
void breakpoint() = delete;
//
template <typename type>
void breakpoint(type) = delete;

template <typename x, typename y>
concept equal = std::same_as<x, y>;

}  // namespace meta

/// Check whether `type` can be invoked with values of type `params...`.
///
template <typename type, typename... params>
concept invocable = std::is_invocable_v<type, params...>;

/// Check whether `type` can be invoked with values of type
/// `params...` and is known not to throw any exceptions.
///
template <typename type, typename... params>
concept nothrow_invocable = xstd::invocable<type, params...> &&
                            std::is_nothrow_invocable_v<type, params...>;

/// Check whether `type` can be invoked with values of type `params...`
/// and returns a value that is implicitly convertible to `result`.
///
template <typename type, typename result, typename... params>
concept invocable_r = xstd::invocable<type, params...> &&
                      std::is_invocable_r_v<result, type, params...>;

/// Check whether `type` can be invoked with values of type `params...`,
/// returns a value that is implicitly convertible to `result`,
/// and is known not to throw any exceptions.
///
template <typename type, typename result, typename... params>
concept nothrow_invocable_r =
    std::is_nothrow_invocable_r_v<result, type, params...>;

/// Check whether `type` can be invoked with values of type `params...`
/// and returns a value whose type is equal to `result`.
///
template <typename type, typename result, typename... params>
concept strict_invocable_r =
    xstd::invocable<type, params...> &&
    std::same_as<result, std::invoke_result_t<type, params...>>;

/// Check whether `type` can be invoked with values of type `params...`,
/// returns a value whose type is equal to `result`,
/// and is known not to throw any exceptions.
///
template <typename type, typename result, typename... params>
concept strict_nothrow_invocable_r =
    xstd::nothrow_invocable<type, params...> &&
    std::same_as<result, std::invoke_result_t<type, params...>>;

/// This concepts checks whether 'x' is reducible to 'y'.
/// That is, by decaying/reducing 'x' we get 'y'.
///
template <typename x, typename y>
concept decays_to = std::same_as<std::decay_t<x>, y>;

/// This concepts checks whether a given type is irreducible.
/// That is, applying a decay/reduction to that type does not change it.
///
template <typename type>
concept non_decayable = decays_to<type, type>;

/// Check whether a given type is not the same as another.
///
template <typename type, typename x>
concept different_to = !std::same_as<type, x>;

/// Check whether a given type is not `void`.
///
template <typename type>
concept not_void = different_to<type, void>;

/// This function checks whether two provided values are
/// strictly equal, meaning their type and values coincide.
///
constexpr bool strict_equal(auto x, auto y) noexcept {
  return false;
}
//
template <typename type>
constexpr bool strict_equal(type x, type y) noexcept {
  return x == y;
}

namespace detail {

template <typename from, typename to>
struct propagate_const_ref {
  using type = to;
};

template <typename from, typename to>
struct propagate_const_ref<from&, to> {
  using type = to&;
};

template <typename from, typename to>
struct propagate_const_ref<from const&, to> {
  using type = std::add_lvalue_reference_t<std::add_const_t<to>>;
};

template <typename from, typename to>
struct propagate_const_ref<from&&, to> {
  using type = to&&;
};

template <typename from, typename to>
struct propagate_const_ref<from const&&, to> {
  using type = std::add_rvalue_reference_t<std::add_const_t<to>>;
};

}  // namespace detail

template <typename from, typename to>
using propagate_const_ref =
    typename detail::propagate_const_ref<from, to>::type;

}  // namespace ensketch::xstd
