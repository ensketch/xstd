#pragma once
#include <ensketch/xstd/detail/static_radix_tree/node.hpp>

namespace ensketch::xstd {

///
///
template <detail::static_radix_tree::instance::node node =
              detail::static_radix_tree::node<"">>
struct static_radix_tree {
  using root = node;

  constexpr static_radix_tree() noexcept = default;
  constexpr static_radix_tree(root) noexcept {}
};

namespace detail {
template <typename type>
struct is_static_radix_tree : std::false_type {};
template <typename type>
struct is_static_radix_tree<xstd::static_radix_tree<type>> : std::true_type {};
}  // namespace detail

namespace instance {

template <typename type>
concept static_radix_tree = detail::is_static_radix_tree<type>::value;

}  // namespace instance

///
/// Ordering
///

/// Check whether two instances of 'static_radix_tree' are the same.
///
consteval auto operator==(instance::static_radix_tree auto x,
                          instance::static_radix_tree auto y) {
  return false;
}
//
template <instance::static_radix_tree tree>
consteval auto operator==(tree, tree) {
  return true;
}

/// Check whether two instances of 'static_radix_tree' are not the same.
///
consteval auto operator!=(instance::static_radix_tree auto x,
                          instance::static_radix_tree auto y) {
  return !(x == y);
}

///
/// Accessors
///

consteval auto root(instance::static_radix_tree auto tree) {
  return typename decltype(tree)::root{};
}

///
/// Modifiers
///

template <static_zstring... str>
consteval auto insert(instance::static_radix_tree auto tree) {
  return static_radix_tree{insert<str...>(root(tree))};
}

///
///
template <static_zstring... str>
consteval auto static_radix_tree_from() {
  return insert<str...>(static_radix_tree<>{});
}

///
/// Algorithms
///

///
///
constexpr auto visit(instance::static_radix_tree auto tree,
                     czstring str,
                     auto&& f) {
  return visit(root(tree), str, std::forward<decltype(f)>(f));
}

///
///
constexpr auto traverse(instance::static_radix_tree auto tree,
                        czstring str,
                        auto&& f) {
  return traverse(root(tree), str, std::forward<decltype(f)>(f));
}

}  // namespace ensketch::xstd
