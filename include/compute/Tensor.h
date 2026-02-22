//
// Tensor - a simple 4D array backed by std::vector<float> in column-major order.
// Compatible with ArrayFire's dim4 layout for seamless data exchange.
//

#ifndef INTELLIDRIVE_TENSOR_H
#define INTELLIDRIVE_TENSOR_H

#include <vector>
#include <array>
#include <cstring>
#include <algorithm>

class Tensor {
public:
    Tensor();
    explicit Tensor(int d0, int d1 = 1, int d2 = 1, int d3 = 1);
    Tensor(int d0, int d1, int d2, int d3, const float* data);
    Tensor(int d0, int d1, int d2, int d3, const std::vector<float>& data);

    int dim(int i) const { return _dims[i]; }
    int elements() const { return _dims[0] * _dims[1] * _dims[2] * _dims[3]; }
    size_t bytes() const { return static_cast<size_t>(elements()) * sizeof(float); }
    bool empty() const { return _data.empty(); }

    float& operator()(int i0, int i1 = 0, int i2 = 0, int i3 = 0);
    float operator()(int i0, int i1 = 0, int i2 = 0, int i3 = 0) const;

    std::vector<float>& data() { return _data; }
    const std::vector<float>& data() const { return _data; }

    // Copy data to an external buffer (like af::array::host)
    void host(float* ptr) const;

    // Shape as array for serialization
    std::array<int, 4> shape() const { return {_dims[0], _dims[1], _dims[2], _dims[3]}; }

private:
    int _dims[4] = {0, 0, 0, 0};
    std::vector<float> _data;

    int idx(int i0, int i1, int i2, int i3) const {
        // Column-major order (matching ArrayFire)
        return i0 + i1 * _dims[0] + i2 * _dims[0] * _dims[1] + i3 * _dims[0] * _dims[1] * _dims[2];
    }
};

#endif // INTELLIDRIVE_TENSOR_H
