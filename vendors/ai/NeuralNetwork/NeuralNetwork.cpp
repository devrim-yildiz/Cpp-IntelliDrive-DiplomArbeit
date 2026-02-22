//
// Created by Tobias on 02.10.2024.
// Refactored to use Tensor + ComputeBackend abstraction layer.
//

#include "NeuralNetwork.h"
#include "json/json.hpp"
#include "../Utility/Utility.h"

NeuralNetwork::NeuralNetwork()
    : _backend(BackendFactory::getBackend()) {}

NeuralNetwork::NeuralNetwork(std::vector<int> &topology, std::vector<Activations> &activations, int n)
    : _backend(BackendFactory::getBackend()) {
    if (activations.size() != topology.size() - 1) {
        std::cerr << "Sizes do not match!" << "\n";
        return;
    }

    _activations = activations;
    for (int i = 1; i < (int)topology.size(); ++i) {
        int currentNeurons = topology[i];
        int previousNeurons = topology[i - 1];

        _weights.emplace_back(currentNeurons, previousNeurons, n);
        _biases.emplace_back(currentNeurons, 1, n);
    }
}

NeuralNetwork::NeuralNetwork(std::vector<int> &topology, std::vector<Activations> &activations, float min,
                             float max, bool uniform, int n)
    : _backend(BackendFactory::getBackend()) {
    if (activations.size() != topology.size() - 1) {
        std::cerr << "Sizes do not match!" << "\n";
        return;
    }

    _activations = activations;
    for (int i = 1; i < (int)topology.size(); ++i) {
        int currentNeurons = topology[i];
        int previousNeurons = topology[i - 1];

        if (uniform) {
            _weights.push_back(_backend->add_scalar(_backend->scale(_backend->randu(currentNeurons, previousNeurons, n), max - min), min));
            _biases.push_back(_backend->add_scalar(_backend->scale(_backend->randu(currentNeurons, 1, n), max - min), min));
        } else {
            _weights.push_back(_backend->add_scalar(_backend->scale(_backend->randn(currentNeurons, previousNeurons, n), max - min), min));
            _biases.push_back(_backend->add_scalar(_backend->scale(_backend->randn(currentNeurons, 1, n), max - min), min));
        }
    }
}

Tensor NeuralNetwork::feed_forward(Tensor &input) {
    Tensor value = input;

    if (_weights.empty()) {
        std::cerr << "The network does not possess any layers!" << "\n";
        return value;
    }

    if (input.dim(0) != _weights[0].dim(1) || input.dim(2) != _weights[0].dim(2)) {
        std::cerr << "The input dimension must match the first layer's weight dimensions!" << "\n";
        return value;
    }

    // The batches are stored in the 4th dimension because the 3rd dimension is occupied by the networks
    int batchSize = value.dim(3);

    for (int i = 0; i < (int)_weights.size(); ++i) {
        Tensor weights = _backend->tile(_weights[i], 1, 1, 1, batchSize);
        Tensor biases = _backend->tile(_biases[i], 1, 1, 1, batchSize);

        // z = activation(weights * inputs + biases)
        value = _backend->add(_backend->matmul(weights, value), biases);
        value = _backend->apply_activation(value, _activations[i]);
    }

    return value;
}

Tensor NeuralNetwork::feed_forward(std::vector<float> &input) {
    Tensor in(static_cast<int>(input.size()), 1, 1, 1, input);
    return feed_forward(in);
}

Tensor NeuralNetwork::feed_forward_single(Tensor &input, int index) {
    Tensor value = input;

    if (_weights.empty()) {
        std::cerr << "The network does not possess any layers!" << "\n";
        return value;
    }

    if (input.dim(0) != _weights[0].dim(1)) {
        std::cerr << "The input dimension must match the first layer's weight dimensions!" << "\n";
        return value;
    }

    // Get the batch size from the input
    int batchSize = value.dim(2);

    for (int i = 0; i < (int)_weights.size(); ++i) {
        Tensor weightSlice = _backend->lookup(_weights[i], {index}, 2);
        Tensor biasSlice = _backend->lookup(_biases[i], {index}, 2);

        weightSlice = _backend->tile(weightSlice, 1, 1, batchSize, 1);
        biasSlice = _backend->tile(biasSlice, 1, 1, batchSize, 1);

        // z = activation(weights * inputs + biases)
        value = _backend->add(_backend->matmul(weightSlice, value), biasSlice);
        value = _backend->apply_activation(value, _activations[i]);
    }

    return value;
}

Tensor NeuralNetwork::feed_forward_single(std::vector<float> &input, int index) {
    Tensor in(static_cast<int>(input.size()), 1, 1, 1, input);
    return feed_forward_single(in, index);
}

