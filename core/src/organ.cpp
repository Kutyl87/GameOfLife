#include "organ.h"

float Organ::getAgingRate() {
	return agingRate;
}

float Organ::getHealth() {
	return health;
}

void Organ::takeDamage(float damage) {
	health -= damage;
}

void Organ::regenerateHealth(float health) {
	this->health += health;
}