//
// CPUBackend - pure CPU implementation of all compute operations.
//

#include "compute/CPUBackend.h"
#include <cmath>
#include <algorithm>
#include <chrono>
#include <iostream>

CPUBackend::CPUBackend()
    : _rng(static_cast<unsigned int>(std::chrono::steady_clock::now().time_since_epoch().count())) {}

void CPUBackend::setup() {
    std::cout << "CPU Backend initialized.\n";
}

Tensor CPUBackend::randu(int d0, int d1, int d2, int d3) {
    Tensor result(d0, d1, d2, d3);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (auto& v : result.data()) {
        v = dist(_rng);
    }
    return result;
}

Tensor CPUBackend::randn(int d0, int d1, int d2, int d3) {
    Tensor result(d0, d1, d2, d3);
    std::normal_distribution<float> dist(0.0f, 1.0f);
    for (auto& v : result.data()) {
        v = dist(_rng);
    }
    return result;
}

Tensor CPUBackend::matmul(const Tensor& a, const Tensor& b) {
    // Batched 2D matmul: for each (d2, d3) pair, compute A[:,:] * B[:,:].
    // A: (M, K, N1, N2), B: (K, P, N1, N2) -> result: (M, P, N1, N2)
    int M = a.dim(0);
    int K = a.dim(1);
    int P = b.dim(1);
    int N1 = a.dim(2);
    int N2 = a.dim(3);

    Tensor result(M, P, N1, N2);

    for (int n2 = 0; n2 < N2; ++n2) {
        for (int n1 = 0; n1 < N1; ++n1) {
            for (int p = 0; p < P; ++p) {
                for (int m = 0; m < M; ++m) {
                    float sum = 0.0f;
                    for (int k = 0; k < K; ++k) {
                        sum += a(m, k, n1, n2) * b(k, p, n1, n2);
                    }
                    result(m, p, n1, n2) = sum;
                }
            }
        }
    }

    return result;
}

Tensor CPUBackend::add(const Tensor& a, const Tensor& b) {
    Tensor result(a.dim(0), a.dim(1), a.dim(2), a.dim(3));
    int bd0 = b.dim(0), bd1 = b.dim(1), bd2 = b.dim(2), bd3 = b.dim(3);

    for (int d3 = 0; d3 < a.dim(3); ++d3) {
        for (int d2 = 0; d2 < a.dim(2); ++d2) {
            for (int d1 = 0; d1 < a.dim(1); ++d1) {
                for (int d0 = 0; d0 < a.dim(0); ++d0) {
                    result(d0, d1, d2, d3) = a(d0, d1, d2, d3) +
                        b(d0 % bd0, d1 % bd1, d2 % bd2, d3 % bd3);
                }
            }
        }
    }

    return result;
}

Tensor CPUBackend::multiply(const Tensor& a, const Tensor& b) {
    Tensor result(a.dim(0), a.dim(1), a.dim(2), a.dim(3));
    const auto& ad = a.data();
    const auto& bd = b.data();
    auto& rd = result.data();
    for (int i = 0; i < a.elements(); ++i) {
        rd[i] = ad[i] * bd[i];
    }
    return result;
}

Tensor CPUBackend::scale(const Tensor& a, float s) {
    Tensor result(a.dim(0), a.dim(1), a.dim(2), a.dim(3));
    const auto& ad = a.data();
    auto& rd = result.data();
    for (int i = 0; i < a.elements(); ++i) {
        rd[i] = ad[i] * s;
    }
    return result;
}

Tensor CPUBackend::add_scalar(const Tensor& a, float s) {
    Tensor result(a.dim(0), a.dim(1), a.dim(2), a.dim(3));
    const auto& ad = a.data();
    auto& rd = result.data();
    for (int i = 0; i < a.elements(); ++i) {
        rd[i] = ad[i] + s;
    }
    return result;
}

Tensor CPUBackend::subtract_from_scalar(float s, const Tensor& a) {
    Tensor result(a.dim(0), a.dim(1), a.dim(2), a.dim(3));
    const auto& ad = a.data();
    auto& rd = result.data();
    for (int i = 0; i < a.elements(); ++i) {
        rd[i] = s - ad[i];
    }
    return result;
}

Tensor CPUBackend::apply_activation(const Tensor& a, Activations act) {
    Tensor result(a.dim(0), a.dim(1), a.dim(2), a.dim(3));
    const auto& ad = a.data();
    auto& rd = result.data();
    int n = a.elements();

    switch (act) {
        case Activations::ReLU:
            for (int i = 0; i < n; ++i)
                rd[i] = std::max(ad[i], 0.0f);
            break;
        case Activations::LeakyReLU:
            for (int i = 0; i < n; ++i)
                rd[i] = ad[i] > 0.0f ? ad[i] : 0.1f * ad[i];
            break;
        case Activations::Sigmoid:
            for (int i = 0; i < n; ++i)
                rd[i] = 1.0f / (1.0f + std::exp(-ad[i]));
            break;
        case Activations::Tanh:
            for (int i = 0; i < n; ++i)
                rd[i] = std::tanh(ad[i]);
            break;
        case Activations::Linear:
        default:
            for (int i = 0; i < n; ++i)
                rd[i] = ad[i];
            break;
    }

    return result;
}

