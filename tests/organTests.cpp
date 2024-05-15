#include <catch2/catch_test_macros.hpp>
#include "organ.h"

TEST_CASE(
	"Organ getAgingRate"
	"[Organ]") {
	Organ organ(0.5, 100);
	REQUIRE(organ.getAgingRate() == 0.5);
}

TEST_CASE("Organ getHealth", "[Organ]") {
	Organ organ(0.5, 100);
	REQUIRE(organ.getHealth() == 100);
}

TEST_CASE("Organ takeDamage", "[Organ]") {
	Organ organ(0.5, 100);
	organ.takeDamage(10);
	REQUIRE(organ.getHealth() == 90);
}

TEST_CASE("Organ regenerateHealth", "[Organ]") {
	Organ organ(0.5, 100);
	organ.regenerateHealth(10);
	REQUIRE(organ.getHealth() == 110);
}