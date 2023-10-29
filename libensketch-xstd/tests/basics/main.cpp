#include <iomanip>
#include <iostream>
//
#include <ensketch/xstd/static_radix_tree.hpp>

using namespace ensketch::xstd;
// using ensketch::xstd::static_radix_tree;
// namespace instance = ensketch::xstd::instance;

// template <detail::static_radix_tree::instance::node root, static_zstring
// prefix> constexpr void print() {
//   using namespace std;
//   constexpr auto str = prefix + '|' + root::string;
//   for_each(typename root::children{},
//            [&]<typename child> { print<child, str>(); });
//   if constexpr (root::is_leaf) cout << '"' << str << '"' << endl;
// }

// template <typename root>
// constexpr void print() {
//   using namespace std;
//   for_each(typename root::children{},
//            [&]<typename child> { print<child, root::string>(); });
//   if constexpr (root::is_leaf) cout << '"' << root::string << '"' << endl;
// }

// namespace srt = detail::static_radix_tree;

// using srt::leaf;
// using srt::node;

// static_assert(srt::instance::node_list<type_list<>>);
// static_assert(!srt::instance::node_list<type_list<float>>);
// static_assert(srt::instance::node_list<type_list<node<"test">>>);
// static_assert(srt::instance::node_list<type_list<leaf<"">>>);
// static_assert(!srt::instance::node_list<type_list<node<"test">, float>>);

// using tree = srt<"help", "version", "test", "hell">;
// using tree = srt::construction<  //
//     "help",
//     "version",
//     "helo",
//     "hel",
//     "verbose",
//     "help-me",
//     "abc",
//     "key",
//     "check",
//     "make",
//     "input",
//     "output",
//     "man",
//     "cheat",
//     "in",
//     "out",
//     "help",
//     "help">;
// static_assert(srt::instance::node<tree>);

// inline void visit(czstring cstr) {
//   const auto visited = srt::visit<tree>(cstr, []<static_zstring str> {
//     cout << '"' << str << '"' << " has been visited!" << endl;
//   });
//   if (!visited)
//     cout << '"' << cstr << '"' << " is not inside the static radix tree."
//          << endl;
// }

// inline void traverse(czstring cstr) {
//   const auto traversed =
//       srt::traverse<tree>(cstr, [&]<static_zstring str>(czstring tail) {
//         cout << '"' << cstr << '"' << " visited the prefix \"" << str
//              << "\" with the tail \"" << tail << "\"!" << endl;
//       });
//   if (!traversed)
//     cout << '"' << cstr << '"'
//          << " has no known prefix inside the static radix tree." << endl;
// }

using namespace std;

template <detail::static_radix_tree::instance::node root, static_zstring prefix>
constexpr void print() {
  constexpr auto str = prefix + '|' + root::string;
  for_each(typename root::children{},
           [&]<typename child> { print<child, str>(); });
  if constexpr (root::is_leaf) cout << '"' << str << '"' << endl;
}

constexpr void print(instance::static_radix_tree auto t) {
  using tree = decltype(t);
  for_each(children(root(tree{})),
           [&]<typename child> { print<child, tree::root::string>(); });
  if constexpr (tree::root::is_leaf)
    cout << '"' << tree::root::string << '"' << endl;
}

inline void print_visit(instance::static_radix_tree auto tree, czstring cstr) {
  const auto visited = visit(tree, cstr, []<static_zstring str> {
    cout << '"' << str << '"' << " has been visited!" << endl;
  });
  if (!visited)
    cout << '"' << cstr << '"' << " is not inside the static radix tree."
         << endl;
}

inline void print_traverse(instance::static_radix_tree auto tree,
                           czstring cstr) {
  const auto traversed =
      traverse(tree, cstr, [&]<static_zstring str>(czstring tail) {
        cout << '"' << cstr << '"' << " visited the prefix \"" << str
             << "\" with the tail \"" << tail << "\"!" << endl;
      });
  if (!traversed)
    cout << '"' << cstr << '"'
         << " has no known prefix inside the static radix tree." << endl;
}

int main() {
  using detail::static_radix_tree::leaf;
  using detail::static_radix_tree::node;
  using detail::static_radix_tree::node_list;

  // meta::print_type(
  //     static_radix_tree_from<"help", "hello", "version", "verbose", "very",
  //                            "in", "input", "out", "output">());

  constexpr auto tree =
      static_radix_tree_from<"help", "version", "helo", "hel", "verbose",
                             "help-me", "abc", "key", "check", "make", "input",
                             "output", "man", "cheat", "in", "out", "help",
                             "help">();

  print(tree);

  print_visit(tree, "check");
  print_visit(tree, "help");
  print_visit(tree, "mine");
  print_visit(tree, "long");
  print_visit(tree, "verbose");

  print_traverse(tree, "check");
  print_traverse(tree, "help");
  print_traverse(tree, "mine");
  print_traverse(tree, "long");
  print_traverse(tree, "verbose");
  print_traverse(tree, "hela");
  print_traverse(tree, "key=uiae");
  print_traverse(tree, "xyz");
}
