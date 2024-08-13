#pragma once
#include <ensketch/xstd/meta/name_list.hpp>
#include <ensketch/xstd/tuple.hpp>

namespace ensketch::xstd {

namespace generic {

///
///
template <typename tuple_type, static_zstring name>
concept named_tuple_value_access = requires(tuple_type x) {
  {
    value<name>(x)
  } -> std::convertible_to<typename tuple_type::template type<name>>;
};

/// Checks whether a given type fulfills the requirements of a generic tuple.
///
template <typename tuple_type>
concept named_tuple =
    tuple<tuple_type> && for_all(tuple_type::names, []<static_zstring name> {
      return named_tuple_value_access<tuple_type, name>;
    });

///
///
template <typename T>
concept reducible_named_tuple = named_tuple<reduction<T>>;

///
///
template <typename tuple_type>
concept unnamed_tuple = tuple<tuple_type> && (!named_tuple<tuple_type>);

///
///
template <typename tuple_type>
concept reducible_unnamed_tuple = unnamed_tuple<reduction<tuple_type>>;

}  // namespace generic

template <meta::name_list_instance identifiers, generic::tuple T>
  requires(size(identifiers{}) == std::tuple_size<T>::value)  //
struct named_tuple;

namespace detail {
template <typename type>
struct is_named_tuple : std::false_type {};
template <meta::name_list_instance names, generic::tuple tuple_type>
struct is_named_tuple<named_tuple<names, tuple_type>> : std::true_type {};
}  // namespace detail

namespace instance {

///
///
template <typename type>
concept named_tuple = xstd::detail::is_named_tuple<type>::value;

///
///
template <typename T>
concept reducible_named_tuple = named_tuple<reduction<T>>;

}  // namespace instance

template <meta::name_list_instance identifiers, generic::tuple T>
  requires(size(identifiers{}) == std::tuple_size<T>::value)  //
struct named_tuple : T {
  static constexpr auto names = identifiers{};
  using tuple_type            = T;
  static constexpr auto types = type_list_from<tuple_type>();

  template <meta::static_zstring name>
  using type = decltype(element<index<name>(names)>(types));

  static constexpr auto size() noexcept -> size_t { return tuple_type::size(); }

  // Enable all constructors of the underlying tuple type.
  //
  using tuple_type::tuple_type;

  ///
  ///
  template <size_t... indices>
  constexpr named_tuple(generic::reducible_unnamed_tuple auto&& x,
                        meta::static_index_list<indices...>) noexcept(  //
      noexcept(named_tuple(value<indices>(std::forward<decltype(x)>(x))...)))
      : named_tuple(value<indices>(std::forward<decltype(x)>(x))...) {}
  ///
  explicit constexpr named_tuple(
      generic::reducible_unnamed_tuple auto&& x) noexcept(  //
      noexcept(named_tuple(std::forward<decltype(x)>(x),
                           meta::static_index_list_from_iota<size()>())))
      : named_tuple(std::forward<decltype(x)>(x),
                    meta::static_index_list_from_iota<size()>()) {}

  //
  template <meta::static_zstring... names>
  constexpr named_tuple(generic::reducible_named_tuple auto&& x,
                        meta::name_list<names...>) noexcept(                //
      noexcept(tuple_type(value<names>(std::forward<decltype(x)>(x))...)))  //
      : tuple_type(value<names>(std::forward<decltype(x)>(x))...) {}

  explicit constexpr named_tuple(
      generic::reducible_named_tuple auto&& x) noexcept(  //
      noexcept(named_tuple(std::forward<decltype(x)>(x), names)))
      : named_tuple(std::forward<decltype(x)>(x), names) {}

  // Generic Copy/Move Construction
  //
  // explicit constexpr regular_tuple(
  //     instance::reducible_regular_tuple auto&& x) noexcept(  //
  //     noexcept(tuple_type(std::forward<decltype(x)>(x).tuple())))
  //     : tuple_type(std::forward<decltype(x)>(x).tuple()) {}

  // Generic Assignment Operator
  //
  constexpr named_tuple&
  operator=(generic::reducible_unnamed_tuple auto&& x) noexcept(noexcept(
      static_cast<tuple_type&>(*this) = std::forward<decltype(x)>(x))) {
    static_cast<tuple_type&>(*this) = std::forward<decltype(x)>(x);
    return *this;
  }

  template <meta::static_zstring... names>
  constexpr void assign(generic::reducible_named_tuple auto&& x,
                        meta::name_list<names...>) noexcept(  //
      noexcept(static_cast<tuple_type&>(*this).assign(
          value<names>(std::forward<decltype(x)>(x))...))) {
    static_cast<tuple_type&>(*this).assign(
        value<names>(std::forward<decltype(x)>(x))...);
  }

