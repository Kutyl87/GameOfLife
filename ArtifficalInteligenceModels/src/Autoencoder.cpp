#include "Autoencoder.h"

Autoencoder::Autoencoder(int64_t input_size, int64_t hidden_size, int64_t output_size)
	: encoder(std::make_shared<Encoder>(input_size, hidden_size)),
	  decoder(std::make_shared<Decoder>(hidden_size, output_size)) {
	register_module("encoder", encoder);
	register_module("decoder", decoder);
}

torch::Tensor Autoencoder::forward(torch::Tensor x) {
	x = encoder->forward(x);
	x = decoder->forward(x);
	return x;
}

torch::Tensor Autoencoder::encode(torch::Tensor x) {
	return encoder->forward(x);
}