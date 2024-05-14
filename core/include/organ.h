
class Organ {
public:
	Organ(float agingRate, float health) : aging_rate(agingRate), health(health) {}
	float getAgingRate();
	float getHealth();
	float getDamage(float damage);
	float regenerateHealth(float health);
	~Organ();

private:
	float agingRate;
	float health;
};