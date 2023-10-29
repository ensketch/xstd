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
// For that, include strings and the standard exceptions.
//
#include <stdexcept>
#include <string>

namespace ensketch::xstd {

// Add consistent floating-point type definitions.
// 'float' and 'double' do not seem to be that consistent.
//
using float32 = float;
using float64 = double;

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

// The 'meta' namespace is meant for so-called meta functions.
// Meta functions are functions that work on types
// and do not offer any overloading facilities.
// Hence, we put them in a dedicated namespace.
//
namespace meta {

/// When either of these two functions is employed,
/// it will result in a compiler error,
/// while simultaneously providing the beneficial outcome
/// of displaying their instantiated types.
/// Although not entirely precise,
/// this approach can offer valuable insights into compile-time issues.
/// Therefore, they are primarily useful during the development and debugging phases.
///
template <typename type>
constexpr auto print_type() = delete;
//
template <typename type>
constexpr auto print_type(type) = delete;

/// This meta function is by definition the same as 'std::decay_t'
/// and returns the decayed type.
///
template <typename type>
using reduction = std::decay_t<type>;

/// This meta function is by definition the same as 'std::same_as'
/// and checks whether two given types are equal.
///
template <typename x, typename y>
constexpr bool equal = std::same_as<x, y>;

/// This function checks whether two provided values are
/// strictly equal, meaning their type and values coincide.
///
constexpr bool strict_equal(auto x, auto y) noexcept {
  return false;
}
//
template <typename type>
constexpr bool strict_equal(type x, type y) noexcept {
  return x == y;
}

}  // namespace meta

// The syntax for concepts in modern C++ introduces some problems
// when trying to consistently name concepts and structures
// while still being able to distinguish them.
// Here, all concepts will be put into their dedicated namespace
// 'generic' to be able to give types and concepts the same name.
//
namespace generic {

// Defining new concepts will require to call meta functions.
// It makes sense to use this namespace by default.
//
using namespace meta;

/// This concepts checks whether two given types are equal.
///
template <typename x, typename y>
concept identical = equal<x, y>;

/// This concepts checks whether 'x' is reducible to 'y'.
/// That is, by decaying/reducing 'x' we get 'y'.
///
template <typename x, typename y>
concept reducible = identical<reduction<x>, y>;

/// This concepts checks whether a given type is irreducible.
/// That is, applying a decay/reduction to that type does not change it.
///
template <typename x>
concept irreducible = reducible<x, x>;

/// This concepts checks whether a given type is a tag type, i.e. an empty type.
///
template <typename type>
concept tag = std::is_empty_v<type>;

}  // namespace generic

// This namespace is used for concepts, that check whether a
// given type is a specialization of a specific type template.
// As types and type templates may use the same name as similar concepts,
// this namespace is mandatory to distinguish those different entities.
//
namespace instance {

// As we will work with meta functions,
// make the namespace available by default.
//
using namespace meta;

}  // namespace instance

}  // namespace ensketch::xstd
