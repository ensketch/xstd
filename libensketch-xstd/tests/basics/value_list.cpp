#include <ensketch/xstd/meta/value_list.hpp>

// Only the template itself needs to be available
// to access all its functions by making use of ADL.
//
using ensketch::xstd::meta::value_list;

// Check if a given type is an instance of the 'value_list' template.
//
namespace instance = ensketch::xstd::meta::instance;
static_assert(instance::value_list<value_list<>>);
static_assert(instance::value_list<value_list<-1>>);
static_assert(instance::value_list<value_list<'c', -1>>);
static_assert(instance::value_list<value_list<-1, 1u, 'c'>>);
static_assert(
    instance::value_list<value_list<-1, 1u, 'c', value_list<-1, 'c'>{}>>);

// Get the number of types inside a type list.
//
static_assert(size(value_list<>{}) == 0);
static_assert(size(value_list<-1>{}) == 1);
static_assert(size(value_list<'c'>{}) == 1);
static_assert(size(value_list<-1, 'c'>{}) == 2);
static_assert(size(value_list<-1, 'c', 1u>{}) == 3);

// Check whether a 'value_list' instance contains no values.
//
static_assert(empty(value_list<>{}));
static_assert(!empty(value_list<-1>{}));
static_assert(!empty(value_list<-1, 'c'>{}));
static_assert(!empty(value_list<-1, 'c', 1u>{}));

// Check if a condition holds for all types.
//
constexpr auto bigger_one = []<auto x> { return x > 1; };
static_assert(for_all(value_list<>{}, bigger_one));
static_assert(!for_all(value_list<-1>{}, bigger_one));
static_assert(for_all(value_list<'c'>{}, bigger_one));
static_assert(!for_all(value_list<-1, 2u>{}, bigger_one));
static_assert(!for_all(value_list<-1, 'c'>{}, bigger_one));
static_assert(for_all(value_list<2u, 'c'>{}, bigger_one));

// Check if a value exists inside the list with a certain condition.
//
static_assert(!exists(value_list<>{}, bigger_one));
static_assert(exists(value_list<'c'>{}, bigger_one));
static_assert(!exists(value_list<-1>{}, bigger_one));
static_assert(exists(value_list<2u>{}, bigger_one));
static_assert(!exists(value_list<-2, -1>{}, bigger_one));
static_assert(exists(value_list<'c', -1>{}, bigger_one));
static_assert(exists(value_list<-1, 2u>{}, bigger_one));

// Check if a 'value_list' instance contains a specific type.
//
static_assert(!contains(value_list<>{}, -1));
static_assert(!contains(value_list<>{}, 'c'));
static_assert(contains(value_list<-1>{}, -1));
static_assert(!contains(value_list<'c'>{}, -1));
static_assert(contains(value_list<-1, 'c'>{}, -1));
static_assert(contains(value_list<-1, 'c'>{}, 'c'));
static_assert(!contains(value_list<-1, 'c'>{}, 2u));
//
static_assert(!(value_list<>{} | ensketch::xstd::meta::contains(-1)));
static_assert(!(value_list<>{} | ensketch::xstd::meta::contains('c')));
static_assert((value_list<-1>{} | ensketch::xstd::meta::contains(-1)));
static_assert(!(value_list<'c'>{} | ensketch::xstd::meta::contains(-1)));
static_assert((value_list<-1, 'c'>{} | ensketch::xstd::meta::contains(-1)));
static_assert((value_list<-1, 'c'>{} | ensketch::xstd::meta::contains('c')));
static_assert(!(value_list<-1, 'c'>{} | ensketch::xstd::meta::contains(2u)));

// Check whether the given value list is a set
//
static_assert(is_set(value_list<>{}));
static_assert(is_set(value_list<-1>{}));
static_assert(is_set(value_list<'c'>{}));
//
static_assert(is_set(value_list<-1, 'c'>{}));
static_assert(is_set(value_list<'c', -1>{}));
static_assert(!is_set(value_list<'c', 'c'>{}));
static_assert(!is_set(value_list<-1, -1>{}));

// Access values of a 'value_list' instance by their index.
//
static_assert(element<0>(value_list<-1>{}) == -1);
static_assert(element<0>(value_list<-1, 'c'>{}) == -1);
static_assert(element<1>(value_list<-1, 'c'>{}) == 'c');
static_assert(element<0>(value_list<2u, -1, 'c'>{}) == 2u);
static_assert(element<1>(value_list<2u, -1, 'c'>{}) == -1);
static_assert(element<2>(value_list<2u, -1, 'c'>{}) == 'c');
//
// static_assert((value_list<1, 2, 3>{} | ensketch::xstd::element(0)) == 1);

//
//
static_assert(index<-1>(value_list<-1>{}) == 0);
static_assert(index<-1>(value_list<'c', -1>{}) == 1);
