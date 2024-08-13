#include <ensketch/xstd/meta/static_identifier_list.hpp>

using ensketch::xstd::meta::static_identifier_list;
using ensketch::xstd::meta::static_zstring_list;
namespace instance = ensketch::xstd::meta::instance;

static_assert(instance::static_identifier_list<
              static_identifier_list<"help", "version">>);
static_assert(
    !instance::static_identifier_list<static_zstring_list<"help", "help">>);
