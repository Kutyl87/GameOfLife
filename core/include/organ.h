
class Organ {
public:
	Organ(float agingRate, float health) : agingRate(agingRate), health(health) {}

	float getAgingRate();
	float getHealth();
	void getDamage(float damage);
	void regenerateHealth(float health);

private:
	float agingRate;
	float health;
};