#include <print>
//
#include <ensketch/xstd/meta/string.hpp>

using namespace std;
using namespace ensketch::xstd::meta;

static_assert(substring<2, 4>("--help"_xs) == "help"_xs);
static_assert(prefix<2>("--help"_xs) == "--"_xs);
static_assert(tail<2>("--help"_xs) == "help"_xs);
static_assert(suffix<3>("my_file.txt"_xs) == "txt"_xs);
static_assert(prefix_match_index("version"_xs, "verbose"_xs) == 3);
static_assert(prefix_match_index("version"_xs, ""_xs) == 0);
static_assert(prefix_match_index("help"_xs, "help"_xs) == 4);

// static_assert(string_instance<"help"_xs>);
// static_assert(!string_instance<1>);

// static_assert(is_string("help"_xs));
// static_assert(!is_string("help"));
// static_assert(!is_string(1.0f));
// static_assert(!is_string('a'));

void check() {
  for (auto c : "abc"_xs) print("c = {}", c);
  const auto v = view_from("xyz"_xs);
}
