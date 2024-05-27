#include <catch2/catch_test_macros.hpp>
#include "food.h"

TEST_CASE("Food getSize", "[Food]") {
	Food food = Food(3.0, 2.0, 3.0);
	REQUIRE(food.getSize() == 3.0);
}

TEST_CASE("Food getValuePerUnit", "[Food]") {
	Food food = Food(3.0, 2.0, 3.0);
	REQUIRE(food.getValuePerUnit() == 2.0);
}

TEST_CASE("Food getToxinLevel", "[Food]") {
	Food food = Food(3.0, 2.0, 3.0);
	REQUIRE(food.getToxinLevel() == 3.0);
}

TEST_CASE("Food setSize", "[Food]") {
	Food food = Food(3.0, 2.0, 3.0);
	food.setSize(2.0);
	REQUIRE(food.getSize() == 2.0);
}

TEST_CASE("Food setValuePerUnit", "[Food]") {
	Food food = Food(3.0, 2.0, 3.0);
	food.setValuePerUnit(3.0);
	REQUIRE(food.getValuePerUnit() == 3.0);
}

TEST_CASE("Food setToxinLevel", "[Food]") {
	Food food = Food(3.0, 2.0, 3.0);
	food.setToxinLevel(2.0);
	REQUIRE(food.getToxinLevel() == 2.0);
}