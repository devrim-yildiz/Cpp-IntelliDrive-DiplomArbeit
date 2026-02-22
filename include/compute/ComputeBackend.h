//
// ComputeBackend - abstract interface for compute operations.
// All tensor operations go through a backend, allowing CPU and GPU implementations.
//

#ifndef INTELLIDRIVE_COMPUTEBACKEND_H
#define INTELLIDRIVE_COMPUTEBACKEND_H

#include "Tensor.h"
#include "Activations.h"
#include <string>
#include <vector>
#include <memory>

class ComputeBackend {
public:
    virtual ~ComputeBackend() = default;
    virtual std::string name() const = 0;
    virtual void setup() = 0;

    // Random generation
    virtual Tensor randu(int d0, int d1 = 1, int d2 = 1, int d3 = 1) = 0;
    virtual Tensor randn(int d0, int d1 = 1, int d2 = 1, int d3 = 1) = 0;

    // Arithmetic (element-wise with broadcasting via modulo on b's dims)
    virtual Tensor matmul(const Tensor& a, const Tensor& b) = 0;
    virtual Tensor add(const Tensor& a, const Tensor& b) = 0;
    virtual Tensor multiply(const Tensor& a, const Tensor& b) = 0;
    virtual Tensor scale(const Tensor& a, float s) = 0;
    virtual Tensor add_scalar(const Tensor& a, float s) = 0;
    virtual Tensor subtract_from_scalar(float s, const Tensor& a) = 0;

    // Activation functions
    virtual Tensor apply_activation(const Tensor& a, Activations act) = 0;

    // Array manipulation
    virtual Tensor tile(const Tensor& a, int t0, int t1 = 1, int t2 = 1, int t3 = 1) = 0;
    virtual Tensor lookup(const Tensor& a, const std::vector<int>& indices, int dim) = 0;
    virtual Tensor join(int dim, const Tensor& a, const Tensor& b) = 0;
    virtual Tensor slice(const Tensor& a, int dim, int start, int end) = 0;
    virtual void assign_slice(Tensor& dest, const Tensor& src, int dim, int start, int end) = 0;
    virtual void add_to_slice(Tensor& dest, const Tensor& src, int dim, int start, int end) = 0;

    // Comparison (returns 0.0f/1.0f tensor)
    virtual Tensor greater_than(const Tensor& a, float val) = 0;

    // Constants
    virtual Tensor constant(float val, int d0, int d1 = 1, int d2 = 1, int d3 = 1) = 0;
};

#endif // INTELLIDRIVE_COMPUTEBACKEND_H
