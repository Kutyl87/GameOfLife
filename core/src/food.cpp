#include "food.h"

float Food::getSize() const {
	return size;
}

float Food::getValuePerUnit() const {
	return valuePerUnit;
}

float Food::getToxinLevel() const {
	return toxinLevel;
}

void Food::setSize(float size) {
	this->size = size;
}

void Food::setValuePerUnit(float valuePerUnit) {
	this->valuePerUnit = valuePerUnit;
}

void Food::setToxinLevel(float toxinLevel) {
	this->toxinLevel = toxinLevel;
}