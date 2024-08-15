#pragma once
#include <ensketch/xstd/meta/utility.hpp>

namespace ensketch::xstd::meta {

/// Checks whether a given type acts as type wrapper on another type.
///
template <typename type>
concept generic_type_wrapper = std::regular<type> && std::is_empty_v<type> &&
                               requires { typename type::type; };

/// `type_wrapper` is used to wrap any given type and
/// by default construction make it available as value.
/// With `type_wrapper`, types can be returned as values in functions.
///
template <typename x>
struct type_wrapper {
  using type = x;
};

namespace detail {

template <typename type>
struct is_type_wrapper : std::false_type {};

template <typename type>
struct is_type_wrapper<type_wrapper<type>> : std::true_type {};

}  // namespace detail

/// Checks whether given type is an instance of `type_wrapper`.
///
template <typename type>
concept type_wrapper_instance = detail::is_type_wrapper<type>::value;

/// Receive a type as value wrapped by `type_wrapper'.
///
template <typename type>
inline constexpr auto value = type_wrapper<type>{};

/// Receive the type of given `type_wrapper` instance object.
///
template <type_wrapper_instance auto x>
using unwrap = decltype(x)::type;

/// Checks whether two given `type_wrapper` instances contain the same type.
///
template <typename x, typename y>
consteval bool operator==(type_wrapper<x>, type_wrapper<y>) noexcept {
  return equal<x, y>;
}

/// Checks whether two given `type_wrapper` instances contain different types.
///
consteval bool operator!=(type_wrapper_instance auto x,
                          type_wrapper_instance auto y) noexcept {
  return !(x == y);
}

}  // namespace ensketch::xstd::meta
