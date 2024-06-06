#pragma once

#include <memory>
#include <vector>
#include "Actor.h"
#include "Critic.h"
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
	void update();

private:
	std::shared_ptr<Actor> actor;
	std::shared_ptr<Critic> critic;
	std::array<float, 8> predict();
	std::vector<std::unique_ptr<Limb>> children;
	std::vector<std::unique_ptr<Organ>> organs;
	torch::Tensor actions;
	torch::Tensor rewards;
	std::vector<std::array<float, 2>> positions;
	torch::Tensor states;
	std::array<float, 2> direction;
	static float generateRandomNumber();
	static float dotProduct(const std::array<float, 2>& a, const std::array<float, 2>& b);
	const int maxSteps = 20;
	int step = 0;
	std::unique_ptr<torch::optim::Optimizer> critic_optimizer;
	std::unique_ptr<torch::optim::Optimizer> actor_optimizer;
	float learning_rate = 0.001;
};