  constexpr named_tuple&
  operator=(generic::reducible_named_tuple auto&& x) noexcept(
      noexcept(assign(std::forward<decltype(x)>(x), names))) {
    assign(std::forward<decltype(x)>(x), names);
    return *this;
  }

  // We do not reorder types in here.
  // So, we can use the assign method of the base class.
  // template <size_t... indices>
  // constexpr void assign(generic::reducible_tuple auto&& x,
  //                       static_index_list<indices...>) noexcept(  //
  //     noexcept(assign(get<indices>(std::forward<decltype(x)>(x))...))) {
  //   assign(get<indices>(std::forward<decltype(x)>(x))...);
  // }

  // constexpr regular_tuple& operator=(
  //     generic::reducible_tuple auto&& x) noexcept(  //
  //     noexcept(assign(std::forward<decltype(x)>(x),
  //                     meta::static_index_list::iota<size()>{}))) {
  //   assign(std::forward<decltype(x)>(x),
  //          meta::static_index_list::iota<size()>{});
  //   return *this;
  // }

  /// Default Lexicographic Ordering
  ///
  friend auto operator<=>(const named_tuple&, const named_tuple&) = default;

  // Make forwarding of the underlying tuple type easy.
  //
  constexpr decltype(auto) tuple() & noexcept {
    return static_cast<tuple_type&>(*this);
  }
  constexpr decltype(auto) tuple() && noexcept {
    return static_cast<tuple_type&&>(*this);
  }
  constexpr decltype(auto) tuple() const& noexcept {
    return static_cast<const tuple_type&>(*this);
  }
  constexpr decltype(auto) tuple() const&& noexcept {
    return static_cast<const tuple_type&&>(*this);
  }
};

// template <template <typename...> typename tuple_template,
//           static_zstring... names,
//           typename... types>
// constexpr auto auto_named_tuple(types&&... t) {
//   using tuple_type = decltype(tuple_template(std::forward<types>(t)...));
//   return named_tuple<name_list<names...>, tuple_type>{
//       std::forward<types>(t)...};
//   // return named_tuple<name_list<names...>,
//   //                    std::unwrap_ref_decay_t<types>...>(
//   //     std::forward<types>(t)...);
// }

template <size_t index>
constexpr decltype(auto)
value(instance::reducible_named_tuple auto&& t) noexcept(
    noexcept(get<index>(std::forward<decltype(t)>(t).tuple()))) {
  return get<index>(std::forward<decltype(t)>(t).tuple());
}

template <meta::static_zstring name>
constexpr decltype(auto) value(
    instance::reducible_named_tuple auto&& t) noexcept {
  using type           = meta::reduction<decltype(t)>;
  constexpr auto names = type::names;
  return get<index<name>(names)>(std::forward<decltype(t)>(t).tuple());
}

template <meta::static_zstring... names>
constexpr void for_each(generic::reducible_named_tuple auto&& t,
                        auto&& f,
                        meta::name_list<names...>) {
  (f.template operator()<names>(value<names>(std::forward<decltype(t)>(t))),
   ...);
}

constexpr void for_each(generic::reducible_named_tuple auto&& x, auto&& f) {
  using tuple_type = meta::reduction<decltype(x)>;
  for_each(std::forward<decltype(x)>(x), std::forward<decltype(f)>(f),
           tuple_type::names);
}

/// This function is needed to make structured bindings available.
/// Here, it is a simple wrapper function template for 'value'.
///
template <size_t index>
constexpr decltype(auto) get(
    instance::reducible_named_tuple auto&& t) noexcept {
  return value<index>(std::forward<decltype(t)>(t));
}

}  // namespace ensketch::xstd

namespace std {

/// Provides support for using structured bindings with tuple.
///
template <ensketch::xstd::meta::name_list_instance names,
          ensketch::xstd::generic::tuple tuple_type>
struct tuple_size<ensketch::xstd::named_tuple<names, tuple_type>> {
  static constexpr size_t value = std::tuple_size<tuple_type>::value;
};

/// Provides support for using structured bindings with tuple.
///
template <size_t index,
          ensketch::xstd::meta::name_list_instance names,
          ensketch::xstd::generic::tuple tuple_type>
struct tuple_element<index, ensketch::xstd::named_tuple<names, tuple_type>> {
  using type = typename std::tuple_element<index, tuple_type>::type;
};

}  // namespace std
