#pragma once
#include <ensketch/xstd/meta/string_list.hpp>

namespace ensketch::xstd::meta {

/// Name lists are value lists whose values are strings
/// that are unique among all contained strings.
///
template <static_zstring... strings>
  requires(is_set(string_list<strings...>{}))  //
using name_list = string_list<strings...>;

namespace detail {
template <typename type>
struct is_name_list : std::false_type {};
template <static_zstring... strings>
struct is_name_list<name_list<strings...>> : std::true_type {};
}  // namespace detail

/// This concept checks whether the given type
/// is an instance of the 'name_list' template.
///
template <typename list>
concept name_list_instance = detail::is_name_list<list>::value;

}  // namespace ensketch::xstd::meta
