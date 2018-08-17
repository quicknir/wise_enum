#pragma once

#include <array>
#include <type_traits>
#include <utility>

namespace wise_enum {

namespace detail {

template <class T>
struct Tag {};

constexpr void wise_enum_descriptor_pair_array(...);

template <class T>
static constexpr bool is_wise_enum =
    !std::is_same<void,
                  decltype(wise_enum_descriptor_pair_array(Tag<T>{}))>::value;

template <class T, std::size_t N, std::size_t... Is>
constexpr std::array<T, N>
desc_array_to_array_impl(const std::array<std::pair<T, const char *>, N> &a,
                         std::index_sequence<Is...>) {
  return {a[Is].first...};
}

template <class T, std::size_t N>
constexpr std::array<T, N>
desc_array_to_array(const std::array<std::pair<T, const char *>, N> &a) {
  return desc_array_to_array_impl(a, std::make_index_sequence<N>{});
}

constexpr int strcmp(const char *s1, const char *s2) {
  while (*s1 && (*s1 == *s2))
    s1++, s2++;
  if (*s1 < *s2) {
    return -1;
  }
  if (*s1 > *s2) {
    return 1;
  } else {
    return 0;
  }
}
}
}

#define WISE_ENUM_IMPL_NARG(...)                                               \
  WISE_ENUM_IMPL_NARG_(__VA_ARGS__, WISE_ENUM_IMPL_RSEQ_N())
#define WISE_ENUM_IMPL_NARG_(...) WISE_ENUM_IMPL_ARG_N(__VA_ARGS__)

// ARG_N and RSEQ_N defined in _generated.h

#define WISE_ENUM_IMPL_XSTR(s) #s
#define WISE_ENUM_IMPL_STR(s) WISE_ENUM_IMPL_XSTR(s)

#define WISE_ENUM_IMPL_SECOND_2(x, n, ...) n
#define WISE_ENUM_IMPL_SECOND(...) WISE_ENUM_IMPL_SECOND_2(__VA_ARGS__, 0)

#define WISE_ENUM_IMPL_ENUM_INIT_2(x, y) 0, x = y,
#define WISE_ENUM_IMPL_ENUM_INIT(x)                                            \
  WISE_ENUM_IMPL_SECOND(WISE_ENUM_IMPL_ENUM_INIT_2 x, x)

#define WISE_ENUM_IMPL_ENUM_2(x, y) 0, x,
#define WISE_ENUM_IMPL_ENUM(x) WISE_ENUM_IMPL_SECOND(WISE_ENUM_IMPL_ENUM_2 x, x)

#define WISE_ENUM_IMPL_ENUM_STR_2(x, y) 0, WISE_ENUM_IMPL_STR(x),
#define WISE_ENUM_IMPL_ENUM_STR(x)                                             \
  WISE_ENUM_IMPL_SECOND(WISE_ENUM_IMPL_ENUM_STR_2 x, WISE_ENUM_IMPL_STR(x))

#define WISE_ENUM_IMPL_DESC_PAIR(x)                                            \
  std::make_pair(WISE_ENUM_IMPL_ENUM(x), WISE_ENUM_IMPL_ENUM_STR(x))

#define WISE_ENUM_IMPL_CAT(a, b) WISE_ENUM_IMPL_PRIMITIVE_CAT(a, b)
#define WISE_ENUM_IMPL_PRIMITIVE_CAT(a, b) a##b

#define WISE_ENUM_IMPL(type, name, storage, ...)                               \
  WISE_ENUM_IMPL_CAT(WISE_ENUM_IMPL_, WISE_ENUM_IMPL_NARG(__VA_ARGS__))        \
  (type, name, storage, __VA_ARGS__)