void NeuralNetwork::breed(std::vector<float> &fitness, int winners, float min, float max, bool uniform) {
    if (_weights.empty()) {
        std::cerr << "The network does not possess any layers!" << "\n";
        return;
    }

    int numNetworks = _weights[0].dim(2);

    if (winners > numNetworks) {
        std::cerr << "The number of winners cannot be higher than the number of networks!\n";
        return;
    }

    // Buffer for child networks - pre-filled with mutation values
    std::vector<Tensor> weights;
    std::vector<Tensor> biases;

    for (int i = 0; i < (int)_weights.size(); ++i) {
        auto ws = _weights[i].shape();
        auto bs = _biases[i].shape();

        if (uniform) {
            weights.push_back(_backend->add_scalar(_backend->scale(_backend->randu(ws[0], ws[1], ws[2], ws[3]), max - min), min));
            biases.push_back(_backend->add_scalar(_backend->scale(_backend->randu(bs[0], bs[1], bs[2], bs[3]), max - min), min));
        } else {
            weights.push_back(_backend->add_scalar(_backend->scale(_backend->randn(ws[0], ws[1], ws[2], ws[3]), max - min), min));
            biases.push_back(_backend->add_scalar(_backend->scale(_backend->randn(bs[0], bs[1], bs[2], bs[3]), max - min), min));
        }
    }

    // Find the best neural networks
    auto selectedNetworks = Utility::find_top_n(fitness, winners);

    // Copy the winners into the children to preserve them
    for (int layer = 0; layer < (int)_weights.size(); ++layer) {
        Tensor selectedWeights = _backend->lookup(_weights[layer], selectedNetworks, 2);
        Tensor selectedBiases = _backend->lookup(_biases[layer], selectedNetworks, 2);

        _backend->assign_slice(weights[layer], selectedWeights, 2, 0, static_cast<int>(selectedNetworks.size()) - 1);
        _backend->assign_slice(biases[layer], selectedBiases, 2, 0, static_cast<int>(selectedNetworks.size()) - 1);
    }

    // Decide the breeding pairs
    int numPairs = numNetworks - winners;
    std::vector<int> n1Vec(numPairs), n2Vec(numPairs);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(selectedNetworks.size()) - 1);

    for (int i = 0; i < numPairs; ++i) {
        n1Vec[i] = dis(gen);
        n2Vec[i] = dis(gen);
    }

    // Cross the values of the networks
    for (int layer = 0; layer < (int)_weights.size(); ++layer) {
        int wd0 = _weights[layer].dim(0);
        int wd1 = _weights[layer].dim(1);
        int bd0 = _biases[layer].dim(0);
        int bd1 = _biases[layer].dim(1);

        // Generate masks for all network pairs
        Tensor wMasks = _backend->greater_than(_backend->randu(wd0, wd1, numPairs, 1), 0.5f);
        Tensor bMasks = _backend->greater_than(_backend->randu(bd0, bd1, numPairs, 1), 0.5f);

        // Extract parent weights and biases
        Tensor parent1Weights = _backend->lookup(_weights[layer], n1Vec, 2);
        Tensor parent2Weights = _backend->lookup(_weights[layer], n2Vec, 2);
        Tensor parent1Biases = _backend->lookup(_biases[layer], n1Vec, 2);
        Tensor parent2Biases = _backend->lookup(_biases[layer], n2Vec, 2);

        // Perform crossover using masks
        Tensor invWMasks = _backend->subtract_from_scalar(1.0f, wMasks);
        Tensor invBMasks = _backend->subtract_from_scalar(1.0f, bMasks);

        Tensor newWeights = _backend->add(
            _backend->multiply(parent1Weights, wMasks),
            _backend->multiply(parent2Weights, invWMasks)
        );
        Tensor newBiases = _backend->add(
            _backend->multiply(parent1Biases, bMasks),
            _backend->multiply(parent2Biases, invBMasks)
        );

        // Assign the new weights and biases (adding to mutation values)
        _backend->add_to_slice(weights[layer], newWeights, 2, winners, winners + numPairs - 1);
        _backend->add_to_slice(biases[layer], newBiases, 2, winners, winners + numPairs - 1);
    }

    // Copy the children into the networks
    for (int layer = 0; layer < (int)_weights.size(); ++layer) {
        _weights[layer] = weights[layer];
        _biases[layer] = biases[layer];
    }
}

void NeuralNetwork::breed(Tensor &fitness, int winners, float min, float max, bool uniform) {
    std::vector<float> fitnessVec = fitness.data();
    breed(fitnessVec, winners, min, max, uniform);
}

int NeuralNetwork::networks() {
    if (_weights.empty()) {
        std::cerr << "The network does not possess any layers!" << "\n";
        return -1;
    }
    return _weights[0].dim(2);
}

int NeuralNetwork::size() {
    return (int)_weights.size() + 1;
}

