#pragma once

#include <torch/torch.h>

class Critic : public torch::nn::Module {
public:
	Critic(int64_t state_dim, int64_t action_dim);

	torch::Tensor forward(torch::Tensor x);
	torch::Tensor predict(torch::Tensor x);

private:
	torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};
};