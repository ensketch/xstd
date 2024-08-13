#include <ensketch/xstd/meta/static_index_list.hpp>

using ensketch::xstd::meta::static_index_list;
using ensketch::xstd::meta::static_index_list_from_iota;

static_assert(static_index_list_from_iota<0>() == static_index_list<>{});
static_assert(static_index_list_from_iota<1>() == static_index_list<0>{});
static_assert(static_index_list_from_iota<2>() == static_index_list<0, 1>{});
static_assert(static_index_list_from_iota<3>() == static_index_list<0, 1, 2>{});
static_assert(static_index_list_from_iota<4>() ==
              static_index_list<0, 1, 2, 3>{});
//
static_assert(static_index_list_from_iota<0, 2>() == static_index_list<>{});
static_assert(static_index_list_from_iota<1, 2>() == static_index_list<2>{});
static_assert(static_index_list_from_iota<2, 2>() == static_index_list<2, 3>{});
static_assert(static_index_list_from_iota<3, 2>() ==
              static_index_list<2, 3, 4>{});
static_assert(static_index_list_from_iota<4, 2>() ==
              static_index_list<2, 3, 4, 5>{});

static_assert(bounded<0>(static_index_list<>{}));
static_assert(bounded<1>(static_index_list<>{}));
static_assert(bounded<2>(static_index_list<>{}));
//
static_assert(!bounded<0>(static_index_list<0>{}));
static_assert(bounded<1>(static_index_list<0>{}));
static_assert(bounded<2>(static_index_list<0>{}));
//
static_assert(!bounded<0>(static_index_list<1>{}));
static_assert(!bounded<1>(static_index_list<1>{}));
static_assert(bounded<2>(static_index_list<1>{}));
//
static_assert(bounded<4>(static_index_list<1, 2, 3>{}));
