#include <ensketch/xstd/static_zstring_list.hpp>

using ensketch::xstd::static_zstring_list;
namespace instance = ensketch::xstd::instance;

static_assert(instance::static_zstring_list<static_zstring_list<>>);
static_assert(instance::static_zstring_list<static_zstring_list<"help">>);
static_assert(instance::static_zstring_list<static_zstring_list<"hello">>);
static_assert(instance::static_zstring_list<static_zstring_list<"version">>);
