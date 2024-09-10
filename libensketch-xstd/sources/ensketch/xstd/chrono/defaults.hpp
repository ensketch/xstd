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
#include <ensketch/xstd/defaults.hpp>
//
#include <chrono>
#include <ctime>

namespace ensketch::xstd {

namespace chrono = std::chrono;

using clock = chrono::high_resolution_clock;

template <typename clock, typename duration>
inline auto elapsed_seconds(chrono::time_point<clock, duration> start,
                            chrono::time_point<clock, duration> end) noexcept {
  return chrono::duration<float32>(end - start).count();
}

}  // namespace ensketch::xstd
