#pragma once
#include <ensketch/xstd/meta/utility.hpp>

namespace ensketch::xstd::meta {

/// The 'type_list' template is a template meta programming utility.
/// Within a 'type_list', packaging, accessing, and modifying types
/// originally given by variadic template parameters is possible.
/// All 'type_list' instances are empty and can be used as tag types.
///
template <typename... types>
struct type_list {};

// For tight constraints in template requirements,
// we want to be able to decide whether a given type is
// an instance of the 'type_list' template.
//
// The implementation is inside the 'detail' namespace and
// carried out by a struct template predicate specialization.
//
namespace detail {

template <typename type>
struct is_type_list : std::false_type {};

template <typename... types>
struct is_type_list<type_list<types...>> : std::true_type {};

}  // namespace detail

/// This is a concept that checks whether a given type
/// is an instance of the 'type_list' template.
///
template <typename type>
concept type_list_instance = detail::is_type_list<type>::value;

///
/// Representing Types by Values
///

/// Types cannot be returned by standard functions directly.
/// Instead, `type_wrapper` can be used to wrap a type
/// and generate a value by default construction.
/// Using this approach based on tag types,
/// allows to use types as simple values (also for comparison).
///
template <typename type>
using type_wrapper = type_list<type>;

namespace detail {

template <typename type>
struct is_type_wrapper : std::false_type {};

template <typename type>
struct is_type_wrapper<type_wrapper<type>> : std::true_type {};

}  // namespace detail

/// Checks whether given type is an instance of `type_wrapper`.
///
template <typename type>
concept type_wrapper_instance = detail::is_type_wrapper<type>::value;

/// Receive a type as value wrapped by `type_wrapper'.
///
template <typename type>
inline constexpr auto value = type_wrapper<type>{};

namespace detail {
template <typename type>
consteval auto unwrap(type_list<type>) -> type;
}  // namespace detail

/// Receive the wrapped type of a given `type_list` instance object.
///
template <type_wrapper_instance auto x>
using unwrap = decltype(detail::unwrap(x));

///
/// Equality and Inequality
///

/// Check whether two instances of 'type_list' are the same.
///
consteval auto operator==(type_list_instance auto x,
                          type_list_instance auto y) {
  return false;
}
//
template <type_list_instance list>
consteval auto operator==(list, list) {
  return true;
}

/// Check whether two instances of 'type_list' are not the same.
///
consteval auto operator!=(type_list_instance auto x,
                          type_list_instance auto y) {
  return !(x == y);
}

///
/// Accessors
///

/// Access a specific type of a 'type_list' instance by its index.
///
template <size_t index>
auto element(type_list<>) = delete;
//
template <size_t index, typename type, typename... types>
auto element(type_list<type, types...> list)
    -> decltype(element<index - 1>(type_list<types...>{}))
  requires((0 < index) && (index < size(list)));
//
template <size_t index, typename type, typename... types>
auto element(type_list<type, types...>) -> type
  requires(index == 0);

///
///
template <typename type>
consteval auto index(type_list_instance auto list) -> size_t
  requires(!empty(list)) && (is_set(list)) && (contains<type>(list))
{
  if constexpr (meta::equal<type, decltype(front(list))>)
    return 0;
  else
    return 1 + index<type>(--list);
}

/// Access a specific type of a 'type_list' instance by its index.
/// The result type is wrapped by the 'type_list' template
/// and returned as a slice of the given 'type_list' instance.
///
template <size_t index>
consteval auto slice(type_list_instance auto list) {
  return type_list<decltype(element<index>(list))>{};
}

/// Access the first element of a 'type_list' instance.
/// This function leads to compile errors
/// if the given 'type_list' instance is empty.
///
auto front(type_list_instance auto list) -> decltype(element<0>(list))
  requires(!empty(list));

/// Access the first element of a 'type_list' instance.
/// The result type is wrapped by the 'type_list' template
/// and returned as a slice of the given 'type_list' instance.
///
consteval auto front_slice(type_list_instance auto list)
  requires(!empty(list))
{
  return type_list<decltype(front(list))>{};
}
//
consteval auto operator*(type_list_instance auto list) {
  return front_slice(list);
}

/// Access the last element of a 'type_list' instance.
///
template <type_list_instance list>
auto back(list x) -> decltype(element<size(list{}) - 1>(x))
  requires(!empty(x));

/// Access the last element of a 'type_list' instance.
/// The result type is wrapped by the 'type_list' template
/// and returned as a slice of the given 'type_list' instance.
///
consteval auto back_slice(type_list_instance auto list)
  requires(!empty(list))
{
  return type_list<decltype(back(list))>{};
}
//
consteval auto operator!(type_list_instance auto list) {
  return back_slice(list);
}

///
/// Predicates
///

/// Returns the size of a given 'type_list' instance.
///
template <typename... types>
consteval auto size(type_list<types...>) -> size_t {
  return sizeof...(types);
}

template <typename functor, typename type>
concept generic_type_predicate = requires(functor f) {
  { f.template operator()<type>() } -> std::same_as<bool>;
};
template <typename functor, typename... types>
concept generic_types_predicate =
    (generic_type_predicate<functor, types> && ...);

template <typename functor, typename list>
concept generic_predicate =
    type_list_instance<list> && all_of<list>([]<typename type> {
      return generic_type_predicate<functor, type>;
    });

/// Check whether a condition provided by a 'constexpr' predicate
/// holds for all types inside a 'type_list' instance.
///
template <typename... types>
consteval auto for_all(type_list<types...>, auto f) {
  return (f.template operator()<types>() && ...);
}

/// Check whether a condition provided by a 'constexpr' predicate
/// holds for at least one type inside a 'type_list' instance.
///
template <typename... types>
consteval auto exists(type_list<types...>, auto f) {
  return (f.template operator()<types>() || ...);
}

/// Check whether a given 'type_list' instance contains no types.
///
consteval auto empty(type_list_instance auto list) {
  return size(list) == 0;
}

/// Check whether a given type is contained
/// inside a given 'type_list' instance.
///
// template <typename type, typename... types>
// consteval auto contains(type_list<types...>) {
//   return (meta::equal<type, types> || ...);
// }
template <typename type>
consteval auto contains(type_list_instance auto list) {
  return exists(list, []<typename t> { return meta::equal<t, type>; });
}

/// Check whether a given 'type_list' slice
/// is contained inside a 'type_list' instance.
///
template <typename type>
consteval auto contains(type_list_instance auto list, type_list<type>) {
  return contains<type>(list);
}

///
///
consteval bool is_set(type_list<>) {
  return true;
}
//
template <typename type, typename... types>
consteval bool is_set(type_list<type, types...>) {
  constexpr auto tail = type_list<types...>{};
  return !contains<type>(tail) && is_set(tail);
}

///
/// Modifiers
///

/// Returns a new instance of 'type_list' by adding
/// the given type to the front of the given 'type_list' instance.
///
template <typename type, typename... types>
consteval auto push_front(type_list<types...>) {
  return type_list<type, types...>{};
}

/// Returns a new instance of 'type_list' by adding
/// the given type to the back of the given 'type_list' instance.
///
template <typename type, typename... types>
consteval auto push_back(type_list<types...>) {
  return type_list<types..., type>{};
}

/// Returns a new instance of 'type_list' by
/// concatenating the two given 'type_list' instances.
///
template <typename... x, typename... y>
consteval auto concat(type_list<x...>, type_list<y...>) {
  return type_list<x..., y...>{};
}
//
consteval auto operator+(type_list_instance auto x, type_list_instance auto y) {
  return concat(x, y);
}

/// Remove the first element of a 'type_list' instance.
///
consteval auto pop_front(type_list<>) = delete;
//
template <typename type, typename... types>
consteval auto pop_front(type_list<type, types...>) {
  return type_list<types...>{};
}
//
consteval auto operator--(type_list_instance auto list) {
  return pop_front(list);
}

/// Remove the last element of a 'type_list' instance.
///
consteval auto pop_back(type_list<>) = delete;
//
template <typename type>
consteval auto pop_back(type_list<type>) {
  return type_list<>{};
}
//
consteval auto pop_back(type_list_instance auto list) {
  // return push_front<decltype(front(list))>(pop_back(pop_front(list)));
  return *list + pop_back(--list);
}
//
consteval auto operator--(type_list_instance auto list, int) {
  return pop_back(list);
}

/// Reverse the order of types inside a 'type_list' instance.
///
consteval auto reverse(type_list<> list) {
  return list;
}
//
consteval auto reverse(type_list_instance auto list) {
  // return push_back<decltype(front(list))>(reverse(pop_front(list)));
  return reverse(--list) + *list;
}
//
consteval auto operator~(type_list_instance auto list) {
  return reverse(list);
}

/// Insert a type at a given index into a 'type_list' instance;
///
template <size_t index, typename type>
consteval auto insert(type_list_instance auto list)
  requires(index <= size(list))
{
  if constexpr (index == 0)
    return push_front<type>(list);
  else
    // return push_front<decltype(front(list))>(
    //     insert<index - 1, type>(pop_front(list)));
    return *list + insert<index - 1, type>(--list);
}

/// Insert a type into a 'type_list' instance by using predicate.
///
template <typename type>
consteval auto insert(type_list<>, auto less) {
  return type_list<type>{};
}
//
template <typename type>
consteval auto insert(type_list_instance auto list, auto less) {
  if constexpr (less.template operator()<type, decltype(front(list))>())
    return push_front<type>(list);
  else
    // return push_front<decltype(front(list))>(
    //     insert<type>(pop_front(list), less));
    return *list + insert<type>(--list, less);
}

/// Remove a type at a given index from a 'type_list' instance.
///
template <size_t index>
consteval auto remove(type_list_instance auto list)
  requires(index < size(list))
{
  if constexpr (index == 0)
    // return pop_front(list);
    return --list;
  else
    // return push_front<decltype(front(list))>(
    //     remove<index - 1>(pop_front(list)));
    return *list + remove<index - 1>(--list);
}

/// Remove all types from a 'type_list' instance
/// for which the given predicate returns 'true'.
///
consteval auto remove(type_list<> list, auto f) {
  return list;
}
//
consteval auto remove(type_list_instance auto list, auto f) {
  if constexpr (f.template operator()<decltype(front(list))>())
    // return remove(pop_front(list), f);
    return remove(--list, f);
  else
    // return push_front<decltype(front(list))>(remove(pop_front(list), f));
    return *list + remove(--list, f);
}

/// Remove a given amount of types from the front of a 'type_list' instance.
///
template <size_t n>
  requires(n == 0)
consteval auto trim_front(type_list_instance auto list) {
  return list;
}
//
template <size_t n>
consteval auto trim_front(type_list_instance auto list)
  requires((0 < n) && (n <= size(list)))
{
  return trim_front<n - 1>(pop_front(list));
}

/// Remove a given amount of types from the back of a 'type_list' instance.
///
template <size_t n>
  requires(n == 0)
consteval auto trim_back(type_list_instance auto list) {
  return list;
}
//
template <size_t n>
consteval auto trim_back(type_list_instance auto list)
  requires((0 < n) && (n <= size(list)))
{
  return trim_back<n - 1>(pop_back(list));
}

/// Get a subrange of types from a 'type_list' instance.
///
template <size_t first, size_t last>
consteval auto range(type_list_instance auto list)
  requires((first <= last) && (last <= size(list)))
{
  constexpr auto n = size(list);
  return trim_front<first>(trim_back<n - last>(list));
}

/// Swap two types given by their position inside a 'type_list' instance.
///
template <size_t i, size_t j>
consteval auto swap(type_list_instance auto list)
  requires((i == j) && (j < size(list)))
{
  return list;
}
//
template <size_t i, size_t j>
consteval auto swap(type_list_instance auto list)
  requires((i < j) && (j < size(list)))
{
  constexpr auto n = size(list);
  return range<0, i>(list) + slice<j>(list) + range<i + 1, j>(list) +
         slice<i>(list) + range<j + 1, n>(list);
}
//
template <size_t i, size_t j>
consteval auto swap(type_list_instance auto list)
  requires((j < i) && (i < size(list)))
{
  return swap<j, i>(list);
}

/// Merge two sorted 'type_list' instances by using a 'less' predicate.
///
consteval auto merge(type_list_instance auto left,
                     type_list_instance auto right,
                     auto less)
  requires(empty(left) || empty(right))
{
  return left + right;
}
//
consteval auto merge(type_list_instance auto left,
                     type_list_instance auto right,
                     auto less) {
  if constexpr (less.template  //
                operator()<decltype(front(left)), decltype(front(right))>())
    return push_front<decltype(front(left))>(
        merge(pop_front(left), right, less));
  else
    return push_front<decltype(front(right))>(
        merge(left, pop_front(right), less));
}

/// Sort a 'type_list' instance by using a 'less' predicate.
///
consteval auto sort(type_list_instance auto list, auto less) {
  constexpr auto n    = size(list);
  constexpr auto half = n / 2;
  return merge(sort(range<0, half>(list), less),
               sort(range<half, n>(list), less), less);
}
//
consteval auto sort(type_list_instance auto list, auto less)
  requires(size(list) < 2)
{
  return list;
}

template <typename functor, typename type>
concept transform_type_to_wrapper = requires(functor f) {
  { f.template operator()<type>() } -> type_wrapper_instance;
};
template <typename functor, typename... types>
concept type_to_wrapper_transformer =
    (transform_type_to_wrapper<functor, types> && ...);

template <typename functor, typename type>
concept transform_slice = requires(functor f) {
  { f.template operator()<type>() } -> type_list_instance;
};
template <typename functor, typename... types>
concept slice_transformer = (transform_slice<functor, types> && ...);

// template <typename functor, typename... types>
// concept generic_types_transformer =
//     (generic_type_transform<functor, types> && ...);

// template <typename functor, typename list>
// concept generic_type_transformer =
//     type_list_instance<list> && for_all(list{}, []<typename type> {
//       return generic_type_transform<functor, type>;
//     });

// template <typename functor, size_t index, typename type>
// concept generic_pair_transform =
//     requires(functor f) { f.template operator()<index, type>(); };

// template <typename functor, typename list>
// concept generic_pair_transformer =
//     type_list_instance<list> && for_all(list{}, []<typename type> {
//       return generic_type_transform<functor, index, type>;
//     });

///
///
template <typename... types>
consteval auto transform(type_list<types...>,
                         slice_transformer<types...> auto f) {
  // f needs to return slices
  return (f.template operator()<types>() + ...);
}

// template <typename... types>
// consteval auto transform(type_list<types...>,
//                          type_to_wrapper_transformer<types...> auto f) {
//   return type_list<unwrap<f.template operator()<types>()>...> {};
// }

///
///
// template <typename... types>
// constexpr auto transform_and_fold(type_list<types...>,
//                                   auto&& transform,
//                                   auto&& fold) {
//   using result_type =
//       decltype(std::invoke(std::forward<decltype(fold)>(fold),
//                            transform.template operator()<types>()...));
//   if constexpr (meta::equal<void, result_type>)
//     std::invoke(std::forward<decltype(fold)>(fold),
//                 transform.template operator()<types>()...);
//   else
//     return std::invoke(std::forward<decltype(fold)>(fold),
//                        transform.template operator()<types>()...);
// }

///
///
template <template <typename...> typename x, typename... types>
consteval auto transform(type_list<types...>) {
  return meta::value<x<types...>>;
}

template <typename... types>
consteval auto pack_call(type_list<types...>, auto f) {
  return f.template operator()<types...>();
}

///
/// Algorithms
///

///
///
template <typename... types>
constexpr auto for_each(type_list<types...>, auto f) {
  (f.template operator()<types>(), ...);
}

///
///
constexpr auto for_each_until(type_list_instance auto list, auto f) {
  if constexpr (empty(list))
    return false;
  else {
    if (f.template operator()<decltype(front(list))>()) return true;
    return for_each_until(--list, std::forward<decltype(f)>(f));
  }
}

}  // namespace ensketch::xstd::meta
