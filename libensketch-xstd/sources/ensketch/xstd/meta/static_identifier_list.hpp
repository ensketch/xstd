#pragma once
#include <ensketch/xstd/meta/static_zstring_list.hpp>

namespace ensketch::xstd::meta {

/// Static identifier lists are value lists
/// that contain only unique static zstrings.
///
template <static_zstring... strings>
  requires(is_set(static_zstring_list<strings...>{}))  //
using static_identifier_list = static_zstring_list<strings...>;

namespace detail {
template <typename type>
struct is_static_identifier_list : std::false_type {};
template <static_zstring... strings>
struct is_static_identifier_list<static_identifier_list<strings...>>
    : std::true_type {};
}  // namespace detail

namespace instance {

/// This concept checks whether the given type
/// is an instance of the 'static_identifier_list' template.
///
template <typename list>
concept static_identifier_list = detail::is_static_identifier_list<list>::value;

}  // namespace instance

}  // namespace ensketch::xstd::meta
