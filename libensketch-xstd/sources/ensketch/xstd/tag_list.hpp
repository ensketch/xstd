#pragma once
#include <ensketch/xstd/type_list.hpp>

namespace ensketch::xstd {

template <generic::tag... types>
using tag_list = type_list<types...>;

}  // namespace ensketch::xstd
