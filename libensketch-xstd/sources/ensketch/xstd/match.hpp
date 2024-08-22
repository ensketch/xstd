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
#include <ensketch/xstd/utility.hpp>

namespace ensketch::xstd {

///
///
/// Type-Matching Visitor
/// See: https://en.cppreference.com/w/cpp/utility/variant/visit
///
template <typename... functors>
struct match : functors... {
  using functors::operator()...;
};

///
///
/// Must be implemented with `std::invoke_result_t` to use an unevaluated context.
/// Otherwise, more complicated types, such as `std::thread`, would not be usable.
///
template <typename type, auto... functors>
concept matches =
    std::invoke_result_t<decltype(match{
                             [](auto) { return meta::as_signature<false>; },
                             functors...}),
                         type>::value;

/// Check whether the given type is an instance of `match`.
/// This concept will also match qualified alternatives
/// as it will decay a given type before it is matched.
///
template <typename type>
concept match_instance =
    matches<type, []<typename... functors>(match<functors...>) {
      meta::as_signature<true>;
    }>;

}  // namespace ensketch::xstd
