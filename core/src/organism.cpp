#include "organism.h"
#include <random>

Organism::Organism(std::array<float, 3> position, std::array<float, 3> rotation, std::weak_ptr<Object> parent,
				   std::vector<std::unique_ptr<Limb>> children, std::vector<std::unique_ptr<Organ>> organs)
	: Object(position, rotation, parent) {
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

float Organism::generateRandomNumber() {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	static std::uniform_real_distribution<> dis(-1.0, 1.0);
	return dis(gen);
}

std::array<float, 2> Organism::getDirection() const {
	std::array<float, 2> direction = {generateRandomNumber(), generateRandomNumber()};
	float magnitude = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
	direction[0] /= magnitude;
	direction[1] /= magnitude;
	return direction;
}

std::array<float, 8> Organism::getJointForces() {
	float reward = getReward(direction[0], direction[1]);
	rewards.push_back(reward);
	auto state = getState();
	auto action = predict();
	states.push_back(state);
	actions.push_back(action);
	rewards.push_back(reward);

	return action;
}

float Organism::getReward(float dirX, float dirZ) {
	std::array<float, 2> lastPos = positions[positions.size() - 1];
	std::array<float, 2> currentPos = {getPosition()[0], getPosition()[2]};
	std::array<float, 2> positionDifference = {currentPos[0] - lastPos[0], currentPos[1] - lastPos[1]};
	return dotProduct(positionDifference, direction);
}

float Organism::dotProduct(const std::array<float, 2>& a, const std::array<float, 2>& b) {
	return a[0] * b[0] + a[1] * b[1];
}

std::array<float, 18> Organism::getState() {
	std::array<float, 18> state;
	for (size_t i = 0; i < 4; i++) {
		state[2 * i] = children[i]->getRotation()[0];
		state[2 * i + 1] = children[i]->getChildLimb()->getRotation()[0];
	}
	for (size_t i = 0; i < 4; i++) {
		state[2 * i + 8] = children[i]->getLength();
		state[2 * i + 9] = children[i]->getChildLimb()->getLength();
	}
	state[state.size() - 2] = direction[0];
	state[state.size() - 1] = direction[1];
	return state;
}