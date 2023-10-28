#pragma once
#include <ensketch/xstd/utility.hpp>

namespace ensketch::xstd {

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

// We simplify the API by providing a respective concept
// inside the 'instance' namespace for this predicate.
//
namespace instance {

/// Check if a given type is an instance of the 'value_list' template.
///
template <typename type>
concept value_list = detail::is_value_list<type>::value;

}  // namespace instance

///
/// Ordering
///

/// Check whether two instances of 'value_list' are the same.
///
consteval auto operator==(instance::value_list auto x,
                          instance::value_list auto y) {
  return false;
}
//
template <instance::value_list list>
consteval auto operator==(list, list) {
  return true;
}

/// Check whether two instances of 'value_list' are not the same.
///
consteval auto operator!=(instance::value_list auto x,
                          instance::value_list auto y) {
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
consteval auto empty(instance::value_list auto list) {
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
consteval auto contains(instance::value_list auto list, auto value) {
  return exists(list, [value]<auto x> { return meta::strict_equal(x, value); });
}

consteval auto contains(auto value) {
  return [value](instance::value_list auto list) consteval {  //
    return contains(list, value);
  };
}

consteval auto operator|(auto list, auto f) {
  return f(list);
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
  return [index](instance::value_list auto list) consteval {
    return element<index>(list);
  };
}

/// Access the first element of a 'value_list' instance.
///
consteval auto front(instance::value_list auto list)
  requires(!empty(list))
{
  return element<0>(list);
}

/// Access the last element of a 'value_list' instance.
///
consteval auto back(instance::value_list auto list)
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
consteval auto operator+(instance::value_list auto x,
                         instance::value_list auto y) {
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
consteval auto operator--(instance::value_list auto list) {
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
consteval auto pop_back(instance::value_list auto list) {
  // return push_front<decltype(front(list))>(pop_back(pop_front(list)));
  return *list + pop_back(--list);
}
//
consteval auto operator--(instance::value_list auto list, int) {
  return pop_back(list);
}

/// Reverse the order of types inside a 'value_list' instance.
///
consteval auto reverse(value_list<> list) {
  return list;
}
//
consteval auto reverse(instance::value_list auto list) {
  return reverse(--list) + *list;
}
//
consteval auto operator~(instance::value_list auto list) {
  return reverse(list);
}

consteval auto reverse() {
  return [](instance::value_list auto list) consteval {  //
    return reverse(list);
  };
}

}  // namespace ensketch::xstd
