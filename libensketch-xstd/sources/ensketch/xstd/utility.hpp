#pragma once
#include <ensketch/xstd/meta/meta.hpp>

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

}  // namespace ensketch::xstd
