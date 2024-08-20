#pragma once
#include <ensketch/xstd/meta/value_list.hpp>

namespace ensketch::xstd::meta {

/// 'index_list' is a constrained alias of 'value_list'
/// that only contains values of type 'size_t'.
///
template <size_t... indices>
using index_list = value_list<indices...>;

namespace detail {

template <typename type>
struct is_index_list : std::false_type {};

template <size_t... indices>
struct is_index_list<index_list<indices...>> : std::true_type {};

}  // namespace detail

/// Check whether the given type is an instance of 'index_list'.
///
template <typename T>
concept index_list_instance = detail::is_index_list<T>::value;

///
/// Constructor Extensions
///

///
///
template <size_t size, size_t offset = 0>
  requires(size == 0)
consteval auto index_list_from_iota() {
  return index_list<>{};
}
//
template <size_t size, size_t offset = 0>
  requires(size > 0)
consteval auto index_list_from_iota() {
  return index_list<offset>{} + index_list_from_iota<size - 1, offset + 1>();
}

///
/// Predicates
///

/// Check whether each contained value inside the
/// index list is smaller than a given integer.
///
template <size_t bound>
consteval bool bounded(index_list_instance auto list) {
  return all_of(list, []<size_t x> { return x < bound; });
}

}  // namespace ensketch::xstd::meta
