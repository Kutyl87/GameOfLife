#include "simulationManager.h"
#include "utils.h"

SimulationManager::SimulationManager(std::function<void(std::shared_ptr<Object>)> spawnFunction,
                                     std::function<void(std::shared_ptr<Object>)> deleteFunction)
    : spawnFunction_(spawnFunction), deleteFunction_(deleteFunction) {
    createObjects();
    autoencoder = std::make_shared<Autoencoder>(8, 2, 8);
    optimizer = std::make_unique<torch::optim::Adam>(autoencoder->parameters(), torch::optim::AdamOptions(learningRate));
}

void SimulationManager::createObjects() {
    for (int i = 0; i < 1000; ++i) {
        spawnNewFood();
    }
    for (int i = 0; i < 20; ++i) {
        spawnNewOrganism();
    }
}

void SimulationManager::spawnNewOrganism() {
    std::vector<std::unique_ptr<Limb>> limbs;
    float limbLength = generateRandomNumber(0.3, 2);
    float childLimbLength = generateRandomNumber(0.3, 2);
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), childLimbLength, 0.15f)));
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{-1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), childLimbLength, 0.15f)));
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), childLimbLength, 0.15f)));
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{-1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), childLimbLength, 0.15f)));

    std::shared_ptr<Organism> test_organism = std::make_shared<Organism>(
        std::array<float, 3>{generateRandomNumber(15, 285), limbLength + childLimbLength + 1, generateRandomNumber(15, 285)},
        std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), std::move(limbs), std::vector<std::unique_ptr<Organ>>{});
    spawn(test_organism);
}

void SimulationManager::spawnNewFood() {
    spawn(std::make_shared<Food>(std::array<float, 3>{generateRandomNumber(15, 285), 0.2, generateRandomNumber(15, 285)},
                                 std::array<float, 3>{0, 0, 0}, std::weak_ptr<Object>(), generateRandomNumber(0.1, 2),
                                 generateRandomNumber(0.1, 2), generateRandomNumber(0.1, 2)));
}

void SimulationManager::manage() {
    train();
    for (int i = 0; i < organisms.size(); ++i) {
        auto& organism = organisms[i];
        organism->food -= 0.01;
        if (organism->breedingCooldown > 0) {
            --organism->breedingCooldown;
        }
        if (organism->getPosition()[1] < 0) {
            deleteOnDie(organism);
            --i;
            continue;
        }
        if (organism->hp < 0) {
            deleteOnDie(organism);
            --i;
            continue;
        }
        if (organism->food < 0) {
            organism->hp += organism->food;
        }
        for (auto food : foods) {
            auto foodPos = food->getPosition();
            auto organismPos = organism->getPosition();
            if (calculateDistance(organismPos, foodPos) < 10) {
                organism->food += food->getSize() * food->getValuePerUnit();
                organism->hp -= food->getSize() * food->getToxinLevel();
                deleteFood(food);
                break;
            }
        }
    }

    std::vector<std::pair<Organism&, Organism&>> pairs = getPairs();
    for (auto& pair : pairs) {
        auto pos1 = pair.first.getPosition();
        auto pos2 = pair.second.getPosition();
        float distanceBetweenOrganisms = calculateDistance(pos1, pos2);
        if (distanceBetweenOrganisms <= 10 && pair.first.breedingCooldown == 0 && pair.second.breedingCooldown == 0) {
            std::shared_ptr<Organism> newOrganism = breed(pair);
            pair.first.breedingCooldown = 1200;
            pair.second.breedingCooldown = 1200;
            spawn(newOrganism);
            continue;
        }
        std::array<float, 2> direction = { pos2[0] - pos1[0], pos2[2] - pos1[2] };

        float magnitude = std::sqrt(direction[0] * direction[0] + direction[1] * direction[1]);
        direction[0] /= magnitude;
        direction[1] /= magnitude;

        float angle = -pair.first.getRotation()[1];
        float cos_angle = std::cos(angle);
        float sin_angle = std::sin(angle);

        std::array<float, 2> direction1 = {
            cos_angle * direction[0] - sin_angle * direction[1],
            sin_angle * direction[0] + cos_angle * direction[1]
        };

        pair.first.setDirection(direction1);

        angle = -pair.second.getRotation()[1];
        cos_angle = std::cos(angle);
        sin_angle = std::sin(angle);

        std::array<float, 2> direction2 = {
            cos_angle * -direction[0] - sin_angle * -direction[1],
            sin_angle * -direction[0] + cos_angle * -direction[1]
        };

        pair.second.setDirection(direction2);
    }
    if (organisms.size() < 5) {
        spawnNewOrganism();
    }
    if (foods.size() < 800) {
        spawnNewFood();
    }
}

float SimulationManager::crossover(float minLength, float maxLength) {
    return generateRandomNumber(minLength, maxLength);
}

