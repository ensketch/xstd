#include <ensketch/xstd/meta/name_list.hpp>

using ensketch::xstd::meta::name_list;
using ensketch::xstd::meta::name_list_instance;
using ensketch::xstd::meta::string_list;

static_assert(name_list_instance<name_list<"help", "version">>);
static_assert(!name_list_instance<string_list<"help", "help">>);
