#pragma once
#include <tuple>
//
#include <ensketch/xstd/meta/static_index_list.hpp>
#include <ensketch/xstd/meta/type_list.hpp>

namespace ensketch::xstd {

// Multiple tuple structures can exist for various reasons,
// each significant in different situations.
// A concept for generic tuples is introduced,
// utilizing structured bindings requirements for generalization.

namespace generic {

///
///
template <typename tuple_type, size_t index>
concept tuple_get_access = requires(tuple_type value) {
  {
    get<index>(value)
  }
  -> std::convertible_to<typename std::tuple_element<index, tuple_type>::type>;
};

/// Checks whether a given type fulfills the requirements of a generic tuple.
///
template <typename tuple_type>
concept tuple = for_all(
    meta::static_index_list_from_iota<std::tuple_size<tuple_type>::value>(),
    []<size_t index> { return tuple_get_access<tuple_type, index>; });

///
///
template <typename T>
concept reducible_tuple = tuple<reduction<T>>;

///
///
template <typename tuple_type, size_t index>
concept xstd_tuple_value_access = requires(tuple_type tuple) {
  {
    value<index>(tuple)
  } -> std::convertible_to<typename tuple_type::template type<index>>;
  // Only ask for specific type.
  // For an array, you would not want to provide a type list.
};

///
///
template <typename tuple_type>
concept xstd_tuple = for_all(
    meta::static_index_list_from_iota<tuple_type::size()>(),  //
    []<size_t index> { return xstd_tuple_value_access<tuple_type, index>; });

///
///
template <typename tuple_type>
concept reducible_xstd_tuple = xstd_tuple<reduction<tuple_type>>;

}  // namespace generic

///
///
// A tuple type is not a tag type and as such should be given as template argument.
template <generic::tuple tuple_type, size_t... indices>
consteval auto type_list_from(meta::static_index_list<indices...>) {
  return meta::type_list<
      typename std::tuple_element<indices, tuple_type>::type...>{};
}
//
template <generic::tuple tuple_type>
consteval auto type_list_from() {
  return type_list_from<tuple_type>(
      meta::static_index_list_from_iota<std::tuple_size<tuple_type>::value>());
}

///
///
template <size_t... indices>
constexpr auto for_each(generic::reducible_tuple auto&& tuple,
                        auto&& f,
                        meta::static_index_list<indices...>) {
  (f(get<indices>(std::forward<decltype(tuple)>(tuple))), ...);
}
//
constexpr auto for_each(generic::reducible_tuple auto&& tuple, auto&& f) {
  using tuple_type = meta::reduction<decltype(tuple)>;
  for_each(
      std::forward<decltype(tuple)>(tuple), std::forward<decltype(f)>(f),
      meta::static_index_list_from_iota<std::tuple_size<tuple_type>::value>());
}

}  // namespace ensketch::xstd
