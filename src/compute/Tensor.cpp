//
// Tensor implementation
//

#include "compute/Tensor.h"

Tensor::Tensor() : _dims{0, 0, 0, 0} {}

Tensor::Tensor(int d0, int d1, int d2, int d3)
    : _dims{d0, d1, d2, d3}, _data(static_cast<size_t>(d0) * d1 * d2 * d3, 0.0f) {}

Tensor::Tensor(int d0, int d1, int d2, int d3, const float* data)
    : _dims{d0, d1, d2, d3}, _data(data, data + static_cast<size_t>(d0) * d1 * d2 * d3) {}

Tensor::Tensor(int d0, int d1, int d2, int d3, const std::vector<float>& data)
    : _dims{d0, d1, d2, d3}, _data(data) {
    _data.resize(static_cast<size_t>(d0) * d1 * d2 * d3, 0.0f);
}

float& Tensor::operator()(int i0, int i1, int i2, int i3) {
    return _data[idx(i0, i1, i2, i3)];
}

float Tensor::operator()(int i0, int i1, int i2, int i3) const {
    return _data[idx(i0, i1, i2, i3)];
}

void Tensor::host(float* ptr) const {
    std::copy(_data.begin(), _data.end(), ptr);
}
