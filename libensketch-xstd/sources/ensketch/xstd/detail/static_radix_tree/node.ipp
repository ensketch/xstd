namespace ensketch::xstd::detail::static_radix_tree {

// After the definition of the node template,
// we are able to implement the 'is_node' predicate.
//
template <typename T>
struct is_node_implementation : std::false_type {};
template <static_zstring str, instance::node_list children, bool is_leaf>
struct is_node_implementation<node<str, children, is_leaf>> : std::true_type {};

// The actual implementation of the insertion routine is done recursively.
// For this, five case have to be taken into account concerning
// the given string and the node string and their prefix match index.
//
// 1. No Match:
//    Both strings do not match and the index is zero.
// 2. Full Match:
//    Both strings are the same and the prefix match index is their length.
// 3. Node Match:
//    The node string is fully matched as a prefix by the string.
//    The prefix match index will be the size of the node string.
// 4. String Match:
//    The string is fully matched as a prefix by the node string.
//    The prefix match index will be the size of the string.
// 5. Partial Match:
//    Both strings are matched by a non-empty prefix and
//    the prefix match index will be smaller than both string sizes.
//
// Thereby, we assume that the empty string is used for the root node
// and that this one matches every string.
// In all other cases, a prefix match index of zero means no match at all.

// We need to provide a total order for nodes based on their strings.
// So, the static radix tree will be unique when new strings are inserted.
// But by definition and construction of the radix tree,
// it suffices to compare the first characters of two given strings.
//
// To establish a total order for nodes based on their strings,
// we compare the first characters of the strings, ensuring uniqueness
// when new strings are inserted into the static radix tree.
//
// template <instance::node p, instance::node q>
// struct node_order {
//   static constexpr bool value = p::string[0] <= q::string[0];
// };
constexpr auto node_order = []<instance::node p, instance::node q> {
  return p::string[0] <= q::string[0];
};

// The prefix match tries to match a static prefix
// given as template parameter in a given dynamic string.
// Using compiler optimization concerning the inlining of constexpr functions,
// the generated code should not use any indirected function calls or loops.
// If no prefix match can be detected the nullptr is returned.
// Otherwise, the tail of the given dynamic string is returned.
//
template <static_zstring prefix, size_t index = 0>
constexpr auto prefix_match(czstring str) noexcept -> czstring {
  if constexpr (index == prefix.size())
    return str;
  else {
    if (prefix[index] != *str++) return nullptr;
    return prefix_match<prefix, index + 1>(str);
  }
}

// No Match
// In this case, the insertion should not change the given node at all.
//
template <static_zstring str, size_t index>
consteval auto basic_insert_implementation(instance::node auto root)
  requires(!prefix(root).empty()) && (index == 0)
{
  return root;
}
//
// Full Match
// In this case, the path and node already exist inside the tree.
// The node may not be marked as a leaf.
// So, we have to make it a leaf and leave the rest as it is.
// The full match is therefore a projection and
// makes sure that a given string can only be inserted once.
//
template <static_zstring str, size_t index>
consteval auto basic_insert_implementation(instance::node auto root)
  requires(index == prefix(root).size()) && (index == str.size())
{
  return leaf<prefix(root), decltype(children(root))>{};
  // return auto_leaf<prefix(root)>(children(root));
}
//
// String Match
// In this case, the current node needs to be split.
// The new node will be a leaf with the prefix as its string.
// The old node with its children will be the child of the new node.
// Hereby, the string of the old node will be changed to the tail.
//
template <static_zstring str, size_t index>
consteval auto basic_insert_implementation(instance::node auto root)
  requires(index < prefix(root).size()) && (index == str.size())
{
  using split =
      node<tail<index>(prefix(root)), decltype(children(root)), is_leaf(root)>;
  return leaf<str, node_list<split>>{};
}
//
// Partial Match
// In this case, the current node needs to be split.
// It will provide two children.
// One with its former children and the tail of the node string.
// The other node will provide no children and the tail of the string.
// The new node itself is no leaf.
//
// At this point, the order of the insertion is important,
// if lexicographical order in the radix tree is of interest.
//
template <static_zstring str, size_t index>
consteval auto basic_insert_implementation(instance::node auto root)
  requires(index > 0) && (index < prefix(root).size()) && (index < str.size())
{
  using first =
      node<tail<index>(prefix(root)), decltype(children(root)), is_leaf(root)>;
  using second   = leaf<tail<index>(str)>;
  using children = decltype(insert<second>(node_list<first>{}, node_order));
  return node<prefix<index>(str), children>{};
}
//
// Checking if any of the node children matches a prefix of a given string
// can be simply done by checking the first character
// in addition with some syntax for variadic templates.
//
template <static_zstring str, instance::node... nodes>
consteval bool match_exists(node_list<nodes...>) {
  return ((str[0] == nodes::string[0]) || ...);
}
//
// If no children matches any prefix,
// the given string will be inserted as a new leaf with no children
// into the current children list of the given node.
//
// At this point, the order of the insertion is important,
// if lexicographical order in the radix tree is of interest.
//
template <static_zstring str, size_t index, bool matched>
  requires(!matched)
consteval auto node_match(instance::node auto root) {
  using new_node     = leaf<tail<index>(str)>;
  using new_children = decltype(insert<new_node>(children(root), node_order));
  return node<prefix(root), new_children, is_leaf(root)>{};
}
//
// When there is a child of the current node
// which matches a prefix of the given string
// then by construction of the radix tree
// this node has to be unique.
// In such a case, we syntactically insert the given string into all children
// by recursively calling the basic insertion function
// together with the type list transformation algorithm,
// knowing that all children that do not match any prefix
// will not be changed by this transformation.
//
template <static_zstring str, size_t index, bool matched>
  requires(matched)
consteval auto node_match(instance::node auto root) {
  using new_children = decltype(transform(children(root), []<typename x> {
    return node_list<decltype(basic_insert<tail<index>(str)>(x{}))>{};
  }));
  return node<prefix(root), new_children, is_leaf(root)>{};
}
//
// Node Match
// This case is a little bit more complex.
// Because the node was fully matched,
// the given string tail needs to be forwarded to its children.
// If no children matches a prefix with the tail,
// a new node has to be inserted into the children of the current node.
// In the other case, we can finally recursively call the basic insertion.
//
template <static_zstring str, size_t index>
consteval auto basic_insert_implementation(instance::node auto root)
  requires(index == prefix(root).size()) && (index < str.size())
{
  constexpr bool matched = match_exists<tail<index>(str)>(children(root));
  // using type =
  //     typename node_match_implementation<root, str, index, matched>::type;
  return node_match<str, index, matched>(root);
}

///
///
template <static_zstring str>
consteval auto basic_insert(instance::node auto root) {
  constexpr auto index = prefix_match_index(prefix(root), str);
  return basic_insert_implementation<str, index>(root);
}

/// Non-member type function to insert an arbitrary amount of static strings
/// into a given static radix tree.
// template <instance::node root, static_zstring... str>
// using insertion = typename detail::insertion<root, str...>::type;

/// Non-member type function to construct and initialize a static radix tree
/// based on an arbitrary amount of static strings.
// template <static_zstring... str>
// using construction = insertion<node<"">, str...>;

///
///
template <static_zstring... str>
  requires(sizeof...(str) == 0)
consteval auto insert(instance::node auto root) {
  return root;
}
//
template <static_zstring str, static_zstring... tail>
consteval auto insert(instance::node auto root) {
  return insert<tail...>(basic_insert<str>(root));
}

/// The visit algorithm tries to match the whole string
/// with a static string contained inside the static radix tree.
/// If the given string is not contained in the tree,
/// the algorithm returns false and does not call the function object.
/// In the other case, the algorithm returns true
/// and calls the function object
/// with the static string provided as template parameter.
///
template <static_zstring prefix = "">
constexpr bool visit(instance::node auto r, czstring str, auto&& f) {
  using root = decltype(r);

  constexpr auto new_prefix = prefix + root::string;
  const auto tail           = prefix_match<root::string>(str);
  if (!tail) return false;
  if constexpr (root::is_leaf) {
    if (!*tail) {
      std::forward<decltype(f)>(f).template operator()<new_prefix>();
      return true;
    }
  } else {
    if (!*tail) return false;
  }
  return for_each_until(typename root::children{}, [&]<instance::node child> {
    return visit<new_prefix>(child{}, tail, f);
  });
}

/// The traverse algorithm tries to match the longest prefix
/// of the given string contained in the given static radix tree.
/// If no prefix can be matched the algorithm returns false
/// and does not call the function object.
/// If a prefix can be matched, the function object is called
/// with the static prefix and the dynamic tail.
///
template <static_zstring prefix = "">
constexpr bool traverse(instance::node auto r, czstring str, auto&& f) {
  using root = decltype(r);

  const auto tail = prefix_match<root::string>(str);
  if (tail) {
    constexpr auto new_prefix = prefix + root::string;
    const auto found =
        for_each_until(typename root::children{}, [&]<instance::node child> {
          return traverse<new_prefix>(child{}, tail, f);
        });
    if constexpr (root::is_leaf) {
      if (!found)
        std::forward<decltype(f)>(f).template operator()<new_prefix>(tail);
      return true;
    } else
      return found;
  } else
    return false;
}

}  // namespace ensketch::xstd::detail::static_radix_tree
