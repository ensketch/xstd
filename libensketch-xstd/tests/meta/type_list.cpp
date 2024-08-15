#include <print>
//
#include <ensketch/xstd/meta/type_list.hpp>

namespace meta = ensketch::xstd::meta;
using ensketch::xstd::float32;

template <typename type>
constexpr auto string_from() {
  return typeid(type).name();
}
template <>
constexpr auto string_from<int>() {
  return "int";
}
template <>
constexpr auto string_from<float32>() {
  return "float32";
}
template <>
constexpr auto string_from<std::string>() {
  return "std::string";
}

int main() {
  constexpr auto types = meta::type_list<int, float, std::string>{};

  int i = 0;
  for_each(types, [&]<typename type> {
    std::print("type {} = {}\n", i++, string_from<type>());
  });

  std::string str{};
  for_each(types, [&str]<typename type> {
    str += string_from<type>();
    str += std::format(", ");
  });
  std::print("types = {}\n", str);
}
