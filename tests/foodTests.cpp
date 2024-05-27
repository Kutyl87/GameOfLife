#include <catch2/catch_test_macros.hpp>
#include "food.h"

TEST_CASE("Food getSize", "[Food]") {
	Food food = Food(3.0, 2.0, 3.0);
	REQUIRE(food.getSize() == 3.0);
}