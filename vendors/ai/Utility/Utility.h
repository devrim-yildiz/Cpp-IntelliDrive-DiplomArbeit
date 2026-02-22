//
// Created by Tobias on 01.10.2024.
// Refactored: ArrayFire-specific code removed. Pure CPU utility functions only.
//

#ifndef MATURAPROJEKT_UTILITY_H
#define MATURAPROJEKT_UTILITY_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

#include "compute/Activations.h"

class Utility {
public:
    // Backward compatibility alias for Activations
    using Activations = ::Activations;

    // Conversion functions for size_t
    static std::string sizeToString(size_t size);

    // Find the n biggest values in a vector
    static std::vector<int> find_top_n(const std::vector<float>& vec, int n);

    // Find the biggest elements index in a vector or reversed (used for AI training)
    static int mapVectorToIndex(std::vector<float> const &vector);
    static std::vector<float> mapIndexToVector(int index, int size);
};

#endif //MATURAPROJEKT_UTILITY_H
