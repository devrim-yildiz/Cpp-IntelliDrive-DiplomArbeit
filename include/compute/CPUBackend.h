//
// CPUBackend - pure CPU implementation of all compute operations.
// Uses standard library only. No GPU dependencies.
//

#ifndef INTELLIDRIVE_CPUBACKEND_H
#define INTELLIDRIVE_CPUBACKEND_H

#include "ComputeBackend.h"
#include <random>

class CPUBackend : public ComputeBackend {
public:
    CPUBackend();
    std::string name() const override { return "CPU"; }
    void setup() override;

    Tensor randu(int d0, int d1, int d2, int d3) override;
    Tensor randn(int d0, int d1, int d2, int d3) override;

    Tensor matmul(const Tensor& a, const Tensor& b) override;
    Tensor add(const Tensor& a, const Tensor& b) override;
    Tensor multiply(const Tensor& a, const Tensor& b) override;
    Tensor scale(const Tensor& a, float s) override;
    Tensor add_scalar(const Tensor& a, float s) override;
    Tensor subtract_from_scalar(float s, const Tensor& a) override;

    Tensor apply_activation(const Tensor& a, Activations act) override;

    Tensor tile(const Tensor& a, int t0, int t1, int t2, int t3) override;
    Tensor lookup(const Tensor& a, const std::vector<int>& indices, int dim) override;
    Tensor join(int dim, const Tensor& a, const Tensor& b) override;
    Tensor slice(const Tensor& a, int dim, int start, int end) override;
    void assign_slice(Tensor& dest, const Tensor& src, int dim, int start, int end) override;
    void add_to_slice(Tensor& dest, const Tensor& src, int dim, int start, int end) override;

    Tensor greater_than(const Tensor& a, float val) override;

    Tensor constant(float val, int d0, int d1, int d2, int d3) override;

private:
    std::mt19937 _rng;
};

#endif // INTELLIDRIVE_CPUBACKEND_H
