#include <array>
#include <utility>

// Macro interface
#define WISE_ENUM(name, ...) WISE_ENUM_IMPL(enum, name, , __VA_ARGS__)

#define WISE_ENUM_STORAGE(name, storage, ...)                                  \
  WISE_ENUM_IMPL(enum, name, : storage, __VA_ARGS__)

#define WISE_ENUM_CLASS(name, ...)                                             \
  WISE_ENUM_IMPL(enum class, name, , __VA_ARGS__)

#define WISE_ENUM_CLASS_STORAGE(name, storage, ...)                            \
  WISE_ENUM_IMPL(enum class, name, : storage, __VA_ARGS__)

namespace wise_enum {
namespace detail {

template <class T, std::size_t N>
constexpr std::array<T, N>
desc_array_to_array(const std::array<std::pair<T, const char *>, N> &a) {
  std::array<T, N> r;
  for (std::size_t i = 0; i != N; ++i) {
    r[i] = a[i].first;
  }
  return r;
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

template <class T>
struct Tag {};

template <class T>
constexpr const char *to_string(T t) {
  return wise_enum_to_string(t);
}

template <class T>
constexpr auto descriptor_range = wise_enum_descriptor_pair_array(Tag<T>{});

template <class T>
constexpr std::size_t size = descriptor_range<T>.size();

template <class T>
constexpr auto range = detail::desc_array_to_array(descriptor_range<T>);

// template <class T>
// constexpr WISE_ENUM_IMPL_OPTIONAL<T> from_string(const char *arg) {
//   auto it =
//       std::find_if(descriptor_range<T>.begin(), descriptor_range<T>.end(),
//                    [=](const auto &x) {
//                      return ::wise_enum::detail::strcmp(x.second, arg) == 0;
//                    });
//   if (it == descriptor_range<T>.end())
//     return {};

//   return it->first;
// }
}

#define WISE_ENUM_IMPL_NARG(...)                                               \
  WISE_ENUM_IMPL_NARG_(__VA_ARGS__, WISE_ENUM_IMPL_RSEQ_N())
#define WISE_ENUM_IMPL_NARG_(...) WISE_ENUM_IMPL_ARG_N(__VA_ARGS__)
#define WISE_ENUM_IMPL_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11,     \
                             _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, \
                             _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, \
                             _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, \
                             _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, \
                             _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, \
                             _62, _63, _64, N, ...)                            \
  N

#define WISE_ENUM_IMPL_RSEQ_N()                                                \
  64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46,  \
      45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28,  \
      27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  \
      9, 8, 7, 6, 5, 4, 3, 2, 1, 0

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

// Generated code

#define WISE_ENUM_IMPL_2(type, name, storage, x1, x2)                          \
  type name storage{WISE_ENUM_IMPL_ENUM_INIT(x1),                              \
                    WISE_ENUM_IMPL_ENUM_INIT(x2)};                             \
  constexpr auto wise_enum_descriptor_pair_array(::wise_enum::Tag<name>) {     \
    return std::array<std::pair<name, const char *>, 2>{                       \
        {WISE_ENUM_IMPL_DESC_PAIR(x1), WISE_ENUM_IMPL_DESC_PAIR(x2)}};         \
  }
