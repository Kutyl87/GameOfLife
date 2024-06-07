#include "simulationManager.h"

SimulationManager::SimulationManager(std::vector<std::shared_ptr<Food>> food,
									 std::vector<std::shared_ptr<Organism>> organisms,
									 std::shared_ptr<Autoencoder> autoencoder,
									 std::function<void(std::shared_ptr<Object>)> spawnFunction,
									 std::function<void(std::shared_ptr<Object>)> deleteFunction)
	: foods(std::move(food)),
	  organisms(std::move(organisms)),
	  autoencoder(std::move(autoencoder)),
	  spawnFunction_(spawnFunction),
	  deleteFunction_(deleteFunction) {
	optimizer =
		std::make_unique<torch::optim::Adam>(autoencoder->parameters(), torch::optim::AdamOptions(learningRate));
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