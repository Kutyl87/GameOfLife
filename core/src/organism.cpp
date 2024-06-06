#include "organism.h"
#include <random>

Organism::Organism(std::array<float, 3> position, std::array<float, 3> rotation, std::weak_ptr<Object> parent,
				   std::vector<std::unique_ptr<Limb>> children, std::vector<std::unique_ptr<Organ>> organs)
	: Object(position, rotation, parent) {
	this->children = std::vector<std::unique_ptr<Limb>>();
	this->organs = std::vector<std::unique_ptr<Organ>>();
	critic_optimizer =
		std::make_unique<torch::optim::Adam>(critic->parameters(), torch::optim::AdamOptions(learning_rate));
	actor_optimizer =
		std::make_unique<torch::optim::Adam>(actor->parameters(), torch::optim::AdamOptions(learning_rate));
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
	torch::Tensor reward_tensor = torch::tensor({reward});
	rewards = torch::cat({rewards, reward_tensor});
	auto state = getState();
	torch::Tensor state_tensor = torch::from_blob(state.data(), {1, state.size()});
	actions = torch::cat({states, state_tensor});
	auto action = predict();
	torch::Tensor action_tensor = torch::from_blob(action.data(), {1, action.size()});
	actions = torch::cat({actions, action_tensor});
	positions.push_back({getPosition()[0], getPosition()[2]});
	++step;
	if (step > maxSteps) {
		update();
		step = 0;
		actions = torch::empty({0});
		rewards = torch::empty({0});
		states = torch::empty({0});
	}
	return action;
}

void Organism::update() {
	torch::Tensor discounted_rewards = torch::empty(rewards.size(0));
	double cumulative = 0;
	double gamma = 0.99;
	for (int64_t i = rewards.size(0) - 1; i >= 0; --i) {
		cumulative = rewards[i].item<double>() + gamma * cumulative;
		discounted_rewards[i] = cumulative;
	}
	auto critic_loss = torch::nn::functional::mse_loss(critic->forward(states), discounted_rewards);
	auto values = critic->forward(states);
	auto advantages = discounted_rewards - values;
	auto actor_loss = torch::nn::functional::mse_loss(
		actor->forward(states), actions, torch::nn::functional::MSELossFuncOptions().reduction(torch::kNone));
	actor_loss = actor_loss * advantages;
	actor_loss = torch::mean(actor_loss);
	critic_loss.backward();
	actor_loss.backward();
	critic_optimizer->step();
	actor_optimizer->step();
}

std::array<float, 8> Organism::predict() {
	auto state = getState();
	torch::Tensor state_tensor = torch::from_blob(state.data(), {1, state.size()});
	torch::Tensor action_tensor = actor->forward(state_tensor);
	std::array<float, 8> action;
	std::memcpy(action.data(), action_tensor.data_ptr<float>(), sizeof(float) * action_tensor.numel());
	return action;
}

float Organism::getReward(float dirX, float dirZ) {
	if (positions.size() == 0) {
		return 0;
	}
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