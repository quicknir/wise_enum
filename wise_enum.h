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

// Macro interface
#define WISE_ENUM(name, ...) WISE_ENUM_IMPL(enum, name, , __VA_ARGS__)

#define WISE_ENUM_STORAGE(name, storage, ...)                                  \
  WISE_ENUM_IMPL(enum, name, : storage, __VA_ARGS__)

#define WISE_ENUM_CLASS(name, ...)                                             \
  WISE_ENUM_IMPL(enum class, name, , __VA_ARGS__)

#define WISE_ENUM_CLASS_STORAGE(name, storage, ...)                            \
  WISE_ENUM_IMPL(enum class, name, : storage, __VA_ARGS__)

namespace wise_enum {

template <class T>
constexpr const char *to_string(T t) {
  return wise_enum_to_string(t);
}

template <class T>
constexpr auto
    descriptor_range = wise_enum_descriptor_pair_array(detail::Tag<T>{});

template <class T>
constexpr std::size_t size = descriptor_range<T>.size();

template <class T>
constexpr auto range = detail::desc_array_to_array(descriptor_range<T>);

template <class T>
static constexpr bool is_wise_enum_v = detail::is_wise_enum<T>;

template <class T>
struct is_wise_enum : std::integral_constant<bool, is_wise_enum_v<T>> {};

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
