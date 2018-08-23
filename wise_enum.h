#pragma once

#include "wise_enum_detail.h"
#include "wise_enum_generated.h"

#include <algorithm>
#include <array>
#include <type_traits>
#include <utility>

// optional type needed for interface
#ifndef WISE_ENUM_OPTIONAL
#if __cplusplus == 201703L
#include <optional>
#define WISE_ENUM_OPTIONAL std::optional
#endif
#endif

/*
 Macro interface

 All macro enums take the name of the enum as the first argument. Those
 suffixed with _STORAGE take a second argument which is the explicit storage
 parameter. After that, the macros take a variadic enumerator list. Each entry
 in the list can be either a legal identifier name, or it can be a
 parenthesized pair where the first entry is the identifier and the second
 entry is the initializer. This declares an enum (not enum class) of implicit
 storage capacity

 Usage notes:
   1) Must be used at namespace scope, not nested inside a class.
   2) the initializer cannot contain an unparenthesized comma, nor redundant
      surrounding parentheses
   3) A semi-colon after the macro invocation is not necessary, and your
      compiler may warn for it
*/

// Declare an enum of implicit storage
#define WISE_ENUM(name, ...) WISE_ENUM_IMPL(enum, name, , __VA_ARGS__)

// Declare an enum of explicitly specified storage
#define WISE_ENUM_STORAGE(name, storage, ...)                                  \
  WISE_ENUM_IMPL(enum, name, : storage, __VA_ARGS__)

// Declare an enum class of implicit storage
#define WISE_ENUM_CLASS(name, ...)                                             \
  WISE_ENUM_IMPL(enum class, name, , __VA_ARGS__)

// Declare an enum class of explicitly specified storage
#define WISE_ENUM_CLASS_STORAGE(name, storage, ...)                            \
  WISE_ENUM_IMPL(enum class, name, : storage, __VA_ARGS__)

namespace wise_enum {

// Returns the string representation of an enumerator
template <class T>
constexpr const char *to_string(T t) {
  return wise_enum_to_string(t);
}

// For a given wise enum type, this variable allows iteration over enumerators
// and their string literal descriptors in the declared order. Each iterated
// object has type std::pair<T, const char*>
template <class T>
constexpr auto
    descriptor_range = wise_enum_descriptor_pair_array(detail::Tag<T>{});

// This variable is equal to the number of enumerators for the wise enum type.
template <class T>
constexpr std::size_t size = descriptor_range<T>.size();

// For a given wise enum type, this variable allows iteration over enumerators
// in the declared order.
template <class T>
constexpr auto range = detail::desc_array_to_array(descriptor_range<T>);

// A type trait; this allows checking if a type is a wise_enum in generic code
template <class T>
static constexpr bool is_wise_enum_v = detail::is_wise_enum<T>;

template <class T>
struct is_wise_enum : std::integral_constant<bool, is_wise_enum_v<T>> {};

// Converts a string literal into a wise enum. Returns an optional<T>; if no
// enumerator has descriptor matching the string, the optional is returned
// empty.
template <class T>
constexpr WISE_ENUM_OPTIONAL<T> from_string(const char *arg) {
  auto it =
      std::find_if(descriptor_range<T>.begin(), descriptor_range<T>.end(),
                   [=](const auto &x) {
                     return ::wise_enum::detail::strcmp(x.second, arg) == 0;
                   });
  if (it == descriptor_range<T>.end())
    return {};

  return it->first;
}
}
