#pragma once

#include <stdexcept>
#include <type_traits>

#if __cplusplus == 201103
#define WISE_ENUM_CONSTEXPR_14
#else
#define WISE_ENUM_CONSTEXPR_14 constexpr
#endif

namespace wise_enum {

struct bad_optional_access : std::exception {
  const char *what() const noexcept override {
    return "Error, attempt to access valueless optional!";
  }
};

/* A simple, *forward* compatible optional implementation. That is, it does not
 * provide the full std::optional interface, but all the interface it does
 * provide is found on std::optional, so it should not be a breaking change to
 * upgrade to std::optional.
 */
template <class T>
class optional {
public:
  static_assert(std::is_enum<T>::value,
                "wise enum optional is only for enum types");
  optional() = default;
  optional(T t) : m_t(t), m_active(true) {}

  WISE_ENUM_CONSTEXPR_14 T &operator*() & { return m_t; }
  constexpr const T &operator*() const & { return m_t; }
  WISE_ENUM_CONSTEXPR_14 T &&operator*() && { return m_t; }
  constexpr const T &&operator*() const && { return m_t; }

  constexpr explicit operator bool() const noexcept { return m_active; }
  constexpr bool has_value() const noexcept { return m_active; }

  WISE_ENUM_CONSTEXPR_14 T &value() & {
    if (m_active)
      return m_t;
    else
      throw bad_optional_access{};
  }
  constexpr const T &value() const & {
    if (m_active)
      return m_t;
    else
      throw bad_optional_access{};
  }

  WISE_ENUM_CONSTEXPR_14 T &&value() && {
    if (m_active)
      return m_t;
    else
      throw bad_optional_access{};
  }
  constexpr const T &&value() const && {
    if (m_active)
      return m_t;
    else
      throw bad_optional_access{};
  }

  template <class U>
  constexpr T value_or(U &&u) {
    if (m_active)
      return m_t;
    else
      return std::forward<U>(u);
  }

  void reset() noexcept { m_active = false; }

  optional(const optional &other) = default;
  optional(optional &&other) = default;
  optional &operator=(const optional &other) = default;
  optional &operator=(optional &&other) = default;

private:
  T m_t;
  bool m_active = false;
};
} // namespace wise_enum
