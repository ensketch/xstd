#pragma once
#include <ensketch/xstd/meta/string.hpp>
#include <ensketch/xstd/meta/value_list.hpp>

namespace ensketch::xstd::meta {

/// Static zstring lists are value lists
/// that contain only static zstrings.
/// No inheritance is used, because
/// all value list meta functions still need to be applyable.
///
template <string... strings>
using string_list = value_list<strings...>;

namespace detail {
template <typename type>
struct is_string_list : std::false_type {};
template <string... strings>
struct is_string_list<string_list<strings...>> : std::true_type {};
}  // namespace detail

/// The concept checks whether the given type
/// is a static zstring list.
///
template <typename list>
concept string_list_instance = detail::is_string_list<list>::value;

}  // namespace ensketch::xstd::meta
