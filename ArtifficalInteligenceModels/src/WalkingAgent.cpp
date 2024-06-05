#include "WalkingAgent.h"
#include <torch/torch.h>
#include <cmath>

WalkingAgent::WalkingAgent(int64_t state_dim, int64_t action_dim, double lr, double gamma, double eps_clip,
						   int K_epochs)
	: policy(state_dim, action_dim),
	  optimizer(policy.parameters(), torch::optim::AdamOptions(lr)),
	  gamma(gamma),
	  eps_clip(eps_clip),
	  K_epochs(K_epochs) {}

std::pair<torch::Tensor, torch::Tensor> WalkingAgent::select_action(torch::Tensor state) {
	state = state.unsqueeze(0);
	torch::Tensor probs = policy.forward(state);

	// Manual Normal Distribution
	torch::Tensor mean = probs;
	torch::Tensor std = torch::ones_like(mean);
	torch::Tensor action = mean + std * torch::randn_like(mean);

	torch::Tensor log_prob = -((action - mean).pow(2) / (2 * std.pow(2))) - std.log() - std::sqrt(2 * M_PI);
	return {action.squeeze(0), log_prob.sum(-1)};
}

std::vector<double> WalkingAgent::compute_returns(const std::vector<double>& rewards, const std::vector<bool>& dones) {
	std::vector<double> returns;
	double Gt = 0.0;
	for (int i = rewards.size() - 1; i >= 0; --i) {
		if (dones[i])
			Gt = 0.0;
		Gt = rewards[i] + gamma * Gt;
		returns.insert(returns.begin(), Gt);
	}
	return returns;
}

void WalkingAgent::update(const std::vector<torch::Tensor>& states, const std::vector<torch::Tensor>& actions,
						  const std::vector<torch::Tensor>& log_probs, const std::vector<double>& returns) {
	for (int i = 0; i < K_epochs; ++i) {
		torch::Tensor states_tensor = torch::stack(states);
		torch::Tensor actions_tensor = torch::stack(actions);
		torch::Tensor old_log_probs_tensor = torch::stack(log_probs);
		torch::Tensor returns_tensor = torch::tensor(returns);

		torch::Tensor probs = policy.forward(states_tensor);

		// Manual Normal Distribution
		torch::Tensor mean = probs;
		torch::Tensor std = torch::ones_like(mean);
		torch::Tensor new_log_probs =
			-((actions_tensor - mean).pow(2) / (2 * std.pow(2))) - std.log() - std::sqrt(2 * M_PI);
		new_log_probs = new_log_probs.sum(-1);

		torch::Tensor ratios = torch::exp(new_log_probs - old_log_probs_tensor);

		torch::Tensor advantages = returns_tensor - returns_tensor.mean();
		torch::Tensor surr1 = ratios * advantages;
		torch::Tensor surr2 = torch::clamp(ratios, 1 - eps_clip, 1 + eps_clip) * advantages;
		torch::Tensor loss = -torch::min(surr1, surr2).mean();

		optimizer.zero_grad();
		loss.backward();
		optimizer.step();
	}
}
