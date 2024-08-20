#pragma once
#include <ensketch/xstd/std_defaults.hpp>

namespace ensketch::xstd::meta {

/// When either of these two functions is employed,
/// it will result in a compiler error,
/// while simultaneously providing the beneficial outcome
/// of displaying their instantiated types.
/// Although not entirely precise,
/// this approach can offer valuable insights into compile-time issues.
/// Therefore, they are primarily useful during the development and debugging phases.
///
template <typename type>
constexpr auto print_type() = delete;
//
template <typename type>
constexpr auto print_type(type) = delete;

/// This meta function is by definition the same as 'std::decay_t'
/// and returns the decayed type.
///
template <typename type>
using reduction = std::decay_t<type>;

/// This meta function is by definition the same as 'std::same_as'
/// and checks whether two given types are equal.
///
template <typename x, typename y>
constexpr bool equal = std::same_as<x, y>;

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

}  // namespace ensketch::xstd::meta
