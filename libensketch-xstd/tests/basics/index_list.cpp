#include <ensketch/xstd/meta/index_list.hpp>

using ensketch::xstd::meta::index_list;
using ensketch::xstd::meta::index_list_from_iota;

static_assert(index_list_from_iota<0>() == index_list<>{});
static_assert(index_list_from_iota<1>() == index_list<0>{});
static_assert(index_list_from_iota<2>() == index_list<0, 1>{});
static_assert(index_list_from_iota<3>() == index_list<0, 1, 2>{});
static_assert(index_list_from_iota<4>() == index_list<0, 1, 2, 3>{});
//
static_assert(index_list_from_iota<0, 2>() == index_list<>{});
static_assert(index_list_from_iota<1, 2>() == index_list<2>{});
static_assert(index_list_from_iota<2, 2>() == index_list<2, 3>{});
static_assert(index_list_from_iota<3, 2>() == index_list<2, 3, 4>{});
static_assert(index_list_from_iota<4, 2>() == index_list<2, 3, 4, 5>{});

static_assert(bounded<0>(index_list<>{}));
static_assert(bounded<1>(index_list<>{}));
static_assert(bounded<2>(index_list<>{}));
//
static_assert(!bounded<0>(index_list<0>{}));
static_assert(bounded<1>(index_list<0>{}));
static_assert(bounded<2>(index_list<0>{}));
//
static_assert(!bounded<0>(index_list<1>{}));
static_assert(!bounded<1>(index_list<1>{}));
static_assert(bounded<2>(index_list<1>{}));
//
static_assert(bounded<4>(index_list<1, 2, 3>{}));
