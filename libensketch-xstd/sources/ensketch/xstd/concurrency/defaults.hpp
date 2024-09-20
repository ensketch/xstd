// Copyright © 2024 Markus Pawellek
//
// This file is part of `xstd`.
//
// `xstd` is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// `xstd` is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with `xstd`. If not, see <https://www.gnu.org/licenses/>.
//
#pragma once
#include <ensketch/xstd/meta/defaults.hpp>
//
#include <atomic>
#include <barrier>
#include <condition_variable>
#include <future>
#include <latch>
#include <mutex>
#include <semaphore>
#include <shared_mutex>
#include <thread>

namespace ensketch::xstd {

/// `async_invoke` acts like `std::async` but uses
/// `std::launch::async` as launch policy.
///
/// "[...] having a function that acts like `std::async`,
/// but that automatically uses `std::launch::async` as the
/// launch policy, is a convenient tool to have around, [...]"
///
/// from Effective Modern C++ by Scott Meyers
///
[[nodiscard]] inline auto async_invoke(auto&& f, auto&&... args) {
  return std::async(std::launch::async,  //
                    std::forward<decltype(f)>(f),
                    std::forward<decltype(args)>(args)...);
}

/// Checks whether the given type can be used as a task object,
/// i.e., is movable and invocable with no arguments.
///
template <typename type>
concept nullary_task = std::invocable<type>;

/// Checks whether the given type is a nullary task and its return type
/// from its invocation with no arguments coincides with the given return type.
///
template <typename type, typename result>
concept nullary_task_for =
    nullary_task<type> && std::same_as<result, std::invoke_result_t<type>>;

/// Bind the callable `f` to the arguments `args...` for asynchronous invocation.
/// Intrinsically, the generated function object will call `std::invoke`.
/// This function template is a modern and lightweight alternative to `std::bind`.
/// However, it offers less features and may not work for member functions.
///
[[nodiscard]] constexpr auto task_bind(auto&& f, auto&&... args) {
  return
      [f = auto(std::forward<decltype(f)>(f)),
       ... args = auto(std::forward<decltype(args)>(args))](this auto&& self) {
        return std::invoke(std::forward_like<decltype(self)>(f),
                           std::forward_like<decltype(self)>(args)...);
      };
}

/// Bind the callable `f` to the arguments `args...` for asynchronous invocation.
/// Intrinsically, the generated function object will call `std::invoke_r`
/// to implicitly convert the return value to `result` type.
/// This function template is a modern and lightweight alternative to `std::bind`.
/// However, it offers less features and may not work for member functions.
///
template <typename result>
[[nodiscard]] constexpr auto task_bind(auto&& f, auto&&... args) {
  return [f = auto(std::forward<decltype(f)>(f)),
          ... args =
              auto(std::forward<decltype(args)>(args))](this auto&& self) {
    return std::invoke_r<result>(std::forward_like<decltype(self)>(f),
                                 std::forward_like<decltype(self)>(args)...);
  };
}

}  // namespace ensketch::xstd
