class Food {
public:
	Food(float size, float valuePerUnit, float toxinLevel)
		: size(size), valuePerUnit(valuePerUnit), toxinLevel(toxinLevel) {}

	float getSize() const;
	float getValuePerUnit() const;

private:
	float size;
	float valuePerUnit;
	float toxinLevel;
};