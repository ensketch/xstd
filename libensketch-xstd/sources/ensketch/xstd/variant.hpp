#pragma once
#include <ensketch/xstd/match.hpp>

namespace ensketch::xstd {

///
///
constexpr auto operator|(auto&& variant, match_instance auto&& matcher) {
  using result_type =
      decltype(std::visit(std::forward<decltype(matcher)>(matcher),
                          std::forward<decltype(variant)>(variant)));
  if constexpr (meta::not_void<result_type>)
    return std::visit(std::forward<decltype(matcher)>(matcher),
                      std::forward<decltype(variant)>(variant));
  else
    std::visit(std::forward<decltype(matcher)>(matcher),
               std::forward<decltype(variant)>(variant));
}

}  // namespace ensketch::xstd
