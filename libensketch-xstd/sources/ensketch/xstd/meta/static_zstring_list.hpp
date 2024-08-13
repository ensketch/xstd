#pragma once
#include <ensketch/xstd/meta/static_zstring.hpp>
#include <ensketch/xstd/meta/value_list.hpp>

namespace ensketch::xstd::meta {

/// Static zstring lists are value lists
/// that contain only static zstrings.
/// No inheritance is used, because
/// all value list meta functions still need to be applyable.
///
template <static_zstring... strings>
using static_zstring_list = value_list<strings...>;

namespace detail {
template <typename type>
struct is_static_zstring_list : std::false_type {};
template <static_zstring... strings>
struct is_static_zstring_list<static_zstring_list<strings...>>
    : std::true_type {};
}  // namespace detail

// The namespace 'instance' is always used to provide concepts
// that check whether a given type is
// the instance of a specific type template.
//
namespace instance {

/// The concept checks whether the given type
/// is a static zstring list.
///
template <typename list>
concept static_zstring_list = detail::is_static_zstring_list<list>::value;

}  // namespace instance

}  // namespace ensketch::xstd::meta
