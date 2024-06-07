#include "Decoder.h"

Decoder::Decoder(int64_t hidden_size, int64_t output_size) {
	fc1 = register_module("fc1", torch::nn::Linear(hidden_size / 2, hidden_size));
	fc2 = register_module("fc2", torch::nn::Linear(hidden_size, output_size));
}

torch::Tensor Decoder::forward(torch::Tensor x) {
	x = torch::relu(fc1->forward(x));
	x = torch::sigmoid(fc2->forward(x));
	return x;
}