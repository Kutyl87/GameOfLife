#include <torch/torch.h>
#include <functional>
#include <memory>
#include <vector>
#include "Autoencoder.h"
#include "food.h"
#include "object.h"
#include "organism.h"

class SimulationManager {
public:
	SimulationManager(std::function<void(std::shared_ptr<Object> organism)> spawnFunction,
					  std::function<void(std::shared_ptr<Object> object)> deleteFunction);
	void spawn(std::shared_ptr<Organism> organism);
	void spawn(std::shared_ptr<Food> food);
	void train();
	void deleteOnDie(std::shared_ptr<Organism> organism);
	void createObjects();

private:
	std::vector<std::shared_ptr<Food>> foods;
	std::vector<std::shared_ptr<Organism>> organisms;
	std::shared_ptr<Autoencoder> autoencoder;
	std::function<void(std::shared_ptr<Object> object)> spawnFunction_;
	std::function<void(std::shared_ptr<Object> object)> deleteFunction_;
	torch::Tensor createOrganismDataset();
	std::unique_ptr<torch::optim::Optimizer> optimizer;
	const int learningRate = 0.001;
};