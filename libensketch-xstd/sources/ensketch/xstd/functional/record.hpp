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
#include <ensketch/xstd/functional/defaults.hpp>
//
#include <ensketch/xstd/meta/index_list.hpp>
#include <ensketch/xstd/meta/map.hpp>
#include <ensketch/xstd/meta/name_list.hpp>
#include <ensketch/xstd/meta/type_list.hpp>

namespace ensketch::xstd {

template <meta::string name, typename type>
using record_field = meta::pair<meta::value_tag<name>, type>;

namespace detail {

template <typename type>
struct is_record_field : std::false_type {};

template <meta::string name, typename type>
struct is_record_field<record_field<name, type>> : std::true_type {};

}  // namespace detail

template <typename type>
concept record_field_instance = detail::is_record_field<type>::value;

template <record_field_instance... elements>
using record_map = meta::map<elements...>;

namespace detail {

template <typename type>
struct is_record_map : std::false_type {};

template <typename... elements>
struct is_record_map<record_map<elements...>> : std::true_type {};

}  // namespace detail

template <typename type>
concept record_map_instance = detail::is_record_map<type>::value;

template <typename type>
concept has_record_elements = requires(type value) {
  { record_elements(value) } -> record_map_instance;
};

template <typename record, meta::string key, typename type>
concept record_gettable_for = requires(record&& r) {
  {
    get<key>(r)
  } -> std::same_as<decltype(std::forward_like<record>(std::declval<type>()))>;
};

template <typename record>
concept record_like =
    has_record_elements<record> &&
    all_of(decltype(record_elements(std::declval<record>())){},
           []<typename field> {
             return record_gettable_for<record, field::key::value,
                                        typename field::value>;
           });

}  // namespace ensketch::xstd
