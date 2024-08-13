#pragma once
#include <ensketch/xstd/meta/detail/radix_tree/node.hpp>
#include <ensketch/xstd/meta/string_list.hpp>

namespace ensketch::xstd::meta {

/// Instances of the 'radix_tree' template
/// are tag types that represent a radix tree.
/// Each instance provides access to the root of the tree.
/// A static radix tree must be constructed at compile time
/// and cannot change during runtime.
/// However, by using algorithms 'visit' and 'traverse',
/// it is possible to check at runtime whether a dynamic string
/// is fully contained or a prefix matches inside the radix tree.
///
template <detail::radix_tree::node_instance node = detail::radix_tree::node<"">>
struct radix_tree {
  using root = node;

  constexpr radix_tree() noexcept = default;

  // This constructor allows for CTAD.
  //
  constexpr radix_tree(root) noexcept {}
};

namespace detail {
template <typename type>
struct is_radix_tree : std::false_type {};
template <typename type>
struct is_radix_tree<meta::radix_tree<type>> : std::true_type {};
}  // namespace detail

///
///
template <typename type>
concept radix_tree_instance = detail::is_radix_tree<type>::value;

///
/// Constructor Extensions
///

///
///
template <string... str>
consteval auto radix_tree_from() {
  return insert<str...>(radix_tree<>{});
}

///
///
template <string... str>
consteval auto radix_tree_from(string_list<str...>) {
  return radix_tree_from<str...>();
}

///
/// Ordering
///

/// Check whether two instances of 'radix_tree' are the same.
///
consteval auto operator==(radix_tree_instance auto x,
                          radix_tree_instance auto y) {
  return false;
}
//
template <radix_tree_instance tree>
consteval auto operator==(tree, tree) {
  return true;
}

/// Check whether two instances of 'radix_tree' are not the same.
///
consteval auto operator!=(radix_tree_instance auto x,
                          radix_tree_instance auto y) {
  return !(x == y);
}

///
/// Accessors
///

consteval auto root(radix_tree_instance auto tree) {
  return typename decltype(tree)::root{};
}

///
/// Modifiers
///

template <string... str>
consteval auto insert(radix_tree_instance auto tree) {
  return radix_tree{insert<str...>(root(tree))};
}

///
/// Algorithms
///

///
///
constexpr auto visit(radix_tree_instance auto tree, czstring str, auto&& f) {
  return visit(root(tree), str, std::forward<decltype(f)>(f));
}

///
///
constexpr auto traverse(radix_tree_instance auto tree, czstring str, auto&& f) {
  return traverse(root(tree), str, std::forward<decltype(f)>(f));
}

}  // namespace ensketch::xstd::meta
