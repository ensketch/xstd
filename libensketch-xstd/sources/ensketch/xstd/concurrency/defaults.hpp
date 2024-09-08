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

/// Checks whether the given type can be used as a task object,
/// i.e., is movable and invocable with no arguments.
///
template <typename type>
concept nullary_task = std::movable<type> && std::invocable<type>;

/// Checks whether the given type is a nullary task and its return type
/// from its invocation with no arguments coincides with the given return type.
///
template <typename type, typename result>
concept nullary_task_for =
    nullary_task<type> && std::same_as<result, std::invoke_result_t<type>>;

}  // namespace ensketch::xstd
