#pragma once

#include <memory>
#include "object.h"

class Limb : public Object {
public:
	Limb(std::array<float, 3> position, std::array<float, 3> rotation, std::weak_ptr<Object> parent, float length,
		 float width, std::shared_ptr<Limb> childLimb = std::shared_ptr<Limb>())
		: Object(position, rotation, parent),
		  length(length),
		  width(width),
		  childLimb(childLimb),
		  parentObject(parent) {}

	float getLength() const;
	void setLength(float length);

	float getWidth() const;
	void setWidth(float width);

	std::shared_ptr<Limb> getChildLimb() const;
	void setChildLimb(std::shared_ptr<Limb> childLimb);

	std::weak_ptr<Object> getParentObject() const;
	void setParentObject(std::weak_ptr<Object> parentObject);

private:
	float length;
	float width;
	std::shared_ptr<Limb> childLimb;
	std::weak_ptr<Object> parentObject;
};