#include "wise_enum.h"

#include <iostream>

namespace user {
WISE_ENUM_CLASS_STORAGE(foo, std::size_t, (bar, 3), baz);
}

int main() {
  std::cerr << std::boolalpha;
  static_assert(sizeof(user::foo) == sizeof(std::size_t), "");

  auto x = user::foo::bar;
  std::cerr << wise_enum::to_string(x) << " is: " << static_cast<int>(x)
            << "\n";

  auto y_oops = wise_enum::from_string<user::foo>("babbbb");
  std::cerr << "incorrect string, optional return: "
            << static_cast<bool>(y_oops) << "\n";
  auto y_opt = wise_enum::from_string<user::foo>("bar");
  std::cerr << "correct string, optional return: " << static_cast<bool>(y_opt)
            << "\n";

  if (!y_opt)
    return -1;

  auto y = *y_opt;

  std::cerr << "x and y equal (to bar): " << (y == x) << "\n";

  auto z = user::foo::baz;
  std::cerr << "z is baz, z and y compare: " << (z == y) << "\n";

  for (const auto &e : wise_enum::descriptor_range<user::foo>) {
    std::cerr << static_cast<int>(e.first) << ": " << e.second << "\n";
  }

  return 0;
}
