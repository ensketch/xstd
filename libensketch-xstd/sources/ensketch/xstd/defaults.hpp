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

// Include Version Testing Header
//
#ifndef __has_include
#define __has_include(x) 0
#endif
#if __has_include(<version>)
#include <version>
#endif

// Headers for Error Handling
//
#include <cassert>
#include <stdexcept>
#include <system_error>

// Standard Type Definitions
//
#include <cstddef>
#include <cstdint>

// Standard Utilities
//
#include <utility>

namespace ensketch::xstd {

using namespace std::literals;

// Standard Integer Type Aliases
//
using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
//
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

// Standard Floating-Point Type Aliases
//
using float32 = float;
using float64 = double;

}  // namespace ensketch::xstd
