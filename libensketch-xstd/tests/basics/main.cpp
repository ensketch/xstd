#include <iomanip>
#include <iostream>
//
#include <ensketch/xstd/static_radix_tree.hpp>

using namespace std;
using namespace ensketch::xstd;

template <static_radix_tree::instance::node root, static_zstring prefix>
constexpr void print() {
  using namespace std;
  constexpr auto str = prefix + '|' + root::string;
  for_each(typename root::children{},
           [&]<typename child> { print<child, str>(); });
  if constexpr (root::is_leaf) cout << '"' << str << '"' << endl;
}

template <typename root>
constexpr void print() {
  using namespace std;
  for_each(typename root::children{},
           [&]<typename child> { print<child, root::string>(); });
  if constexpr (root::is_leaf) cout << '"' << root::string << '"' << endl;
}

namespace srt = static_radix_tree;

using srt::leaf;
using srt::node;

static_assert(srt::instance::node_list<type_list<>>);
static_assert(!srt::instance::node_list<type_list<float>>);
static_assert(srt::instance::node_list<type_list<node<"test">>>);
static_assert(srt::instance::node_list<type_list<leaf<"">>>);
static_assert(!srt::instance::node_list<type_list<node<"test">, float>>);

// using tree = srt<"help", "version", "test", "hell">;
using tree = srt::construction<  //
    "help",
    "version",
    "helo",
    "hel",
    "verbose",
    "help-me",
    "abc",
    "key",
    "check",
    "make",
    "input",
    "output",
    "man",
    "cheat",
    "in",
    "out",
    "help",
    "help">;
static_assert(srt::instance::node<tree>);

inline void visit(czstring cstr) {
  const auto visited = srt::visit<tree>(cstr, []<static_zstring str> {
    cout << '"' << str << '"' << " has been visited!" << endl;
  });
  if (!visited)
    cout << '"' << cstr << '"' << " is not inside the static radix tree."
         << endl;
}

inline void traverse(czstring cstr) {
  const auto traversed =
      srt::traverse<tree>(cstr, [&]<static_zstring str>(czstring tail) {
        cout << '"' << cstr << '"' << " visited the prefix \"" << str
             << "\" with the tail \"" << tail << "\"!" << endl;
      });
  if (!traversed)
    cout << '"' << cstr << '"'
         << " has no known prefix inside the static radix tree." << endl;
}

int main() {
  print<tree>();

  visit("check");
  visit("help");
  visit("mine");
  visit("long");
  visit("verbose");

  traverse("check");
  traverse("help");
  traverse("mine");
  traverse("long");
  traverse("verbose");
  traverse("hela");
  traverse("key=uiae");
  traverse("xyz");
}
