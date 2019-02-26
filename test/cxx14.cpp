#include <wise_enum.h>

#include <catch2/catch.hpp>
#include <type_traits>

WISE_ENUM_CLASS((Color, int64_t), (BLACK, 1), BLUE, RED)

enum class MoreColor
{
    RED,
    GREEN,
    OLIVE
};
WISE_ENUM_ADAPT(MoreColor, RED, GREEN);

TEST_CASE("invalid mapping", "[wise_enum][cxx14][invalid_enumerator]")
{
    static_assert(std::is_same<wise_enum::string_type, char const*>::value, "string_type char const* expected for C++14");

    CHECK(wise_enum::to_string(Color{}) == nullptr);
    CHECK(wise_enum::to_string(static_cast<Color>(42)) == nullptr);
    CHECK(wise_enum::to_string(MoreColor::OLIVE) == nullptr);
}
