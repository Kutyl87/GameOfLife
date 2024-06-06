#pragma once

class Food {
public:
	Food(float size, float valuePerUnit, float toxinLevel)
		: size(size), valuePerUnit(valuePerUnit), toxinLevel(toxinLevel) {}

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