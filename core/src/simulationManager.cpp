#include "simulationManager.h"
#include "utils.h"

SimulationManager::SimulationManager(std::function<void(std::shared_ptr<Object>)> spawnFunction,
									 std::function<void(std::shared_ptr<Object>)> deleteFunction)
	: spawnFunction_(spawnFunction), deleteFunction_(deleteFunction) {
	createObjects();
	autoencoder = std::make_shared<Autoencoder>(8, 2, 8);
	optimizer =
		std::make_unique<torch::optim::Adam>(autoencoder->parameters(), torch::optim::AdamOptions(learningRate));
}

void SimulationManager::createObjects() {
	spawn(std::make_shared<Food>(std::array<float, 3>{generateRandomNumber(50, 950), 1, generateRandomNumber(50, 950)}, std::array<float, 3>{0, 0, 0},
										   std::weak_ptr<Object>(), 1.0, 1.0, 1.0));
	spawn(std::make_shared<Food>(std::array<float, 3>{generateRandomNumber(50, 950), 1, generateRandomNumber(50, 950)}, std::array<float, 3>{0, 0, 0},
										   std::weak_ptr<Object>(), 1.0, 1.0, 1.0));
	spawnNewOrganism();
	spawnNewOrganism();
}

void SimulationManager::spawnNewOrganism() {
	std::vector<std::unique_ptr<Limb>> limbs;
	float limbLength = generateRandomNumber(0.3, 2);
	float childLimbLength = generateRandomNumber(0.3, 2);
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   childLimbLength, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{-1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   childLimbLength, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   childLimbLength, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{-1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   childLimbLength, 0.15f)));

	std::shared_ptr<Organism> test_organism =
		std::make_shared<Organism>(std::array<float, 3>{generateRandomNumber(50, 950), limbLength + childLimbLength + 1, generateRandomNumber(50, 950)}, std::array<float, 3>{0, 0, 0},
								   std::weak_ptr<Object>(), std::move(limbs), std::vector<std::unique_ptr<Organ>>{});
	spawn(test_organism);
}

void SimulationManager::manage() {
	train();
	for (auto& organism : organisms) {
		if (organism->getPosition()[1] < 0) {
			deleteOnDie(organism);
		}
	}
	if (foods.size() < 2) {
	 // stwórz sobie jedzonko wladku
	}
	if (organisms.size() < 2) {
		spawnNewOrganism();
	}
}

void SimulationManager::spawn(std::shared_ptr<Organism> organism) {
	organisms.push_back(organism);
	spawnFunction_(organism);
}

void SimulationManager::spawn(std::shared_ptr<Food> food) {
	foods.push_back(food);
	spawnFunction_(food);
}

void SimulationManager::train() {
	auto dataset = createOrganismDataset();
	auto output = autoencoder->forward(dataset);
	auto loss = torch::nn::functional::mse_loss(output, dataset);
	optimizer->zero_grad();
	loss.backward();
	optimizer->step();
}

void SimulationManager::deleteOnDie(std::shared_ptr<Organism> organismToRemove) {
	deleteFunction_(organismToRemove);
	organisms.erase(
		std::remove_if(organisms.begin(), organisms.end(),
					   [organismToRemove](std::shared_ptr<Organism> organism) { return organism == organismToRemove; }),
		organisms.end());
}

torch::Tensor SimulationManager::createOrganismDataset() {
	std::vector<torch::Tensor> data;
	for (auto& organism : organisms) {
		std::vector<float> lengths;
		auto& children = organism->getChildren();
		for (const auto& limb : children) {
			lengths.push_back(limb->getLength());
		}
		data.push_back(torch::tensor(lengths));
	}
	return torch::stack(data);
}