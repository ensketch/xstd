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

namespace ensketch::xstd {

///
///
constexpr auto operator|(auto&& variant, match_instance auto&& matcher) {
  using result_type =
      decltype(std::visit(std::forward<decltype(matcher)>(matcher),
                          std::forward<decltype(variant)>(variant)));
  if constexpr (meta::not_void<result_type>)
    return std::visit(std::forward<decltype(matcher)>(matcher),
                      std::forward<decltype(variant)>(variant));
  else
    std::visit(std::forward<decltype(matcher)>(matcher),
               std::forward<decltype(variant)>(variant));
}

}  // namespace ensketch::xstd
