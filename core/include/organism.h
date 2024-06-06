#pragma once

#include <memory>
#include <vector>
#include "limb.h"
#include "object.h"
#include "organ.h"

class Organism : public Object {
public:
	Organism(std::array<float, 3> position, std::array<float, 3> rotation, std::weak_ptr<Object> parent,
			 std::vector<std::unique_ptr<Limb>> children = {}, std::vector<std::unique_ptr<Organ>> organs = {});

	void addChild(std::unique_ptr<Limb> child);
	void addOrgan(std::unique_ptr<Organ> organ);
	void removeChildByIndex(int index);
	void removeOrganByIndex(int index);
	std::vector<std::unique_ptr<Limb>>& getChildren();
	std::vector<std::unique_ptr<Organ>>& getOrgans();
	std::array<float, 2> getDirection() const;
	std::array<float, 8> getJointForces();
	float getReward(float dirX, float dirZ);
	std::array<float, 18> getState();

private:
	std::array<float, 8> predict();
	std::vector<std::unique_ptr<Limb>> children;
	std::vector<std::unique_ptr<Organ>> organs;
	std::vector<std::array<float, 8>> actions;
	std::vector<float> rewards;
	std::vector<std::array<float, 2>> positions;
	std::vector<std::array<float, 18>> states;
	std::array<float, 2> direction;
	static float generateRandomNumber();
	static float dotProduct(const std::array<float, 2>& a, const std::array<float, 2>& b);
};