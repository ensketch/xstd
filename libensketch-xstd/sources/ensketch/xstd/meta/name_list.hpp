#pragma once
#include <ensketch/xstd/meta/string_list.hpp>

namespace ensketch::xstd::meta {

/// `name_list` is a constrained alias for `value_list`
/// that only contains values of type `meta::string`
/// which are unique among all contained values.
///
template <string... strings>
  requires(elementwise_unique(string_list<strings...>{}))
using name_list = string_list<strings...>;

namespace detail {

template <typename type>
struct is_name_list : std::false_type {};

template <string... strings>
struct is_name_list<name_list<strings...>> : std::true_type {};

}  // namespace detail

/// Checks whether the given type is an instance of 'name_list'.
///
template <typename list>
concept name_list_instance = detail::is_name_list<list>::value;

}  // namespace ensketch::xstd::meta
