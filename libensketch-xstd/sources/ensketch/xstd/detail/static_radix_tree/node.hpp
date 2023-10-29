#pragma once
#include <ensketch/xstd/static_zstring.hpp>
#include <ensketch/xstd/type_list.hpp>

namespace ensketch::xstd::detail::static_radix_tree {

// A tree is typically defined as a recursive data structure.
// So, we would like to offer recursive constraints and concepts.
// For this, we pre-declare the template predicate which decides
// whether a given type is a node of a static radix tree.
//
template <typename type>
struct is_node_implementation;

/// This type function checks whether the given
/// type is an instance of the 'node' template.
///
template <typename type>
constexpr bool is_node = is_node_implementation<type>::value;

/// This type function checks whether a given type
/// is an instance of 'type_list' and all contained types
/// are instances of the 'node' template.
///
template <typename list>
constexpr bool is_node_list =
    instance::type_list<list> &&
    for_all(list{}, []<typename type> { return is_node<type>; });

namespace instance {

/// This concept checks whether a given type is an instance of 'node'.
///
template <typename T>
concept node = is_node<T>;

/// This concept checks whether a given type is a 'type_list' instance
/// such that all contained types are instances of the 'node' template.
///
template <typename list>
concept node_list = is_node_list<list>;

}  // namespace instance

/// A node list is a 'type_list' instance that
/// contains only instances of the 'node' template.
///
template <instance::node... nodes>
using node_list = type_list<nodes...>;

/// The actual 'node' template can be defined
/// with correct and recursive constraints.
///
template <static_zstring str,
          instance::node_list nodes = node_list<>,
          bool leaf                 = false>
struct node {
  static constexpr static_zstring string = str;
  using children                         = nodes;
  static constexpr bool is_leaf          = leaf;
};

/// 'leaf' is an alias to the 'node' template.
/// It is used to simplify marking nodes as leaves.
///
template <static_zstring str, instance::node_list children = node_list<>>
using leaf = node<str, children, true>;

///
/// Constructor Extensions
///

///
///
template <static_zstring str, bool is_leaf = false>
consteval auto node_from(instance::node_list auto nodes) {
  return node<str, decltype(nodes), is_leaf>{};
}

///
///
template <static_zstring str>
consteval auto leaf_from(instance::node_list auto nodes) {
  return node_from<str, true>(nodes);
}

///
///
template <instance::node... nodes>
consteval auto node_list_from(nodes...) {
  return node_list<nodes...>{};
}

///
/// Ordering
///

///
/// Accessors
///

///
///
consteval auto prefix(instance::node auto root) {
  return decltype(root)::string;
}

///
///
consteval auto children(instance::node auto root) {
  return typename decltype(root)::children{};
}

///
///
consteval auto is_leaf(instance::node auto root) {
  return decltype(root)::is_leaf;
}

// namespace detail {

// // We need to use standard type list helper functions.
// // Therefore we make the appropriate namespace available
// // inside this implementation namespace.
// // using namespace meta::type_list;

// // The basic insertion shall insert a
// // single string into the given radix tree.
// //
// template <instance::node root, static_zstring str>
// struct basic_insertion;

// // The actual insertion method will allow to insert
// // an arbitrary amount of strings into the given radix tree.
// //
// template <instance::node root, static_zstring... str>
// struct insertion {
//   // If no strings are provided,
//   // the meta function will be an identity.
//   using type = root;
// };
// //
// // In all other cases, the first string is handled by the basic insertion
// // and the following strings again by the insertion method.
// //
// template <instance::node root, static_zstring str, static_zstring... tail>
// struct insertion<root, str, tail...> {
//   using new_root = typename basic_insertion<root, str>::type;
//   using type     = typename insertion<new_root, tail...>::type;
// };

// // For the implementation of the basic insertion mechanism,
// // it is convenient to provide an additional implementation template
// // for which the prefix match index has already been calculated.
// //
// template <instance::node root, static_zstring str, size_t index>
// struct basic_insertion_implementation;
// //
// template <instance::node root, static_zstring str>
// struct basic_insertion {
//   static constexpr size_t index = prefix_match_index(root::string, str);
//   using type = typename basic_insertion_implementation<root, str, index>::type;
// };

// // The actual implementation of the insertion routine is done recursively.
// // For this, five case have to be taken into account concerning
// // the given string and the node string and their prefix match index.
// //
// // 1. No Match:
// //    Both strings do not match and the index is zero.
// // 2. Full Match:
// //    Both strings are the same and the prefix match index is their length.
// // 3. Node Match:
// //    The node string is fully matched as a prefix by the string.
// //    The prefix match index will be the size of the node string.
// // 4. String Match:
// //    The string is fully matched as a prefix by the node string.
// //    The prefix match index will be the size of the string.
// // 5. Partial Match:
// //    Both strings are matched by a non-empty prefix and
// //    the prefix match index will be smaller than both string sizes.
// //
// // Thereby, we assume that the empty string is used for the root node
// // and that this one matches every string.
// // In all other cases, a prefix match index of zero means no match at all.

// // No Match
// // In this case, the insertion should not change the given node at all.
// //
// template <instance::node root, static_zstring str>
//   requires(!root::string.empty())  //
// struct basic_insertion_implementation<root, str, 0> {
//   using type = root;
// };

// // Full Match
// // In this case, the path and node already exist inside the tree.
// // The node may not be marked as a leaf.
// // So, we have to make it a leaf and leave the rest as it is.
// // The full match is therefore a projection and
// // makes sure that a given string can only be inserted once.
// //
// template <instance::node root, static_zstring str, size_t index>
//   requires(index == root::string.size()) && (index == str.size())  //
// struct basic_insertion_implementation<root, str, index> {
//   using type = leaf<root::string, typename root::children>;
// };

// // String Match
// // In this case, the current node needs to be split.
// // The new node will be a leaf with the prefix as its string.
// // The old node with its children will be the child of the new node.
// // Hereby, the string of the old node will be changed to the tail.
// //
// template <instance::node root, static_zstring str, size_t index>
//   requires(index < root::string.size()) && (index == str.size())  //
// struct basic_insertion_implementation<root, str, index> {
//   using split =
//       node<tail<index>(root::string), typename root::children, root::is_leaf>;
//   using type = leaf<str, node_list<split>>;
// };

// // Partial Match
// // In this case, the current node needs to be split.
// // It will provide two children.
// // One with its former children and the tail of the node string.
// // The other node will provide no children and the tail of the string.
// // The new node itself is no leaf.
// //
// // At this point, the order of the insertion is important,
// // if lexicographical order in the radix tree is of interest.
// //
// template <instance::node root, static_zstring str, size_t index>
//   requires(index < root::string.size()) && (index < str.size())  //
// struct basic_insertion_implementation<root, str, index> {
//   using first =
//       node<tail<index>(root::string), typename root::children, root::is_leaf>;
//   using second = leaf<tail<index>(str)>;
//   // If order would not be of importance, this line could be used.
//   // using type = node<prefix<index>(str), node_list<first, second>>;
//   // using children = insert_when<node_list<first>, second, node_order>;
//   // using children =
//   //     decltype(insert<second>(node_list<first>{}, []<typename x, typename y> {
//   //       return node_order<x, y>::value;
//   //     }));
//   using children = decltype(insert<second>(node_list<first>{}, node_order));
//   using type     = node<prefix<index>(str), children>;
// };

// // Node Match
// // This case is a little bit more complex.
// // Because the node was fully matched,
// // the given string tail needs to be forwarded to its children.
// // If no children matches a prefix with the tail,
// // a new node has to be inserted into the children of the current node.
// // In the other case, we can finally recursively call the basic insertion.
// //
// template <static_zstring str, instance::node_list nodes>
// struct match_existence;
// //
// template <instance::node root, static_zstring str, size_t index, bool match>
// struct node_match_implementation;
// //
// template <instance::node root, static_zstring str, size_t index>
//   requires(index == root::string.size()) && (index < str.size())  //
// struct basic_insertion_implementation<root, str, index> {
//   static constexpr bool matched =
//       match_existence<tail<index>(str), typename root::children>::value;
//   using type =
//       typename node_match_implementation<root, str, index, matched>::type;
// };
// //
// // Checking if any of the node children matches a prefix of a given string
// // can be simply done by checking the first character
// // in addition with some syntax for variadic templates.
// //
// template <static_zstring str, typename... types>
// struct match_existence<str, node_list<types...>> {
//   static constexpr bool value = ((str[0] == types::string[0]) || ...);
// };
// //
// // If no children matches any prefix,
// // the given string will be inserted as a new leaf with no children
// // into the current children list of the given node.
// //
// // At this point, the order of the insertion is important,
// // if lexicographical order in the radix tree is of interest.
// //
// template <instance::node root,
//           static_zstring str,
//           size_t index,
//           bool match = false>
// struct node_match_implementation {
//   using new_node = leaf<tail<index>(str)>;
//   // If order would not be of importance, this line could be used.
//   // using new_children = push_back<typename root::children, new_node>;
//   // using new_children =
//   //     insert_when<typename root::children, new_node, node_order>;
//   // using new_children = decltype(insert<new_node>(
//   //     typename root::children{},
//   //     []<typename x, typename y> { return node_order<x, y>::value; }));
//   using new_children =
//       decltype(insert<new_node>(typename root::children{}, node_order));

//   using type = node<root::string, new_children, root::is_leaf>;
// };
// //
// // When there is a child of the current node
// // which matches a prefix of the given string
// // then by construction of the radix tree
// // this node has to be unique.
// // In such a case, we syntactically insert the given string into all children
// // by recursively calling the basic insertion function
// // together with the type list transformation algorithm,
// // knowing that all children that do not match any prefix
// // will not be changed by this transformation.
// //
// template <instance::node root, static_zstring str, size_t index>
// struct node_match_implementation<root, str, index, true> {
//   template <instance::node _>
//   using inserter = basic_insertion<_, tail<index>(str)>;
//   // using new_children = transformation<typename root::children, inserter>;
//   using new_children =
//       decltype(transform(typename root::children{}, []<typename x> {
//         return node_list<typename inserter<x>::type>{};
//       }));
//   using type = node<root::string, new_children, root::is_leaf>;
// };

// }  // namespace detail

}  // namespace ensketch::xstd::detail::static_radix_tree

#include <ensketch/xstd/detail/static_radix_tree/node.ipp>
