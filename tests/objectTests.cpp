#include <catch2/catch_test_macros.hpp>
#include "object.h"

TEST_CASE("Object getPosition", "[Object]") {
	std::weak_ptr<Object> null_object;
	Object object(std::array<float, 3>{1.0f, 2.0f, 3.0f}, std::array<float, 3>{0.0f, 0.0f, 0.0f}, null_object);
	REQUIRE(object.getPosition() == std::array<float, 3>{1.0f, 2.0f, 3.0f});
}

TEST_CASE("Object getRotation", "[Object]") {
	std::weak_ptr<Object> null_object;
	Object object(std::array<float, 3>{1.0f, 2.0f, 3.0f}, std::array<float, 3>{0.0f, 0.0f, 0.0f}, null_object);
	REQUIRE(object.getRotation() == std::array<float, 3>{0.0f, 0.0f, 0.0f});
}

TEST_CASE("Object getParent", "[Object]") {
	std::weak_ptr<Object> null_object;
	Object object(std::array<float, 3>{1.0f, 2.0f, 3.0f}, std::array<float, 3>{0.0f, 0.0f, 0.0f}, null_object);
	REQUIRE(object.getParent().lock() == null_object.lock());
}

TEST_CASE("Object setPosition", "Object") {
	std::weak_ptr<Object> null_object;
	Object object(std::array<float, 3>{1.0f, 2.0f, 3.0f}, std::array<float, 3>{0.0f, 0.0f, 0.0f}, null_object);
	object.setPosition(std::array<float, 3>{4.0f, 5.0f, 6.0f});
	REQUIRE(object.getPosition() == std::array<float, 3>{4.0f, 5.0f, 6.0f});
}

TEST_CASE("Object setRotation", "Object") {
	std::weak_ptr<Object> null_object;
	Object object(std::array<float, 3>{1.0f, 2.0f, 3.0f}, std::array<float, 3>{0.0f, 0.0f, 0.0f}, null_object);
	object.setRotation(std::array<float, 3>{4.0f, 5.0f, 6.0f});
	REQUIRE(object.getRotation() == std::array<float, 3>{4.0f, 5.0f, 6.0f});
}

TEST_CASE("Object setParent", "Object") {
	std::shared_ptr<Object> sharedPtr = std::make_shared<Object>(
		std::array<float, 3>{1.0f, 2.0f, 3.0f}, std::array<float, 3>{0.0f, 0.0f, 0.0f}, std::weak_ptr<Object>{});
	std::weak_ptr<Object> weakPtr = sharedPtr;
	Object objectTested(std::array<float, 3>{1.0f, 2.0f, 3.0f}, std::array<float, 3>{0.0f, 0.0f, 0.0f},
						std::weak_ptr<Object>{});
	objectTested.setParent(weakPtr);
	REQUIRE(objectTested.getParent().lock() == sharedPtr);
}