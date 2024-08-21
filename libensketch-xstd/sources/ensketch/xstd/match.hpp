#pragma once
#include <ensketch/xstd/utility.hpp>

namespace ensketch::xstd {

///
///
/// Type-Matching Visitor
/// See: https://en.cppreference.com/w/cpp/utility/variant/visit
///
template <typename... functors>
struct match : functors... {
  using functors::operator()...;
};

///
///
/// Must be implemented with `std::invoke_result_t` to use an unevaluated context.
/// Otherwise, more complicated types, such as `std::thread`, would not be usable.
///
template <typename type, auto... functors>
concept matches =
    std::invoke_result_t<decltype(match{
                             [](auto) { return meta::as_signature<false>; },
                             functors...}),
                         type>::value;

/// Check whether the given type is an instance of `match`.
/// This concept will also match qualified alternatives
/// as it will decay a given type before it is matched.
///
template <typename type>
concept match_instance =
    matches<type, []<typename... functors>(match<functors...>) {
      meta::as_signature<true>;
    }>;

}  // namespace ensketch::xstd
