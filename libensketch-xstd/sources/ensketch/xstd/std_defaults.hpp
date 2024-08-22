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
#include <ensketch/xstd/std.hpp>

namespace ensketch::xstd {

// Add shorter integer type definitions.
// The suffix '_t' introduces redundancy.
//
using int8  = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
//
using uint8  = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

// According to the C++ Core Guidelines,
// the usage of C-style strings should be clear
// by using appropriate type definitions.
// In the future, this might be handled by
// using the Guideline Support Library (GSL).
//
using zstring  = char*;
using czstring = const char*;

using std::string;

// Add consistent floating-point type definitions.
// 'float' and 'double' do not seem to be that consistent.
//
using float32 = float;
using float64 = double;

// Standard type for floating-point computations
//
using real = float32;

// Provide quick standard math constants for standard floating-point type.
//
inline constexpr auto pi       = std::numbers::pi_v<real>;
inline constexpr auto infinity = std::numeric_limits<real>::infinity();

}  // namespace ensketch::xstd
