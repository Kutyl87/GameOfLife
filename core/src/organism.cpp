#include "organism.h"
#include <random>
#include <iomanip>

Organism::Organism(std::array<float, 3> position, std::array<float, 3> rotation, std::weak_ptr<Object> parent,
                   std::vector<std::unique_ptr<Limb>> children, std::vector<std::unique_ptr<Organ>> organs)
    : Object(position, rotation, parent), children(std::move(children)), organs(std::move(organs)), device(torch::cuda::is_available() ? torch::kCUDA : torch::kCPU) {

    actor = std::make_shared<Actor>(18, 8);
    critic = std::make_shared<Critic>(18, 1);
    auto criticOptimizerOptions = torch::optim::AdamOptions(learning_rate);
    criticOptimizerOptions.weight_decay(0.01);
    auto actorOptimizerOptions = torch::optim::AdamOptions(learning_rate);
    actorOptimizerOptions.weight_decay(0.01);
    critic_optimizer =
        std::make_unique<torch::optim::Adam>(critic->parameters(), criticOptimizerOptions);
    actor_optimizer =
        std::make_unique<torch::optim::Adam>(actor->parameters(), actorOptimizerOptions);

    // Move the models to the device
    actor->to(device);
    critic->to(device);

    // Initialize tensors on the correct device
    actions = torch::empty({0}).to(device);
    rewards = torch::empty({0}).to(device);
    states = torch::empty({0}).to(device);
    direction = {0, 1};
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
    torch::Tensor reward_tensor = torch::tensor({reward}, torch::dtype(torch::kFloat32)).to(device);
    rewards = torch::cat({rewards, reward_tensor});
    auto state = getState();
    torch::Tensor state_tensor = torch::from_blob(state.data(), {1, static_cast<long>(state.size())}, torch::kFloat32).to(device);
    states = torch::cat({states, state_tensor});
    auto action = predict();
    torch::Tensor action_tensor = torch::from_blob(action.data(), {1, static_cast<long>(action.size())}, torch::kFloat32).to(device);
    actions = torch::cat({actions, action_tensor});
    positions.push_back({getPosition()[0], getPosition()[2]});
    ++step;
    if (step > maxSteps) {
        update();
        step = 0;
        actions = torch::empty({0}).to(device);
        rewards = torch::empty({0}).to(device);
        states = torch::empty({0}).to(device);
        if(generateRandomNumber() < -0.99)
        	direction = getDirection();
    }
    return action;
}

void Organism::update() {
    torch::Tensor discounted_rewards = torch::empty(rewards.size(0)).to(device);
    double cumulative = 0;
    double gamma = 0.99;
    for (int64_t i = rewards.size(0) - 1; i >= 0; --i) {
        cumulative = rewards[i].item<double>() + gamma * cumulative;
        discounted_rewards[i] = cumulative;
    }
    
    std::cerr << "Discounted Rewards Dimensions: " << discounted_rewards.sizes() << std::endl;

    discounted_rewards = discounted_rewards.unsqueeze(1); // Make it [21, 1]
    std::cerr << "Discounted Rewards Reshaped Dimensions: " << discounted_rewards.sizes() << std::endl;
    
    auto values = critic->forward(states);
    std::cerr << "Values Dimensions: " << values.sizes() << std::endl;
    
    auto critic_loss = torch::nn::functional::mse_loss(values, discounted_rewards);
    std::cerr << "Critic Loss Dimensions: " << critic_loss.sizes() << std::endl;
    
    auto advantages = discounted_rewards - values;
    std::cerr << "Advantages Dimensions: " << advantages.sizes() << std::endl;
    
    auto actor_output = actor->forward(states);
    std::cerr << "Actor Output Dimensions: " << actor_output.sizes() << std::endl;
    
    auto actor_loss = torch::nn::functional::mse_loss(
        actor_output, actions, torch::nn::functional::MSELossFuncOptions().reduction(torch::kNone));
    std::cerr << "Actor Loss Before Mean Dimensions: " << actor_loss.sizes() << std::endl;
    
    actor_loss = actor_loss.mean(1);
    std::cerr << "Actor Loss After Mean Dimensions: " << actor_loss.sizes() << std::endl;
    
    actor_loss = actor_loss * advantages.squeeze(1);
    std::cerr << "Actor Loss After Multiplying with Advantages Dimensions: " << actor_loss.sizes() << std::endl;
    
    actor_loss = actor_loss.mean();
    std::cerr << "Actor Loss After Final Mean Dimensions: " << actor_loss.sizes() << std::endl;

    critic_optimizer->zero_grad();
    actor_optimizer->zero_grad();
    
    critic_loss.backward({}, true);
    actor_loss.backward();
    
    critic_optimizer->step();
    actor_optimizer->step();
}

std::array<float, 8> Organism::predict() {
    auto state = getState();
    torch::Tensor state_tensor = torch::from_blob(state.data(), {1, static_cast<long>(state.size())}, torch::kFloat32).to(device);
    torch::Tensor action_tensor = actor->forward(state_tensor);
    std::array<float, 8> action;
    std::memcpy(action.data(), action_tensor.data_ptr<float>(), sizeof(float) * action_tensor.numel());
    for(auto& force : action) {
        if(generateRandomNumber() > 0.5) {
            force = generateRandomNumber();
        }
    }
    // for(int i = 0; i < 8; ++i) {
    //     action[i] = -state[i];
    // }
    std::cerr << "forces: ";
    for(auto& force : action) {
        std::cerr << std::fixed << std::setprecision(4) << std::setw(7) << force << ' ';
    }
    std::cerr << '\n';

    return action;
}

float Organism::getReward(float dirX, float dirZ) {
    if (positions.empty()) {
        return 0;
    }

    // Get the last and current positions
    std::array<float, 2> lastPos = positions.back();
    std::array<float, 2> currentPos = {getPosition()[0], getPosition()[2]};
    std::array<float, 2> positionDifference = {currentPos[0] - lastPos[0], currentPos[1] - lastPos[1]};

    // Calculate the original reward based on direction
    float originalReward = dotProduct(positionDifference, direction);

    // Get the current state and convert it to a tensor
    std::array<float, 18> currentStateArray = getState();
    torch::Tensor currentState = torch::from_blob(currentStateArray.data(), {18}, torch::kFloat32).to(device);

    // If there are no previous states, return the original reward
    if (states.size(0) == 0) {
        return originalReward;
    }
    
    // Get the latest state from the states tensor
    torch::Tensor latestState = states[-1];

    // Subtract the mean difference from the original reward
    return -(
        5*originalReward + 0*getPosition()[1] - 20*latestState.index({torch::indexing::Slice(0, 8)}).dot(actions[-1]).item<float>() + 50 * torch::mean(torch::square(actions[-1])).item<float>()
    );
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
    // for(auto s : state) {
    //     std::cerr << s << ' ';
    // }
    // std::cerr << '\n';
    return state;
}
