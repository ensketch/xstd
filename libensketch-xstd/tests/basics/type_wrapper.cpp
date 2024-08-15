#include <ensketch/xstd/meta/type_wrapper.hpp>

namespace meta = ensketch::xstd::meta;

static_assert(meta::generic_type_wrapper<meta::type_wrapper<int>>);

static_assert(meta::value<int> == meta::value<int>);
static_assert(meta::equal<int, meta::unwrap<meta::value<int>>>);

static_assert(meta::value<int> != meta::value<int&>);
static_assert(!meta::equal<int, meta::unwrap<meta::value<int&>>>);
