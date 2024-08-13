#pragma once
#include <ensketch/xstd/meta/type_list.hpp>

namespace ensketch::xstd::meta {

template <generic::tag... types>
using tag_list = type_list<types...>;

}  // namespace ensketch::xstd::meta
