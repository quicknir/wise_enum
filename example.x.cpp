#include "wise_enum2.h"

#include <iostream>
#include <cassert>

WISE_ENUM(foo, (bar, 2), baz)

int main() {

  for (auto e : wise_enum::descriptor_range<foo>) {
      std::cerr << e.second;
  }
  std::cerr << wise_enum::to_string(foo::baz);

  std::cerr << wise_enum::detail::is_wise_enum<foo>;
  std::cerr << wise_enum::detail::is_wise_enum<int>;

  auto x1 = wise_enum::from_string<foo>("bar");
  auto x2 = wise_enum::from_string<foo>("barz");

  assert(x1.value() == foo::bar);
  assert(!x2);

  return 0;
}
