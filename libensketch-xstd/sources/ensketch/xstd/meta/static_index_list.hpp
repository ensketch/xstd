#pragma once
#include <ensketch/xstd/meta/value_list.hpp>

namespace ensketch::xstd::meta {

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
/// Constructor Extensions
///

///
///
template <size_t size, size_t offset = 0>
  requires(size == 0)
consteval auto static_index_list_from_iota() {
  return static_index_list<>{};
}
//
template <size_t size, size_t offset = 0>
  requires(size > 0)
consteval auto static_index_list_from_iota() {
  return static_index_list<offset>{} +
         static_index_list_from_iota<size - 1, offset + 1>();
}

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

}  // namespace ensketch::xstd::meta
