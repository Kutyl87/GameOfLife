#pragma once

#include <torch/torch.h>

class Decoder : public torch::nn::Module {
public:
	Decoder(int64_t hidden_size, int64_t output_size);

	torch::Tensor forward(torch::Tensor x);

private:
	torch::nn::Linear fc1{nullptr}, fc2{nullptr};
};