#pragma once
#include <ensketch/xstd/utility.hpp>

namespace ensketch::xstd::meta {

// Static zero-terminated strings can be used as template arguments.
// C++20 support has to be enabled.

// We will not use a variant such as
// 'basic_static_zstring' to support 'wchar_t'.
// This would make things complicated and
// difficult to read without much benefit.

// The length has to be known.
//
template <size_t N>
struct string {
  static_assert(N > 0, "String must at least store a zero byte.");

  using iterator       = zstring;
  using const_iterator = czstring;

  constexpr string() noexcept = default;

  // Enable implicit construction of static strings from C strings.
  //
  constexpr string(const char (&str)[N]) noexcept {
    for (size_t i = 0; i < N; ++i) data_[i] = str[i];
  }

  // Enable implicit conversion to C strings.
  //
  constexpr operator zstring() noexcept { return data_; }
  constexpr operator czstring() const noexcept { return data_; }

  /// Get index-based access to the characters.
  ///
  constexpr auto operator[](size_t index) noexcept -> char& {
    return data_[index];
  }
  constexpr auto operator[](size_t index) const noexcept -> char {
    return data_[index];
  }

  // The compiler needs to be able to compare the content of strings.
  //
  friend constexpr auto operator<=>(const string&,
                                    const string&) noexcept = default;

  /// Returns the size of the string without the terminating null character.
  ///
  static constexpr auto size() noexcept { return N - 1; }

  /// Checks whether the string is empty.
  ///
  static constexpr bool empty() noexcept { return size() == 0; }

  /// Get access to the raw underlying data.
  ///
  constexpr auto data() noexcept -> zstring { return data_; }
  constexpr auto data() const noexcept -> czstring { return data_; }

  /// Get acces to the underlying data by using iterators.
  ///
  constexpr auto begin() noexcept -> iterator { return &data_[0]; }
  constexpr auto begin() const noexcept -> const_iterator { return &data_[0]; }
  constexpr auto end() noexcept -> iterator { return &data_[N]; }
  constexpr auto end() const noexcept -> const_iterator { return &data_[N]; }

  char data_[N]{};
};

// These overloads decide whether a given value
// is an instance of string.
// Especially, this is useful for template meta programming with concepts.
//
constexpr bool is_string(auto _) noexcept {
  return false;
}
template <size_t N>
constexpr bool is_string(string<N> _) noexcept {
  return true;
}

template <auto value>
concept string_instance = is_string(value);

// Provide support for custom literal '_sz'.
//
template <string str>
constexpr auto operator""_sz() noexcept {
  return str;
}

// Append characters to static string should be compile-time enabled.
//
template <size_t N>
constexpr auto operator+(const string<N>& str, char c) noexcept {
  string<N + 1> result{};
  for (size_t i = 0; i < N - 1; ++i) result[i] = str[i];
  result[N - 1] = c;
  return result;
}

// Appending two static strings should also be compile-time enabled.
//
template <size_t N, size_t M>
constexpr auto operator+(const string<N>& str1,
                         const string<M>& str2) noexcept {
  string<N + M - 1> result{};
  size_t i = 0;
  for (; i < N - 1; ++i) result[i] = str1[i];
  for (; i < N + M - 1; ++i) result[i] = str2[i - N + 1];
  return result;
}

/// Returns a static string containing the substring
/// of the given string specified by offset and size.
///
template <size_t offset, size_t size, size_t N>
constexpr auto substring(string<N> str) noexcept  //
  requires(offset + size < N)
{
  string<size + 1> result{};
  for (size_t i = 0; i < size; ++i) result[i] = str[offset + i];
  return result;
}

/// Returns a static string containing the prefix
/// of the given string with given size.
///
template <size_t size, size_t N>
constexpr auto prefix(string<N> str) noexcept  //
  requires(size < N)
{
  return substring<0, size>(str);
}

/// Returns a static string containing the suffix
/// of the given string with given size.
///
template <size_t size, size_t N>
constexpr auto suffix(string<N> str) noexcept  //
  requires(size < N)
{
  return substring<N - 1 - size, size>(str);
}

/// Returns the first index at which the characters
/// of the two given strings are not equal.
///
template <size_t N, size_t M>
constexpr auto prefix_match_index(string<N> str1,
                                  string<M> str2) noexcept -> size_t {
  constexpr auto bound = ((N < M) ? N : M) - 1;
  for (size_t i = 0; i < bound; ++i)
    if (str1[i] != str2[i]) return i;
  return bound;
}

template <size_t index, size_t N>
constexpr auto tail(string<N> str) noexcept {
  return suffix<str.size() - index>(str);
}

}  // namespace ensketch::xstd::meta
