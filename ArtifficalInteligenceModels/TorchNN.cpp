#include <torch/torch.h>

struct Net : torch::nn::Module {
  Net() {
    fc1 = register_module("fc1", torch::nn::Linear(2, 10));
    fc2 = register_module("fc2", torch::nn::Linear(10, 2));
    fc3 = register_module("fc3", torch::nn::Linear(2, 1));
  }

  torch::Tensor forward(torch::Tensor x) {
    x = torch::relu(fc1->forward(x));
    x = torch::relu(fc2->forward(x));
    x = torch::sigmoid(fc3->forward(x));
    return x;
  }

  torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};
};

void train(std::shared_ptr<Net> net, torch::Tensor inputs, torch::Tensor targets, size_t num_epochs) {
  torch::optim::Adam optimizer(net->parameters(), torch::optim::AdamOptions(0.01));

  for (size_t epoch = 1; epoch <= num_epochs; ++epoch) {
    optimizer.zero_grad();
    torch::Tensor output = net->forward(inputs);
    torch::Tensor loss = torch::binary_cross_entropy(output, targets);
    loss.backward();
    optimizer.step();
    auto prediction = output.detach().round();
    double accuracy = torch::sum(prediction.eq(targets)).item<double>() / targets.numel();
    if (epoch % 500 == 0) {
      std::cout << "Epoch: " << epoch << " | Loss: " << loss.item<float>() << " | Accuracy: " << accuracy << std::endl;
    }
  }
}
void save(std::shared_ptr<Net> net, const std::string& path) {
  torch::save(net, path);
}

int main() {
  auto net = std::make_shared<Net>();
  torch::Tensor inputs = torch::tensor({{0,0}, {0,1}, {1,0}, {1,1}}).to(torch::kFloat);
  torch::Tensor targets = torch::tensor({{0}, {1}, {1}, {0}}).to(torch::kFloat);
  train(net, inputs, targets, 5000);
}