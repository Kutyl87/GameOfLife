#include "Encoder.h"

Encoder::Encoder(int64_t input_size, int64_t hidden_size) {
	fc1 = register_module("fc1", torch::nn::Linear(input_size, hidden_size));
	fc2 = register_module("fc2", torch::nn::Linear(hidden_size, hidden_size));
}

torch::Tensor Encoder::forward(torch::Tensor x) {
	x = torch::relu(fc1->forward(x));
	x = torch::relu(fc2->forward(x));
	return x;
}