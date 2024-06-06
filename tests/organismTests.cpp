#include <catch2/catch_test_macros.hpp>
#include "organism.h"

TEST_CASE("Organism: Default constructor", "[Organism]") {
	Organism organism;
	REQUIRE(organism.getChildren().empty());
	REQUIRE(organism.getChildren().empty());
}

TEST_CASE("Organism: Constructor with parameters", "[Organism]") {
	std::vector<std::unique_ptr<Limb>> children;
	std::vector<std::unique_ptr<Organ>> organs;
	std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
	std::array<float, 3> rotation = {0.0f, 0.0f, 0.0f};
	std::weak_ptr<Object> parent;
	children.push_back(std::make_unique<Limb>(position, rotation, parent, 2, 1));
	organs.push_back(std::make_unique<Organ>(0.5, 100));
	Organism organism(std::move(children), std::move(organs));
	REQUIRE(organism.getChildren().size() == 1);
	REQUIRE(organism.getOrgans().size() == 1);
}

//
TEST_CASE("Organism: Add child", "[Organism]") {
	Organism organism;
	std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
	std::array<float, 3> rotation = {0.0f, 0.0f, 0.0f};
	std::weak_ptr<Object> parent;
	organism.addChild(std::make_unique<Limb>(position, rotation, parent, 2, 1));
	REQUIRE(organism.getChildren().size() == 1);
}

//
TEST_CASE("Organism: Add organ", "[Organism]") {
	Organism organism;
	organism.addOrgan(std::make_unique<Organ>(0.5, 100));
	REQUIRE(organism.getOrgans().size() == 1);
}

//
TEST_CASE("Organism: Remove child by index", "[Organism]") {
	Organism organism;
	std::array<float, 3> position = {0.0f, 0.0f, 0.0f};
	std::array<float, 3> rotation = {0.0f, 0.0f, 0.0f};
	std::weak_ptr<Object> parent;
	organism.addChild(std::make_unique<Limb>(position, rotation, parent, 2, 1));
	organism.removeChildByIndex(0);
	REQUIRE(organism.getChildren().empty());
}

TEST_CASE("Organism: Remove organ by index", "[Organism]") {
	Organism organism;
	organism.addOrgan(std::make_unique<Organ>(0.5, 100));
	organism.removeOrganByIndex(0);
	REQUIRE(organism.getOrgans().empty());
}