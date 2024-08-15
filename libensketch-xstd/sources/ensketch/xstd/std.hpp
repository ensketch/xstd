#pragma once
// This header file is the base header
// that all other headers of this library include.
// It provides basic STL header inclusions, concepts, type
// definitions, and functions that are needed almost everywhere.

// We always want to be able to use 'assert'.
//
#include <cassert>

// Add the standard type definitions for
// bytes, integers, and pointer types.
//
#include <cstddef>
#include <cstdint>

// Math Utilities
//
#include <bit>
#include <cmath>
#include <limits>
#include <numbers>
#include <numeric>

// We will always make use of templates
// Include all standard headers that deal
// with template meta programming facilities.
//
#include <compare>
#include <concepts>
#include <functional>
#include <type_traits>
#include <typeinfo>

// We will make use of exceptions to handle errors.
// For that, include the standard exceptions.
//
#include <stdexcept>

// Utilities to use, manipulate, and format
// strings are always needed.
//
#include <format>
#include <iomanip>
#include <iostream>
#include <print>
#include <sstream>
#include <string>
#include <string_view>

// Standard Containers and Ranges
//
#include <algorithm>
#include <array>
#include <ranges>
#include <vector>

// Almost all executables and libraries will provide interactions
// with the filesystem and reading from or writing to files.
//
#include <filesystem>
#include <fstream>
