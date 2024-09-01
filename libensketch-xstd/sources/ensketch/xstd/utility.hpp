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
#include <ensketch/xstd/meta.hpp>

namespace ensketch::xstd {

/// This concepts checks whether two given types are equal.
///
template <typename x, typename y>
concept generic_identical = meta::equal<x, y>;

/// This concepts checks whether 'x' is reducible to 'y'.
/// That is, by decaying/reducing 'x' we get 'y'.
///
template <typename x, typename y>
concept generic_reducible = generic_identical<meta::reduction<x>, y>;

/// This concepts checks whether a given type is irreducible.
/// That is, applying a decay/reduction to that type does not change it.
///
template <typename x>
concept generic_irreducible = generic_reducible<x, x>;

/// This concepts checks whether a given type is a tag type, i.e. an empty type.
///
template <typename type>
concept generic_tag = std::is_empty_v<type>;

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
