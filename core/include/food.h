#pragma once
#include "object.h"

class Food : public Object {
public:
	Food(std::array<float, 3> position, std::array<float, 3> rotation, std::weak_ptr<Object> parent, float size,
		 float valuePerUnit, float toxinLevel)
		: Object(position, rotation, parent), size(size), valuePerUnit(valuePerUnit), toxinLevel(toxinLevel) {}

	float getSize() const;
	float getValuePerUnit() const;
	float getToxinLevel() const;
	void setSize(float size);
	void setValuePerUnit(float valuePerUnit);
	void setToxinLevel(float toxinLevel);

private:
	float size;
	float valuePerUnit;
	float toxinLevel;
};