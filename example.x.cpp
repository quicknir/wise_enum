#if __cplusplus == 201402L
#include <experimental/optional>
#define WISE_ENUM_OPTIONAL std::experimental::optional
#endif

#include "wise_enum.h"

#include <cassert>
#include <iostream>

WISE_ENUM(foo, (bar, 2), baz)

int main() {

  // Number of enumerations:
  std::cerr << "Number of enumerators: " << wise_enum::size<foo> << "\n";

  // Iterate over the enums:
  std::cerr << "Enum values:";
  for (auto e : wise_enum::range<foo>) {
    std::cerr << " " << static_cast<int>(e);
  }
  std::cerr << "\n";

  // Iterate with descriptors
  std::cerr << "Enum values and descriptors:\n";
  for (auto e : wise_enum::descriptor_range<foo>) {
    std::cerr << static_cast<int>(e.first) << " " << e.second << "\n";
  }
  std::cerr << "\n";

  // Convert any enum to a string
  std::cerr << wise_enum::to_string(foo::baz) << "\n";

  // Convert any string to an optional enum
  auto x1 = wise_enum::from_string<foo>("bar");
  auto x2 = wise_enum::from_string<foo>("barz");

  assert(x1.value() == foo::bar);
  assert(!x2);

  std::cerr << wise_enum::detail::is_wise_enum<foo>;
  std::cerr << wise_enum::detail::is_wise_enum<int>;

  return 0;
}
