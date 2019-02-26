#include <wise_enum.h>

#include <catch2/catch.hpp>
#include <string_view>
#include <type_traits>

WISE_ENUM_CLASS((Color, int64_t), (BLACK, 1), BLUE, RED)

enum class MoreColor
{
    RED,
    GREEN,
    OLIVE
};
WISE_ENUM_ADAPT(MoreColor, RED, GREEN);

TEST_CASE("invalid mapping", "[wise_enum][cxx17][invalid_enumerator]")
{
    static_assert(std::is_same_v<wise_enum::string_type, std::string_view>);

    auto default_constructed{wise_enum::to_string(Color{})};
    CHECK(default_constructed.begin() == default_constructed.end());
    CHECK(default_constructed.size() == 0); // Shouldn't be necessary, but duh...
    CHECK(default_constructed.data() == nullptr);
    CHECK(default_constructed == "");

    auto answer{wise_enum::to_string(static_cast<Color>(42))};
    CHECK(answer.begin() == answer.end());
    CHECK(answer.size() == 0);
    CHECK(answer.data() == nullptr);
    CHECK(answer == "");


    auto greenish{wise_enum::to_string(MoreColor::OLIVE)};
    CHECK(greenish.begin() == greenish.end());
    CHECK(greenish.size() == 0);
    CHECK(greenish.data() == nullptr);
    CHECK(greenish == "");
}