Tensor CPUBackend::tile(const Tensor& a, int t0, int t1, int t2, int t3) {
    int nd0 = a.dim(0) * t0;
    int nd1 = a.dim(1) * t1;
    int nd2 = a.dim(2) * t2;
    int nd3 = a.dim(3) * t3;

    Tensor result(nd0, nd1, nd2, nd3);

    for (int d3 = 0; d3 < nd3; ++d3) {
        for (int d2 = 0; d2 < nd2; ++d2) {
            for (int d1 = 0; d1 < nd1; ++d1) {
                for (int d0 = 0; d0 < nd0; ++d0) {
                    result(d0, d1, d2, d3) = a(d0 % a.dim(0), d1 % a.dim(1),
                                                d2 % a.dim(2), d3 % a.dim(3));
                }
            }
        }
    }

    return result;
}

Tensor CPUBackend::lookup(const Tensor& a, const std::vector<int>& indices, int dim) {
    int n = static_cast<int>(indices.size());
    int nd[4] = {a.dim(0), a.dim(1), a.dim(2), a.dim(3)};
    nd[dim] = n;

    Tensor result(nd[0], nd[1], nd[2], nd[3]);

    for (int d3 = 0; d3 < nd[3]; ++d3) {
        for (int d2 = 0; d2 < nd[2]; ++d2) {
            for (int d1 = 0; d1 < nd[1]; ++d1) {
                for (int d0 = 0; d0 < nd[0]; ++d0) {
                    int src[4] = {d0, d1, d2, d3};
                    // Replace the index in the lookup dimension with the mapped index
                    int idx_in_dim;
                    switch (dim) {
                        case 0: idx_in_dim = d0; break;
                        case 1: idx_in_dim = d1; break;
                        case 2: idx_in_dim = d2; break;
                        default: idx_in_dim = d3; break;
                    }
                    src[dim] = indices[idx_in_dim];
                    result(d0, d1, d2, d3) = a(src[0], src[1], src[2], src[3]);
                }
            }
        }
    }

    return result;
}

Tensor CPUBackend::join(int dim, const Tensor& a, const Tensor& b) {
    int nd[4] = {a.dim(0), a.dim(1), a.dim(2), a.dim(3)};
    nd[dim] = a.dim(dim) + b.dim(dim);

    Tensor result(nd[0], nd[1], nd[2], nd[3]);

    // Copy a
    for (int d3 = 0; d3 < a.dim(3); ++d3) {
        for (int d2 = 0; d2 < a.dim(2); ++d2) {
            for (int d1 = 0; d1 < a.dim(1); ++d1) {
                for (int d0 = 0; d0 < a.dim(0); ++d0) {
                    result(d0, d1, d2, d3) = a(d0, d1, d2, d3);
                }
            }
        }
    }

    // Copy b with offset in the join dimension
    int offset = a.dim(dim);
    for (int d3 = 0; d3 < b.dim(3); ++d3) {
        for (int d2 = 0; d2 < b.dim(2); ++d2) {
            for (int d1 = 0; d1 < b.dim(1); ++d1) {
                for (int d0 = 0; d0 < b.dim(0); ++d0) {
                    int dst[4] = {d0, d1, d2, d3};
                    dst[dim] += offset;
                    result(dst[0], dst[1], dst[2], dst[3]) = b(d0, d1, d2, d3);
                }
            }
        }
    }

    return result;
}

Tensor CPUBackend::slice(const Tensor& a, int dim, int start, int end) {
    int count = end - start + 1;
    int nd[4] = {a.dim(0), a.dim(1), a.dim(2), a.dim(3)};
    nd[dim] = count;

    Tensor result(nd[0], nd[1], nd[2], nd[3]);

    for (int d3 = 0; d3 < nd[3]; ++d3) {
        for (int d2 = 0; d2 < nd[2]; ++d2) {
            for (int d1 = 0; d1 < nd[1]; ++d1) {
                for (int d0 = 0; d0 < nd[0]; ++d0) {
                    int src[4] = {d0, d1, d2, d3};
                    src[dim] += start;
                    result(d0, d1, d2, d3) = a(src[0], src[1], src[2], src[3]);
                }
            }
        }
    }

    return result;
}

void CPUBackend::assign_slice(Tensor& dest, const Tensor& src, int dim, int start, int end) {
    for (int d3 = 0; d3 < src.dim(3); ++d3) {
        for (int d2 = 0; d2 < src.dim(2); ++d2) {
            for (int d1 = 0; d1 < src.dim(1); ++d1) {
                for (int d0 = 0; d0 < src.dim(0); ++d0) {
                    int dst[4] = {d0, d1, d2, d3};
                    dst[dim] += start;
                    dest(dst[0], dst[1], dst[2], dst[3]) = src(d0, d1, d2, d3);
                }
            }
        }
    }
}

void CPUBackend::add_to_slice(Tensor& dest, const Tensor& src, int dim, int start, int end) {
    for (int d3 = 0; d3 < src.dim(3); ++d3) {
        for (int d2 = 0; d2 < src.dim(2); ++d2) {
            for (int d1 = 0; d1 < src.dim(1); ++d1) {
                for (int d0 = 0; d0 < src.dim(0); ++d0) {
                    int dst[4] = {d0, d1, d2, d3};
                    dst[dim] += start;
                    dest(dst[0], dst[1], dst[2], dst[3]) += src(d0, d1, d2, d3);
                }
            }
        }
    }
}

Tensor CPUBackend::greater_than(const Tensor& a, float val) {
    Tensor result(a.dim(0), a.dim(1), a.dim(2), a.dim(3));
    const auto& ad = a.data();
    auto& rd = result.data();
    for (int i = 0; i < a.elements(); ++i) {
        rd[i] = ad[i] > val ? 1.0f : 0.0f;
    }
    return result;
}

Tensor CPUBackend::constant(float val, int d0, int d1, int d2, int d3) {
    Tensor result(d0, d1, d2, d3);
    auto& rd = result.data();
    std::fill(rd.begin(), rd.end(), val);
    return result;
}
