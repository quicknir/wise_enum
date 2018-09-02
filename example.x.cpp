#if __cplusplus == 201402L
#include <experimental/optional>
#define WISE_ENUM_OPTIONAL std::experimental::optional
#endif

#include "wise_enum.h"

#include <cassert>
#include <iostream>

// Equivalent to enum Color {GREEN = 2, RED};
WISE_ENUM(Color, (GREEN, 2), RED)

// Equivalent to enum class MoreColor : char {BLUE, BLACK = 1};
WISE_ENUM_CLASS((MoreColor, int64_t), BLUE, (BLACK, 1))

// Inside a class, must use a different macro, but still works
struct Bar {
    WISE_ENUM_MEMBER(Foo, BUZ)
};

int main() {

  // Number of enumerations:
  static_assert(wise_enum::size<Color> == 2, "");
  std::cerr << "Number of enumerators: " << wise_enum::size<Color> << "\n";

  // Iterate over enums
  std::cerr << "Enum values and names:\n";
  for (auto e : wise_enum::range<Color>) {
    std::cerr << static_cast<int>(e.value) << " " << e.name << "\n";
  }
  std::cerr << "\n";

  // Convert any enum to a string
  std::cerr << wise_enum::to_string(Color::RED) << "\n";

  // Convert any string to an optional enum
  auto x1 = wise_enum::from_string<Color>("GREEN");
  auto x2 = wise_enum::from_string<Color>("Greeeeeeen");

  assert(x1.value() == Color::GREEN);
  assert(!x2);

  // Everything is constexpr, and a type trait is made available for easy use in
  // enable_if/tag dispatch
  static_assert(wise_enum::is_wise_enum_v<Color>, "");
  static_assert(!wise_enum::is_wise_enum_v<int>, "");
  enum flub { blub, glub };
  static_assert(!wise_enum::is_wise_enum_v<flub>, "");

  // Assert underlying type
  static_assert(std::is_same<int64_t, std::underlying_type_t<MoreColor>>::value, "");

  return 0;
}
