#pragma once
#include <ensketch/xstd/meta/string.hpp>
#include <ensketch/xstd/meta/value_list.hpp>

namespace ensketch::xstd::meta {

/// `string_list` is a constrained alias of `value list`
/// that only contains values of type `meta::string`.
///
template <string... strings>
using string_list = value_list<strings...>;

namespace detail {

template <typename type>
struct is_string_list : std::false_type {};

template <string... strings>
struct is_string_list<string_list<strings...>> : std::true_type {};

}  // namespace detail

/// Check whether the given type is an instance of `string_list`.
///
template <typename list>
concept string_list_instance = detail::is_string_list<list>::value;

}  // namespace ensketch::xstd::meta
