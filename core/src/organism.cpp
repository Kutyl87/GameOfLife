#include "organism.h"

Organism::Organism() {
	this->children = std::vector<std::unique_ptr<Limb>>();
	this->organs = std::vector<std::unique_ptr<Organ>>();
}

void Organism::addChild(std::unique_ptr<Limb> child) {
	children.push_back(std::move(child));
}

void Organism::addOrgan(std::unique_ptr<Organ> organ) {
	organs.push_back(std::move(organ));
}

void Organism::removeChildByIndex(int index) {
	children.erase(children.begin() + index);
}

void Organism::removeOrganByIndex(int index) {
	organs.erase(organs.begin() + index);
}

std::vector<std::unique_ptr<Limb>>& Organism::getChildren() {
	return children;
}

std::vector<std::unique_ptr<Organ>>& Organism::getOrgans() {
	return organs;
}