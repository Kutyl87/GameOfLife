#include "LocomotionNetwork.h"

LocomotionNetwork::LocomotionNetwork(int64_t state_dim, int64_t action_dim) {
	fc1 = register_module("fc1", torch::nn::Linear(state_dim, 512));
	fc2 = register_module("fc2", torch::nn::Linear(512, 256));
	fc3 = register_module("fc3", torch::nn::Linear(256, action_dim));
}

torch::Tensor LocomotionNetwork::forward(torch::Tensor x) {
	x = torch::relu(fc1->forward(x));
	x = torch::relu(fc2->forward(x));
	x = torch::sigmoid(fc3->forward(x));
	return x;
}