size_t NeuralNetwork::bytes() {
    if (_weights.empty()) {
        std::cerr << "The network does not possess any layers!" << "\n";
        return 0;
    }
    size_t totalBytes = 0;
    for (int i = 0; i < (int)_weights.size(); ++i) {
        totalBytes += _biases[i].bytes() + _weights[i].bytes();
    }
    return totalBytes;
}

std::vector<int> NeuralNetwork::topology() {
    std::vector<int> output;

    for (int i = 0; i < (int)_weights.size(); ++i) {
        int currentNeurons = _weights[i].dim(0);
        int previousNeurons = _weights[i].dim(1);

        if (i == 0) {
            output.emplace_back(previousNeurons);
        }

        output.emplace_back(currentNeurons);
    }

    return output;
}

bool NeuralNetwork::save(const std::string &path, int n) {
    // Ensure that n does not exceed the actual number of networks
    int numNetworks = this->networks();
    if (n > numNetworks) {
        n = numNetworks;
    }

    // Prepare JSON
    nlohmann::json j;
    j["topology"] = this->topology();

    std::vector<int> activationVec(_activations.size());
    for (size_t i = 0; i < _activations.size(); i++) {
        activationVec[i] = static_cast<int>(_activations[i]);
    }
    j["activations"] = activationVec;
    j["num_networks"] = numNetworks;

    nlohmann::json layersJson = nlohmann::json::array();
    for (size_t i = 0; i < _weights.size(); i++) {
        // Slice out only [0..n-1] in the 3rd dimension
        Tensor wSub = _backend->slice(_weights[i], 2, 0, n - 1);
        Tensor bSub = _backend->slice(_biases[i], 2, 0, n - 1);

        auto wShape = wSub.shape();
        auto bShape = bSub.shape();

        // Build JSON layer entry
        nlohmann::json layerJson;
        layerJson["weights_shape"] = {wShape[0], wShape[1], wShape[2], wShape[3]};
        layerJson["biases_shape"] = {bShape[0], bShape[1], bShape[2], bShape[3]};
        layerJson["weights_data"] = wSub.data();
        layerJson["biases_data"] = bSub.data();

        layersJson.push_back(layerJson);
    }
    j["layers"] = layersJson;

    // Write to file
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << path << "\n";
        return false;
    }
    file << j.dump(4);
    file.close();
    return true;
}

bool NeuralNetwork::load(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for reading: " << path << "\n";
        return false;
    }

    nlohmann::json j;
    try {
        file >> j;
    } catch (const nlohmann::json::exception &e) {
        std::cerr << "JSON parse error: " << e.what() << "\n";
        file.close();
        return false;
    }
    file.close();

    auto actVec = j["activations"].get<std::vector<int>>();
    int net = j["num_networks"].get<int>();

    // Convert activations
    std::vector<Activations> loadedActivations(actVec.size());
    for (size_t i = 0; i < actVec.size(); i++) {
        loadedActivations[i] = static_cast<Activations>(actVec[i]);
    }

    int currentNetworks = _weights.empty() ? 0 : _weights[0].dim(2);

    _weights.clear();
    _biases.clear();
    _activations = loadedActivations;

    // Parse each layer from the JSON
    auto layersJson = j["layers"];
    for (auto &layer : layersJson) {
        // Shapes
        auto wShape = layer["weights_shape"].get<std::vector<int>>();
        auto bShape = layer["biases_shape"].get<std::vector<int>>();

        // Data
        std::vector<float> wData = layer["weights_data"].get<std::vector<float>>();
        std::vector<float> bData = layer["biases_data"].get<std::vector<float>>();

        // Construct Tensors
        Tensor wArr(wShape[0], wShape[1], wShape[2], wShape[3], wData);
        Tensor bArr(bShape[0], bShape[1], bShape[2], bShape[3], bData);

        // Check if 'net' is greater than what was actually saved in this layer
        int savedNetworks = wArr.dim(2);
        if (net > savedNetworks && savedNetworks > 0 && currentNetworks > 0) {
            int fullRepeats = currentNetworks / savedNetworks;
            int leftover = currentNetworks % savedNetworks;

            Tensor wRepeated = _backend->tile(wArr, 1, 1, fullRepeats, 1);
            Tensor bRepeated = _backend->tile(bArr, 1, 1, fullRepeats, 1);

            if (leftover > 0) {
                Tensor wPartial = _backend->slice(wArr, 2, 0, leftover - 1);
                Tensor bPartial = _backend->slice(bArr, 2, 0, leftover - 1);
                wRepeated = _backend->join(2, wRepeated, wPartial);
                bRepeated = _backend->join(2, bRepeated, bPartial);
            }
            wArr = wRepeated;
            bArr = bRepeated;
        }

        _weights.push_back(wArr);
        _biases.push_back(bArr);
    }

    return true;
}

NeuralNetwork::NeuralNetwork(const std::string &path)
    : _backend(BackendFactory::getBackend()) {
    load(path);
}
