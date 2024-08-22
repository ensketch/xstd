#include <ensketch/xstd/meta/string_list.hpp>

using ensketch::xstd::meta::string_list;
using ensketch::xstd::meta::string_list_instance;

static_assert(string_list_instance<string_list<>>);
static_assert(string_list_instance<string_list<"help">>);
static_assert(string_list_instance<string_list<"hello">>);
static_assert(string_list_instance<string_list<"version">>);

int main() {}
