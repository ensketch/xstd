#include <ensketch/xstd/static_index_list.hpp>

using ensketch::xstd::static_index_list;

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
