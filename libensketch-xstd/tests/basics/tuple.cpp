#include <array>
//
#include <ensketch/xstd/tuple.hpp>

using namespace ensketch::xstd;

static_assert(generic::tuple<std::tuple<>>);
static_assert(generic::tuple<std::tuple<int>>);
static_assert(generic::tuple<std::tuple<int, float>>);
//
static_assert(generic::tuple<std::array<int, 0>>);
static_assert(generic::tuple<std::array<int, 5>>);
static_assert(generic::tuple<std::array<float, 0>>);
static_assert(generic::tuple<std::array<float, 5>>);

static_assert(type_list_from<std::tuple<>>() == meta::type_list<>{});
static_assert(type_list_from<std::tuple<int>>() == meta::type_list<int>{});
static_assert(type_list_from<std::tuple<int&>>() == meta::type_list<int&>{});
static_assert(type_list_from<std::tuple<int&, float>>() ==
              meta::type_list<int&, float>{});
//
static_assert(type_list_from<std::array<int, 0>>() == meta::type_list<>{});
static_assert(type_list_from<std::array<int, 5>>() ==
              meta::type_list<int, int, int, int, int>{});
