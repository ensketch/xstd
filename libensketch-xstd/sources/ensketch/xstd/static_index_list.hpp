#pragma once
#include <ensketch/xstd/value_list.hpp>

namespace ensketch::xstd {

/// The 'static_index_list' template is an alias of 'value_list'.
/// Additionally, it requires that every contained value is of type 'size_t'.
///
template <size_t... indices>
using static_index_list = value_list<indices...>;

namespace detail {
template <typename type>
struct is_static_index_list : std::false_type {};
template <size_t... indices>
struct is_static_index_list<static_index_list<indices...>> : std::true_type {};
}  // namespace detail

namespace instance {

/// This concept checks whether the given type
/// is an instance of the 'static_index_list' template.
///
template <typename T>
concept static_index_list = detail::is_static_index_list<T>::value;

}  // namespace instance

///
/// Predicates
///

/// Check whether each contained inside a
/// static index list is smaller than the given integer.
///
template <size_t bound>
consteval bool bounded(instance::static_index_list auto list) {
  return for_all(list, []<size_t x> { return x < bound; });
}

}  // namespace ensketch::xstd
