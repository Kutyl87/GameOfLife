#include "organ.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Organ getAgingRate" "[Organ]"){
	Organ organ(0.5, 100);
	REQUIRE(organ.getAgingRate() == 0.5);
}