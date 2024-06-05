#pragma once

#include <torch/torch.h>
#include <vector>
#include "LocomotionNetwork.h"

class WalkingAgent {
public:
	WalkingAgent(int64_t state_dim, int64_t action_dim, double lr = 3e-4, double gamma = 0.99, double eps_clip = 0.2,
				 int K_epochs = 4);

	std::pair<torch::Tensor, torch::Tensor> select_action(torch::Tensor state);
	std::vector<double> compute_returns(const std::vector<double>& rewards, const std::vector<bool>& dones);
	void update(const std::vector<torch::Tensor>& states, const std::vector<torch::Tensor>& actions,
				const std::vector<torch::Tensor>& log_probs, const std::vector<double>& returns);

private:
	LocomotionNetwork policy;
	torch::optim::Adam optimizer;
	double gamma;
	double eps_clip;
	int K_epochs;
};