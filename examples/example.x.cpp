#include "../wise_enum.h"
#include "../compact_optional.h"

#include <cassert>
#include <iostream>

// Equivalent to enum Color {GREEN = 2, RED};
namespace my_lib {
WISE_ENUM(Color, (GREEN, 2), RED)
}

// Equivalent to enum class MoreColor : int64_t {BLUE, BLACK = 1};
WISE_ENUM_CLASS((MoreColor, int64_t), BLUE, (BLACK, 1))

// Inside a class, must use a different macro, but still works
struct Bar {
  WISE_ENUM_MEMBER(Foo, BUZ)
};

// Adapt an existing enum you don't control so it works with generic code
namespace another_lib {
enum class SomebodyElse { FIRST, SECOND };
}
WISE_ENUM_ADAPT(another_lib::SomebodyElse, FIRST, SECOND)

int main() {

  // Number of enumerations:
  static_assert(wise_enum::size<my_lib::Color> == 2, "");
  std::cerr << "Number of enumerators: "
            << wise_enum::size<my_lib::Color> << "\n";

  // Iterate over enums
  std::cerr << "Enum values and names:\n";
  for (auto e : wise_enum::range<my_lib::Color>) {
    std::cerr << static_cast<int>(e.value) << " " << e.name << "\n";
  }
  std::cerr << "\n";

  // Convert any enum to a string
  std::cerr << wise_enum::to_string(my_lib::Color::RED) << "\n";

  // Convert any string to an optional enum
  auto x1 = wise_enum::from_string<my_lib::Color>("GREEN");
  auto x2 = wise_enum::from_string<my_lib::Color>("Greeeeeeen");

  assert(x1.value() == my_lib::Color::GREEN);
  assert(!x2);

  // Everything is constexpr, and a type trait is made available for easy use in
  // enable_if/tag dispatch
  static_assert(wise_enum::is_wise_enum_v<my_lib::Color>, "");
  static_assert(!wise_enum::is_wise_enum_v<int>, "");
  enum flub { blub, glub };
  static_assert(!wise_enum::is_wise_enum_v<flub>, "");
  // We made a regular enum wise!
  static_assert(wise_enum::is_wise_enum_v<another_lib::SomebodyElse>, "");

  // Assert underlying type
  static_assert(std::is_same<int64_t, std::underlying_type_t<MoreColor>>::value,
                "");

  // Full API available for adapted wise enums
  for (auto e : wise_enum::range<another_lib::SomebodyElse>) {
    std::cerr << static_cast<int>(e.value) << " "
              << wise_enum::to_string(e.value) << "\n";
  }

  // Compact optional demo
  wise_enum::compact_optional<my_lib::Color> o;
  assert(!o);
  assert(o.value_or(my_lib::Color::RED) == my_lib::Color::RED);
  o = my_lib::Color::GREEN;
  assert(o.value() == my_lib::Color::GREEN);
  o.reset();
  assert(!o);

  return 0;
}
