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

// ARG_N and RSEQ_N defined in wise_enum_generated.h

// Building blocks; credit to:
// https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms

#define WISE_ENUM_IMPL_COMMA() ,
#define WISE_ENUM_IMPL_NOTHING()

#define WISE_ENUM_IMPL_CAT(a, ...) WISE_ENUM_IMPL_PRIMITIVE_CAT(a, __VA_ARGS__)
#define WISE_ENUM_IMPL_PRIMITIVE_CAT(a, ...) a##__VA_ARGS__

#define WISE_ENUM_IMPL_XSTR(s) #s
#define WISE_ENUM_IMPL_STR(s) WISE_ENUM_IMPL_XSTR(s)

#define WISE_ENUM_IMPL_IIF(c) WISE_ENUM_IMPL_CAT(WISE_ENUM_IMPL_IIF_, c)
#define WISE_ENUM_IMPL_IIF_0(t, f) f
#define WISE_ENUM_IMPL_IIF_1(t, f) t

#define WISE_ENUM_IMPL_CHECK_N(x, n, ...) n
#define WISE_ENUM_IMPL_CHECK(...) WISE_ENUM_IMPL_CHECK_N(__VA_ARGS__, 0, )
#define WISE_ENUM_IMPL_PROBE(x) x, 1,

#define WISE_ENUM_IMPL_IS_PAREN(x)                                             \
  WISE_ENUM_IMPL_CHECK(WISE_ENUM_IMPL_IS_PAREN_PROBE x)
#define WISE_ENUM_IMPL_IS_PAREN_PROBE(...) WISE_ENUM_IMPL_PROBE(~)

// Use building blocks to conditionally process enumerators; they can either be
// just an identifier, or (identifier, value)
#define WISE_ENUM_IMPL_ENUM_INIT_2(x, ...) x = __VA_ARGS__
#define WISE_ENUM_IMPL_ENUM_INIT(x)                                            \
  WISE_ENUM_IMPL_IIF(WISE_ENUM_IMPL_IS_PAREN(x))                               \
  (WISE_ENUM_IMPL_ENUM_INIT_2 x, x)

#define WISE_ENUM_IMPL_ENUM_2(x, ...) x
#define WISE_ENUM_IMPL_ENUM(x)                                                 \
  WISE_ENUM_IMPL_IIF(WISE_ENUM_IMPL_IS_PAREN(x))                               \
  (WISE_ENUM_IMPL_ENUM_2 x, x)

#define WISE_ENUM_IMPL_ENUM_STR(x) WISE_ENUM_IMPL_STR(WISE_ENUM_IMPL_ENUM(x))

#define WISE_ENUM_IMPL_DESC_PAIR(x)                                            \
  std::make_pair(WISE_ENUM_IMPL_ENUM(x), WISE_ENUM_IMPL_ENUM_STR(x))

#define WISE_ENUM_IMPL_SWITCH_CASE(x)                                          \
  case WISE_ENUM_IMPL_ENUM(x):                                                 \
    return WISE_ENUM_IMPL_ENUM_STR(x);

#define WISE_ENUM_IMPL(type, name, storage, ...)                               \
  WISE_ENUM_IMPL_2(type, name, storage, WISE_ENUM_IMPL_NARG(__VA_ARGS__),      \
                   __VA_ARGS__)

#define WISE_ENUM_IMPL_2(type, name, storage, num_enums, ...)                  \
  WISE_ENUM_IMPL_3(type, name, storage, num_enums,                             \
                   WISE_ENUM_IMPL_CAT(WISE_ENUM_IMPL_LOOP_, num_enums),        \
                   __VA_ARGS__)

#define WISE_ENUM_IMPL_3(type, name, storage, num_enums, loop, ...)            \
  type name storage{                                                           \
      loop(WISE_ENUM_IMPL_ENUM_INIT, WISE_ENUM_IMPL_COMMA, __VA_ARGS__)};      \
                                                                               \
  constexpr auto wise_enum_descriptor_pair_array(                              \
      ::wise_enum::detail::Tag<name>) {                                        \
    return std::array<std::pair<name, const char *>, num_enums>{               \
        {loop(WISE_ENUM_IMPL_DESC_PAIR, WISE_ENUM_IMPL_COMMA, __VA_ARGS__)}};  \
  }                                                                            \
                                                                               \
  constexpr const char *wise_enum_to_string(name e) {                          \
    switch (e) {                                                               \
      loop(WISE_ENUM_IMPL_SWITCH_CASE, WISE_ENUM_IMPL_NOTHING, __VA_ARGS__)    \
    }                                                                          \
    return nullptr;                                                            \
  }
