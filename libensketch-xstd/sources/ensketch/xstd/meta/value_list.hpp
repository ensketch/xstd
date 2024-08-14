#pragma once
#include <ensketch/xstd/meta/utility.hpp>

namespace ensketch::xstd::meta {

/// The template 'value_list' is
/// a utility for template meta programming.
/// Within a 'value_list' instance,
/// packaging, accessing, and modifying static values
/// originally given by variadic template parameters is possible.
/// All instances are empty and can be used as tag types.
///
template <auto... values>
struct value_list {};

// For tight constraints in template requirements,
// we want to be able to decide whether a given type is
// an instance of the 'value_list' template.
//
// The implementation is taken inside the 'details' namespace
// by a typical struct template predicate specialization.
//
namespace detail {

template <typename type>
struct is_value_list : std::false_type {};

template <auto... values>
struct is_value_list<value_list<values...>> : std::true_type {};

}  // namespace detail

/// Check if a given type is an instance of the 'value_list' template.
///
template <typename type>
concept value_list_instance = detail::is_value_list<type>::value;

///
/// Ordering
///

/// Check whether two instances of 'value_list' are the same.
///
consteval auto operator==(value_list_instance auto x,
                          value_list_instance auto y) {
  return false;
}
//
template <value_list_instance list>
consteval auto operator==(list, list) {
  return true;
}

/// Check whether two instances of 'value_list' are not the same.
///
consteval auto operator!=(value_list_instance auto x,
                          value_list_instance auto y) {
  return !(x == y);
}

///
/// Predicates
///

/// Returns the size of a given 'value_list' instance.
///
template <auto... values>
consteval auto size(value_list<values...>) -> size_t {
  return sizeof...(values);
}

/// Check whether a given 'value_list' instance contains no types.
///
consteval auto empty(value_list_instance auto list) {
  return size(list) == 0;
}

/// Check whether a condition provided by a 'constexpr' predicate
/// holds for all values inside a 'value_list' instance.
///
template <auto... values>
consteval auto for_all(value_list<values...>, auto f) {
  return (f.template operator()<values>() && ...);
}

/// Check whether a condition provided by a 'constexpr' predicate
/// holds for at least one value inside a 'value_list' instance.
///
template <auto... values>
consteval auto exists(value_list<values...>, auto f) {
  return (f.template operator()<values>() || ...);
}

/// Check whether a given value is contained
/// inside a given 'value_list' instance.
///
template <auto value>
consteval bool contains(value_list_instance auto list) {
  return exists(list, []<auto x> { return meta::strict_equal(x, value); });
}

consteval auto contains(value_list_instance auto list, auto value) {
  return exists(list, [value]<auto x> { return meta::strict_equal(x, value); });
}

consteval auto contains(auto value) {
  return [value](value_list_instance auto list) consteval {  //
    return contains(list, value);
  };
}

consteval auto operator|(auto list, auto f) {
  return f(list);
}

///
///
consteval bool is_set(value_list<>) {
  return true;
}
//
template <auto x, auto... values>
consteval bool is_set(value_list<x, values...>) {
  constexpr auto tail = value_list<values...>{};
  return !contains<x>(tail) && is_set(tail);
}

///
/// Accessors
///

/// Access a specific value of a 'value_list' instance by its index.
///
template <size_t index, auto value, auto... values>
consteval auto element(value_list<value, values...> list)
  requires((0 < index) && (index < size(list)))
{
  return element<index - 1>(value_list<values...>{});
}
//
template <size_t index, auto value, auto... values>
consteval auto element(value_list<value, values...>)
  requires(index == 0)
{
  return value;
}

consteval auto element(size_t index) {
  return [index](value_list_instance auto list) consteval {
    return element<index>(list);
  };
}

///
///
///
///
template <auto value>
consteval auto index(value_list_instance auto list) -> size_t
  requires(!empty(list)) && (is_set(list)) && (contains<value>(list))
{
  if constexpr (meta::strict_equal(value, front(list)))
    return 0;
  else
    return 1 + index<value>(--list);
}

/// Access the first element of a 'value_list' instance.
///
consteval auto front(value_list_instance auto list)
  requires(!empty(list))
{
  return element<0>(list);
}

/// Access the last element of a 'value_list' instance.
///
consteval auto back(value_list_instance auto list)
  requires(!empty(list))
{
  return element<size(list) - 1>(list);
}

///
/// Modifiers
///

/// Returns a new instance of 'value_list'
/// by adding a given value to the front of the given 'value_list' instance.
///
template <auto value, auto... values>
consteval auto push_front(value_list<values...>) {
  return value_list<value, values...>{};
}

/// Returns a new instance of 'value_list'
/// by adding a given value to the front of the given 'value_list' instance.
///
template <auto value, auto... values>
consteval auto push_back(value_list<values...>) {
  return value_list<values..., value>{};
}

/// Concatenate two given 'value_list' instances.
///
template <auto... x, auto... y>
consteval auto concat(value_list<x...>, value_list<y...>) {
  return value_list<x..., y...>{};
}
//
consteval auto operator+(value_list_instance auto x,
                         value_list_instance auto y) {
  return concat(x, y);
}

/// Remove the first element of a 'value_list' instance.
///
consteval auto pop_front(value_list<>) = delete;
//
template <auto value, auto... values>
consteval auto pop_front(value_list<value, values...>) {
  return value_list<values...>{};
}
//
consteval auto operator--(value_list_instance auto list) {
  return pop_front(list);
}

/// Remove the last element of a 'value_list' instance.
///
consteval auto pop_back(value_list<>) = delete;
//
template <auto value>
consteval auto pop_back(value_list<value>) {
  return value_list<>{};
}
//
consteval auto pop_back(value_list_instance auto list) {
  // return push_front<decltype(front(list))>(pop_back(pop_front(list)));
  return *list + pop_back(--list);
}
//
consteval auto operator--(value_list_instance auto list, int) {
  return pop_back(list);
}

/// Reverse the order of types inside a 'value_list' instance.
///
consteval auto reverse(value_list<> list) {
  return list;
}
//
consteval auto reverse(value_list_instance auto list) {
  return reverse(--list) + *list;
}
//
consteval auto operator~(value_list_instance auto list) {
  return reverse(list);
}

consteval auto reverse() {
  return [](value_list_instance auto list) consteval {  //
    return reverse(list);
  };
}

///
/// Algorithms
///

template <auto... values>
constexpr void for_each(value_list<values...>, auto&& f) {
  (f.template operator()<values>(), ...);
}

}  // namespace ensketch::xstd::meta
