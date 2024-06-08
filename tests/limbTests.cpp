#include <catch2/catch_test_macros.hpp>
#include "limb.h"

TEST_CASE("Limb getLength", "[Limb]") {
	std::shared_ptr<Limb> parentObject =
		std::make_shared<Limb>(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 2, 1);
	Limb limb = Limb(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 5.0f, 2.0f,
					 parentObject);
	REQUIRE(limb.getLength() == 5.0f);
}

TEST_CASE("Limb getWidth", "[Limb]") {
	std::shared_ptr<Limb> parentObject =
		std::make_shared<Limb>(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 2, 1);
	Limb limb = Limb(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 5.0f, 2.0f,
					 parentObject);
	REQUIRE(limb.getWidth() == 2.0f);
}

TEST_CASE("Limb setLength", "[Limb]") {
	std::shared_ptr<Limb> parentObject =
		std::make_shared<Limb>(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 2, 1);
	Limb limb = Limb(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 5.0f, 2.0f,
					 parentObject);
	limb.setLength(3.0f);
	REQUIRE(limb.getLength() == 3.0f);
}

TEST_CASE("Limb setWidth", "[Limb]") {
	std::shared_ptr<Limb> parentObject =
		std::make_shared<Limb>(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 2, 1);
	Limb limb = Limb(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), 5.0f, 2.0f,
					 parentObject);
	limb.setWidth(1.0f);
	REQUIRE(limb.getWidth() == 1.0f);
}