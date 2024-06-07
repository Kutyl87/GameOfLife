#pragma once

#include <torch/torch.h>

class Encoder : public torch::nn::Module {
public:
	Encoder(int64_t input_size, int64_t hidden_size);

	torch::Tensor forward(torch::Tensor x);

private:
	torch::nn::Linear fc1{nullptr}, fc2{nullptr};
};