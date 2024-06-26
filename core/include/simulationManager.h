#include <torch/torch.h>
#include <functional>
#include <memory>
#include <vector>
#include "Autoencoder.h"
#include "food.h"
#include "object.h"
#include "organism.h"
#include "utils.h"

class SimulationManager {
public:
	SimulationManager(std::function<void(std::shared_ptr<Object> organism)> spawnFunction,
					  std::function<void(std::shared_ptr<Object> object)> deleteFunction);
	void spawn(std::shared_ptr<Organism> organism);
	void spawn(std::shared_ptr<Food> food);
	void train();
	void deleteOnDie(std::shared_ptr<Organism> organism);
	void deleteFood(std::shared_ptr<Food> food);
	void createObjects();
	void manage();
	std::vector<std::pair<Organism&, Organism&>> getPairs();
	std::vector<torch::Tensor> createLatentSpaceVector();
	std::multimap<float, std::pair<Organism&, Organism&>, std::greater<>> createDistanceMap(
		const std::vector<torch::Tensor>& latentSpaceVec);
	std::vector<std::pair<Organism&, Organism&>> createPairs(
		const std::multimap<float, std::pair<Organism&, Organism&>, std::greater<>>& distanceMap);
	float crossover(float minLength, float maxLength);
	float mutate(float minLength, float maxLength);
	std::shared_ptr<Organism> breed(std::pair<Organism&, Organism&>);

private:
	void spawnNewOrganism();
	void spawnNewFood();
	std::vector<std::shared_ptr<Food>> foods;
	std::vector<std::shared_ptr<Organism>> organisms;
	std::shared_ptr<Autoencoder> autoencoder;
	std::function<void(std::shared_ptr<Object> object)> spawnFunction_;
	std::function<void(std::shared_ptr<Object> object)> deleteFunction_;
	torch::Tensor createOrganismDataset();
	std::unique_ptr<torch::optim::Optimizer> optimizer;
	const float learningRate = 0.001;
};