#include "wise_enum2.h"

#include <iostream>

WISE_ENUM(foo, (bar, 2), baz);

int main() {
  // auto x = foo::bar;

  for (auto e : wise_enum::descriptor_range<foo>) {
      std::cerr << e.second;
  }

  return 0;
}
