#include "simulationManager.h"

SimulationManager::SimulationManager(std::function<void(std::shared_ptr<Object>)> spawnFunction,
									 std::function<void(std::shared_ptr<Object>)> deleteFunction)
	: spawnFunction_(spawnFunction), deleteFunction_(deleteFunction) {
	createObjects();
	optimizer =
		std::make_unique<torch::optim::Adam>(autoencoder->parameters(), torch::optim::AdamOptions(learningRate));
}

void SimulationManager::createObjects() {
	foods.push_back(std::make_shared<Food>(std::array<float, 3>{0, 0, 0}, std::array<float, 3>{0, 0, 0},
										   std::weak_ptr<Object>(), 1.0, 1.0, 1.0));
	foods.push_back(std::make_shared<Food>(std::array<float, 3>{1, 1, 1}, std::array<float, 3>{0, 0, 0},
										   std::weak_ptr<Object>(), 1.0, 1.0, 1.0));
	std::vector<std::unique_ptr<Limb>> limbs;
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{-1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));
	limbs.emplace_back(std::make_unique<Limb>(
		std::array<float, 3>{-1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), 1.0f, 0.2f,
		std::make_unique<Limb>(std::array<float, 3>{0, -1, 0}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(),
							   1.f, 0.15f)));

	std::shared_ptr<Organism> test_organism =
		std::make_shared<Organism>(std::array<float, 3>{50, 20, 50}, std::array<float, 3>{0, 0, 0},
								   std::weak_ptr<Object>(), std::move(limbs), std::vector<std::unique_ptr<Organ>>{});
	organisms.push_back(test_organism);
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