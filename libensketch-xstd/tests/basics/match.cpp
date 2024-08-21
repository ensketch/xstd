#include <cstdint>
#include <print>
#include <thread>
//
#include <ensketch/xstd/match.hpp>
#include <ensketch/xstd/meta/type_list.hpp>

using namespace ensketch::xstd::meta;
using ensketch::xstd::match;
using ensketch::xstd::matches;

/// Provide simple `test_list` template and `test_list_match` concept.
/// `test_list` is a variadic template and `test_list_match`
/// returns `true` for any given `test_list` instance.
///
template <typename... types>
struct test_list {};
//
template <typename type>
concept test_list_match =
    matches<type, []<typename... types>(test_list<types...>) {
      return as_signature<true>;
    }>;

/// Test valid and invalid matches for `test_list_match`.
///
static_assert(test_list_match<test_list<>>);
static_assert(test_list_match<test_list<int>>);
static_assert(test_list_match<test_list<int, float>>);
static_assert(test_list_match<test_list<char, int, float>>);
//
static_assert(test_list_match<test_list<>&>);
static_assert(test_list_match<const test_list<>&>);
static_assert(test_list_match<test_list<>&&>);
static_assert(test_list_match<const test_list<>&&>);
//
static_assert(test_list_match<test_list<int>&>);
static_assert(test_list_match<const test_list<int>&>);
static_assert(test_list_match<test_list<int>&&>);
static_assert(test_list_match<const test_list<int>&&>);
//
static_assert(test_list_match<test_list<int, float>&>);
static_assert(test_list_match<const test_list<int, float>&>);
static_assert(test_list_match<test_list<int, float>&&>);
static_assert(test_list_match<const test_list<int, float>&&>);
//
static_assert(!test_list_match<type_list<>>);
static_assert(!test_list_match<type_list<int>>);
static_assert(!test_list_match<type_list<int, float>>);
static_assert(!test_list_match<type_list<char, int, float>>);
//
static_assert(!test_list_match<std::string>);
static_assert(!test_list_match<std::thread>);
//
static_assert(!test_list_match<int>);
static_assert(!test_list_match<float>);
static_assert(!test_list_match<char>);

// Prepared Matcher
//
constexpr auto test_list_matcher = match{
    // Allow every instance of `test_list`.
    []<typename... types>(test_list<types...>) { return as_signature<true>; },
    // Remove empty instances of `test_list`.
    [](test_list<>) { return as_signature<false>; },
    // Also, remove `test_list` instances with one type.
    []<typename x>(test_list<x>) { return as_signature<false>; },
};

// Another Prepared Matcher
//
constexpr auto std_type_matcher = match{
    // Allow lvalue references of `std::string`.
    [](std::string&) { return as_signature<true>; },
    // Allow rvalue references of `std::thread`.
    [](std::thread&&) { return as_signature<true>; },
};

/// Use a very complicated test matcher.
///
template <typename type>
concept test_match =
    matches<type,
            // Prepared Matcher
            //
            test_list_matcher,

            // In-Place Lambda Expressions
            //
            // Allow all instances of `type_list` with concepts.
            [](type_list_instance auto list) { return as_signature<true>; },
            // But remove `type_list` instances with two elements.
            []<type_list_instance list>(list)
              requires(size(list{}) == 2)
            { return as_signature<false>; },

            // Another Prepared Matcher
            //
            std_type_matcher,

            // In-Place Matcher
            match{
                // Allow const-lvalue references of `float`.
                [](const float&) { return as_signature<true>; },
                // Allow values of type `int`.
                [](int) { return as_signature<true>; },
            }>;

/// Test valid and invalid matches for `test_match`.
/// Remember that functions will be decayed first.
///
static_assert(!test_match<test_list<>>);
static_assert(!test_match<test_list<int>>);
static_assert(!test_match<test_list<float>>);
static_assert(!test_match<test_list<char>>);
static_assert(test_match<test_list<int, float>>);
static_assert(test_match<test_list<char, int, float>>);
//
static_assert(test_match<type_list<>>);
static_assert(test_match<type_list<int>>);
static_assert(!test_match<type_list<int, float>>);
static_assert(test_match<type_list<char, int, float>>);
//
static_assert(!test_match<std::string>);
static_assert(test_match<std::string&>);
static_assert(!test_match<std::string&&>);
static_assert(!test_match<const std::string&>);
static_assert(!test_match<const std::string&&>);
//
static_assert(test_match<std::thread>);
static_assert(!test_match<std::thread&>);
static_assert(test_match<std::thread&&>);
static_assert(!test_match<const std::thread&>);
static_assert(!test_match<const std::thread&&>);
//
static_assert(test_match<int>);
static_assert(test_match<int&>);
static_assert(test_match<int&&>);
static_assert(test_match<const int&>);
static_assert(test_match<const int&&>);
//
static_assert(test_match<float>);
static_assert(test_match<float&>);
static_assert(test_match<float&&>);
static_assert(test_match<const float&>);
static_assert(test_match<const float&&>);
//
static_assert(!test_match<char>);
static_assert(!test_match<char&>);
static_assert(!test_match<char&&>);
static_assert(!test_match<const char&>);
static_assert(!test_match<const char&&>);

// enum class error { negative };
// using result = std::variant<float, error>;
// auto calc(float x) -> result {
//   if (x < 0.0f) return error::negative;
//   return x;
// }

// int main() {
// calc(1.0f) | match{[](error e) {
//                      std::print("ERROR: The given number was negative.\n");
//                    },
//                    [](float x) {
//                      std::print("The given number is {} and valid.\n", x);
//                    }};

// calc(-1.0f) |
//     match{
//         [](error e) {
//           std::print("ERROR: The given number was negative.\n");
//         },
//         [](float x) { std::print("The given number is {} and valid.\n", x); },
//     };
// }
