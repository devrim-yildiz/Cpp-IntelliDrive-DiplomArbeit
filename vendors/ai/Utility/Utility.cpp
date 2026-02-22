//
// Created by Tobias on 01.10.2024.
// Refactored: ArrayFire-specific code removed. Pure CPU utility functions only.
//

#include "Utility.h"
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

std::string Utility::sizeToString(size_t size) {
    std::string out;
    int unit = 0;
    int komma = 0;

    while(size > 1000){
        size_t temp = size / 1000;
        komma = static_cast<int>(size / 10 - temp);
        size = temp;
        unit++;
    }

    out = std::to_string(size) + "." + std::to_string(komma);

    switch (unit) {
        case 0: out += " Byte"; break;
        case 1: out += " kB"; break;
        case 2: out += " MB"; break;
        case 3: out += " GB"; break;
        case 4: out += " TB"; break;
        case 5: out += " PB"; break;
        default: out += " undefined unit"; break;
    }

    return out;
}

std::vector<int> Utility::find_top_n(const std::vector<float>& vec, int n) {
    if (vec.empty() || n <= 0) {
        std::cerr << "The vector cannot be empty.\n";
        return {};
    }

    // Create a vector of pairs (value, index)
    std::vector<std::pair<float, int>> value_index_pairs;
    value_index_pairs.reserve(vec.size());

    for (size_t i = 0; i < vec.size(); ++i) {
        value_index_pairs.emplace_back(vec[i], static_cast<int>(i));
    }

    // Sort the pairs based on the values in descending order
    std::sort(value_index_pairs.begin(), value_index_pairs.end(),
              [](const std::pair<float, int>& a, const std::pair<float, int>& b) -> bool {
                  return a.first > b.first;
              }
    );

    n = std::min(n, static_cast<int>(value_index_pairs.size()));

    // Extract the indices of the top n elements
    std::vector<int> result;
    result.reserve(n);

    for (int i = 0; i < n; ++i) {
        result.push_back(value_index_pairs[i].second);
    }

    return result;
}

int Utility::mapVectorToIndex(const std::vector<float> &vector) {
    return find_top_n(vector, 1)[0];
}

std::vector<float> Utility::mapIndexToVector(int index, int size) {
    if(index >= size){
        std::cerr << "The index cannot be greater then or equal to the size.\n";
        return {};
    }

    std::vector<float> vector(size, 0.0f);
    vector[index] = 1.0f;
    return vector;
}