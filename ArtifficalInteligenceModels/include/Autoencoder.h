#pragma once

#include <torch/torch.h>
#include "Encoder.h"
#include "Decoder.h"

class Autoencoder : public torch::nn::Module {
public:
	Autoencoder(int64_t input_size, int64_t hidden_size, int64_t output_size);

	torch::Tensor forward(torch::Tensor x);
	torch::Tensor encode(torch::Tensor x);

private:
	torch::nn::ModuleHolder<Encoder> encoder;
	torch::nn::ModuleHolder<Decoder> decoder;
};