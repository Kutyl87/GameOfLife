#include "limb.h"

float Limb::getLength() const {
	return length;
}

void Limb::setLength(float length) {
	this->length = length;
}

float Limb::getWidth() const {
	return width;
}

void Limb::setWidth(float width) {
	this->width = width;
}

std::shared_ptr<Limb> Limb::getChildLimb() const {
	return childLimb;
}

void Limb::setChildLimb(std::shared_ptr<Limb> childLimb) {
	this->childLimb = childLimb;
}

std::weak_ptr<Object> Limb::getParentObject() const {
	return parentObject;
}

void Limb::setParentObject(std::weak_ptr<Object> parentObject) {
	this->parentObject = parentObject;
}