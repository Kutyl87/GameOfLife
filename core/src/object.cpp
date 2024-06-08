#include "object.h"

std::array<float, 3> Object::getPosition() const {
	return position;
}

std::array<float, 3> Object::getRotation() const {
	return rotation;
}

std::weak_ptr<Object> Object::getParent() const {
	return parent;
}

void Object::setPosition(std::array<float, 3> position) {
	this->position = position;
}

void Object::setRotation(std::array<float, 3> rotation) {
	this->rotation = rotation;
}

void Object::setParent(std::weak_ptr<Object> parent) {
	this->parent = parent;
}

bool Object::operator==(const Object& other) const {
	return position == other.position && rotation == other.rotation && parent.lock() == other.parent.lock();
}

Object::~Object() {}