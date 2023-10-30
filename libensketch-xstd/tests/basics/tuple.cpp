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

static_assert(type_list_from<std::tuple<>>() == type_list<>{});
static_assert(type_list_from<std::tuple<int>>() == type_list<int>{});
static_assert(type_list_from<std::tuple<int&>>() == type_list<int&>{});
static_assert(type_list_from<std::tuple<int&, float>>() ==
              type_list<int&, float>{});
//
static_assert(type_list_from<std::array<int, 0>>() == type_list<>{});
static_assert(type_list_from<std::array<int, 5>>() ==
              type_list<int, int, int, int, int>{});
