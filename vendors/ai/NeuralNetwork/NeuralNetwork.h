//
// Created by Tobias on 02.10.2024.
// Refactored to use Tensor + ComputeBackend abstraction layer.
//

#ifndef MATURAPROJEKT_NEURALNETWORK_H
#define MATURAPROJEKT_NEURALNETWORK_H

#include <vector>
#include <random>
#include <fstream>
#include <chrono>
#include <sstream>
#include <memory>

#include "compute/Tensor.h"
#include "compute/Activations.h"
#include "compute/ComputeBackend.h"
#include "compute/BackendFactory.h"

class NeuralNetwork {
private:
    std::vector<Tensor> _weights;
    std::vector<Tensor> _biases;
    std::vector<Activations> _activations;
    std::shared_ptr<ComputeBackend> _backend;

public:
    NeuralNetwork();
    NeuralNetwork(std::vector<int> &topology, std::vector<Activations> &activations, int n = 1);
    NeuralNetwork(std::vector<int> &topology, std::vector<Activations> &activations, float min,
                  float max, bool uniform = true, int n = 1);
    explicit NeuralNetwork(const std::string &path);

    // Getter and setter
    [[nodiscard]] std::vector<Tensor> &weights() { return _weights; }
    [[nodiscard]] std::vector<Tensor> &biases() { return _biases; }
    [[nodiscard]] std::vector<Activations> &activationValues() { return _activations; }
    [[nodiscard]] Tensor &weights(int i) { return _weights[i]; }
    [[nodiscard]] Tensor &biases(int i) { return _biases[i]; }
    [[nodiscard]] Activations &activations(int i) { return _activations[i]; }

    // Functions
    bool load(const std::string &path);
    bool save(const std::string &path, int amount = 1);
    int networks();
    int size();
    size_t bytes();
    std::vector<int> topology();

    Tensor feed_forward(Tensor &input);
    Tensor feed_forward(std::vector<float> &input);

    Tensor feed_forward_single(Tensor &input, int index);
    Tensor feed_forward_single(std::vector<float> &input, int index);

    void breed(Tensor &fitness, int winners, float min, float max, bool uniform = true);
    void breed(std::vector<float> &fitness, int winners, float min, float max, bool uniform = true);
};


#endif //MATURAPROJEKT_NEURALNETWORK_H
