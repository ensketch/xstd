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

// static_assert(transform(type_list<float, void, int>{}, []<typename x> {
//                 if constexpr (equal<x, void>)
//                   return type_list<type_list<>>{};
//                 else
//                   return type_list<x>{};
//               }) == type_list<float, type_list<>, int>{});

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

  // meta::print_type(transform_and_fold(
  //     types, []<typename type> { return meta::value<type>; },
  //     [](auto... args) { return std::tuple<meta::unwrap<args>...>{}; }));

  // meta::print_type<meta::unwrap<transform<std::variant>(types)>>();
  // meta::print_type(transform(types, []<typename type> {
  //   if constexpr (meta::equal<type, int>)
  //     return meta::value<type>;
  //   else
  //     return meta::type_list<type>{};
  // }));
}
