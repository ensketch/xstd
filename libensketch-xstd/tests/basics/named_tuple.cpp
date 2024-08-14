#include <ensketch/xstd/named_tuple.hpp>

using namespace ensketch::xstd;

using tuple_type =
    named_tuple<meta::name_list<"id", "value">, std::tuple<int, float>>;

static_assert(generic_named_tuple<tuple_type>);