float SimulationManager::mutate(float minLength, float maxLength) {
    return generateRandomNumber(0.1 * minLength, 0.1 * maxLength);
}

std::shared_ptr<Organism> SimulationManager::breed(std::pair<Organism&, Organism&> pair) {
    float minLengthParents = std::min(pair.first.getChildren()[0]->getLength(), pair.second.getChildren()[0]->getLength());
    float maxLengthParents = std::max(pair.first.getChildren()[0]->getLength(), pair.second.getChildren()[0]->getLength());
    float crossoverLimbLength = crossover(minLengthParents, maxLengthParents);
    float mutatedLimbLength = mutate(minLengthParents, maxLengthParents);
    float limbLength = crossoverLimbLength + mutatedLimbLength;
    std::vector<std::unique_ptr<Limb>> limbs;
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), limbLength, 0.15f)));
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{-1, 0, 1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), limbLength, 0.15f)));
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), limbLength, 0.15f)));
    limbs.emplace_back(std::make_unique<Limb>(
        std::array<float, 3>{-1, 0, -1.5}, std::array<float, 3>{0, 0, 0}, std::weak_ptr<::Object>(), limbLength, 0.2f,
        std::make_unique<Limb>(std::array<float, 3>{0, -limbLength, 0}, std::array<float, 3>{0, 0, 0},
                               std::weak_ptr<::Object>(), limbLength, 0.15f)));
    std::array<float, 3> midPoint = calculateMidpoint(pair.first.getPosition(), pair.second.getPosition());
    std::shared_ptr<Organism> newOrganism =
        std::make_shared<Organism>(midPoint, std::array<float, 3>{0, 0, 0}, std::shared_ptr<Object>(), std::move(limbs),
                                   std::vector<std::unique_ptr<Organ>>{});
    return newOrganism;
}

std::vector<torch::Tensor> SimulationManager::createLatentSpaceVector() {
    auto dataset = createOrganismDataset();
    torch::Tensor latentSpace = autoencoder->encode(dataset);
    return latentSpace.split(1, 0);
}

std::multimap<float, std::pair<Organism&, Organism&>, std::greater<>> SimulationManager::createDistanceMap(
    const std::vector<torch::Tensor>& latentSpaceVec) {
    std::multimap<float, std::pair<Organism&, Organism&>, std::greater<>> distanceMap;
    for (int i = 0; i < latentSpaceVec.size(); ++i) {
        for (int j = i + 1; j < latentSpaceVec.size(); ++j) {
            float distance = torch::dist(latentSpaceVec[i], latentSpaceVec[j]).item<float>();
            std::pair<float, std::pair<Organism&, Organism&>> pair_to_insert =
                std::make_pair(distance, std::make_pair(std::ref(*organisms[i]), std::ref(*organisms[j])));
            distanceMap.insert(pair_to_insert);
        }
    }
    return distanceMap;
}

std::vector<std::pair<Organism&, Organism&>> SimulationManager::createPairs(
    const std::multimap<float, std::pair<Organism&, Organism&>, std::greater<>>& distanceMap) {
    std::vector<std::pair<Organism&, Organism&>> pairs;
    std::unordered_set<Organism*> pairedOrganisms;
    for (const auto& entry : distanceMap) {
        Organism& org1 = entry.second.first;
        Organism& org2 = entry.second.second;
        if (pairedOrganisms.find(&org1) == pairedOrganisms.end() &&
            pairedOrganisms.find(&org2) == pairedOrganisms.end()) {
            pairs.push_back({org1, org2});
            pairedOrganisms.insert(&org1);
            pairedOrganisms.insert(&org2);
        }
    }
    return pairs;
}

std::vector<std::pair<Organism&, Organism&>> SimulationManager::getPairs() {
    std::vector<torch::Tensor> latentSpaceVec = createLatentSpaceVector();
    std::multimap<float, std::pair<Organism&, Organism&>, std::greater<>> distanceMap = createDistanceMap(latentSpaceVec);
    return createPairs(distanceMap);
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

void SimulationManager::deleteFood(std::shared_ptr<Food> foodToRemove) {
    deleteFunction_(foodToRemove);
    foods.erase(
        std::remove_if(foods.begin(), foods.end(),
                       [foodToRemove](std::shared_ptr<Food> food) { return food == foodToRemove; }),
        foods.end());
}

torch::Tensor SimulationManager::createOrganismDataset() {
    std::vector<torch::Tensor> data;
    for (auto& organism : organisms) {
        std::vector<float> lengths;
        auto& children = organism->getChildren();
        for (const auto& limb : children) {
            lengths.push_back(limb->getLength());
            lengths.push_back(limb->getChildLimb()->getLength());
        }
        data.push_back(torch::tensor(lengths));
    }
    if (data.size() == 0) {
        return torch::Tensor();
    }
    return torch::stack(data);
}
