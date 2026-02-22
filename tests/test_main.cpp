//
// Unit tests for the compute abstraction layer and neural network.
// Standalone - no SFML or GUI dependencies.
//

#include <iostream>
#include <cmath>
#include <cassert>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

#include "compute/Tensor.h"
#include "compute/CPUBackend.h"
#include "compute/BackendFactory.h"
#include "compute/Activations.h"
#include "NeuralNetwork/NeuralNetwork.h"
#include "Utility/Utility.h"

static int tests_passed = 0;
static int tests_failed = 0;

#define TEST(name) \
    std::cout << "  Running: " << name << "... "; \
    try {

#define END_TEST \
        std::cout << "PASSED\n"; \
        tests_passed++; \
    } catch (const std::exception& e) { \
        std::cout << "FAILED: " << e.what() << "\n"; \
        tests_failed++; \
    } catch (...) { \
        std::cout << "FAILED (unknown exception)\n"; \
        tests_failed++; \
    }

#define ASSERT_EQ(a, b) \
    if ((a) != (b)) { \
        std::ostringstream ss; \
        ss << "Expected " << (b) << " but got " << (a) << " at line " << __LINE__; \
        throw std::runtime_error(ss.str()); \
    }

#define ASSERT_NEAR(a, b, eps) \
    if (std::fabs((a) - (b)) > (eps)) { \
        std::ostringstream ss; \
        ss << "Expected ~" << (b) << " but got " << (a) << " (eps=" << (eps) << ") at line " << __LINE__; \
        throw std::runtime_error(ss.str()); \
    }

#define ASSERT_TRUE(cond) \
    if (!(cond)) { \
        std::ostringstream ss; \
        ss << "Assertion failed at line " << __LINE__; \
        throw std::runtime_error(ss.str()); \
    }

// ===========================================================================
// Tensor Tests
// ===========================================================================
void test_tensor() {
    std::cout << "\n=== Tensor Tests ===\n";

    TEST("Default constructor")
        Tensor t;
        ASSERT_EQ(t.dim(0), 0);
        ASSERT_EQ(t.elements(), 0);
        ASSERT_TRUE(t.empty());
    END_TEST

    TEST("Sized constructor")
        Tensor t(3, 2, 1, 1);
        ASSERT_EQ(t.dim(0), 3);
        ASSERT_EQ(t.dim(1), 2);
        ASSERT_EQ(t.dim(2), 1);
        ASSERT_EQ(t.dim(3), 1);
        ASSERT_EQ(t.elements(), 6);
        ASSERT_EQ(t.bytes(), 6 * sizeof(float));
        ASSERT_TRUE(!t.empty());
    END_TEST

    TEST("Data constructor from pointer")
        float data[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
        Tensor t(3, 2, 1, 1, data);
        // Column-major: (0,0)=1, (1,0)=2, (2,0)=3, (0,1)=4, (1,1)=5, (2,1)=6
        ASSERT_NEAR(t(0, 0), 1.0f, 1e-6f);
        ASSERT_NEAR(t(1, 0), 2.0f, 1e-6f);
        ASSERT_NEAR(t(2, 0), 3.0f, 1e-6f);
        ASSERT_NEAR(t(0, 1), 4.0f, 1e-6f);
        ASSERT_NEAR(t(1, 1), 5.0f, 1e-6f);
        ASSERT_NEAR(t(2, 1), 6.0f, 1e-6f);
    END_TEST

    TEST("Data constructor from vector")
        std::vector<float> data = {1.0f, 2.0f, 3.0f, 4.0f};
        Tensor t(2, 2, 1, 1, data);
        ASSERT_NEAR(t(0, 0), 1.0f, 1e-6f);
        ASSERT_NEAR(t(1, 0), 2.0f, 1e-6f);
        ASSERT_NEAR(t(0, 1), 3.0f, 1e-6f);
        ASSERT_NEAR(t(1, 1), 4.0f, 1e-6f);
    END_TEST

    TEST("Host copy")
        std::vector<float> data = {1.0f, 2.0f, 3.0f};
        Tensor t(3, 1, 1, 1, data);
        float buf[3] = {0};
        t.host(buf);
        ASSERT_NEAR(buf[0], 1.0f, 1e-6f);
        ASSERT_NEAR(buf[1], 2.0f, 1e-6f);
        ASSERT_NEAR(buf[2], 3.0f, 1e-6f);
    END_TEST

    TEST("Shape accessor")
        Tensor t(4, 3, 2, 1);
        auto s = t.shape();
        ASSERT_EQ(s[0], 4);
        ASSERT_EQ(s[1], 3);
        ASSERT_EQ(s[2], 2);
        ASSERT_EQ(s[3], 1);
    END_TEST

    TEST("3D tensor indexing")
        // (2, 2, 2) tensor
        std::vector<float> data = {1, 2, 3, 4, 5, 6, 7, 8};
        Tensor t(2, 2, 2, 1, data);
        // Column-major: idx = i0 + i1*d0 + i2*d0*d1
        ASSERT_NEAR(t(0, 0, 0), 1.0f, 1e-6f);
        ASSERT_NEAR(t(1, 0, 0), 2.0f, 1e-6f);
        ASSERT_NEAR(t(0, 1, 0), 3.0f, 1e-6f);
        ASSERT_NEAR(t(1, 1, 0), 4.0f, 1e-6f);
        ASSERT_NEAR(t(0, 0, 1), 5.0f, 1e-6f);
        ASSERT_NEAR(t(1, 0, 1), 6.0f, 1e-6f);
        ASSERT_NEAR(t(0, 1, 1), 7.0f, 1e-6f);
        ASSERT_NEAR(t(1, 1, 1), 8.0f, 1e-6f);
    END_TEST
}

// ===========================================================================
// CPU Backend Tests
// ===========================================================================
void test_cpu_backend() {
    std::cout << "\n=== CPU Backend Tests ===\n";
    CPUBackend backend;
    backend.setup();

    TEST("Random uniform generation")
        Tensor r = backend.randu(10, 1, 1, 1);
        ASSERT_EQ(r.dim(0), 10);
        ASSERT_EQ(r.elements(), 10);
        for (int i = 0; i < 10; ++i) {
            ASSERT_TRUE(r(i) >= 0.0f && r(i) <= 1.0f);
        }
    END_TEST

    TEST("Random normal generation")
        Tensor r = backend.randn(1000, 1, 1, 1);
        ASSERT_EQ(r.dim(0), 1000);
        // Check mean is roughly 0
        float sum = 0.0f;
        for (int i = 0; i < 1000; ++i) sum += r(i);
        float mean = sum / 1000.0f;
        ASSERT_TRUE(std::fabs(mean) < 0.2f); // rough check
    END_TEST

    TEST("Matrix multiply 2x3 * 3x1")
        // A = [[1,2,3],[4,5,6]] (2x3 in col-major)
        // Col-major: col0=[1,4], col1=[2,5], col2=[3,6]
        std::vector<float> aData = {1, 4, 2, 5, 3, 6};
        Tensor a(2, 3, 1, 1, aData);
        // B = [[1],[2],[3]] (3x1)
        std::vector<float> bData = {1, 2, 3};
        Tensor b(3, 1, 1, 1, bData);

        Tensor c = backend.matmul(a, b);
        ASSERT_EQ(c.dim(0), 2);
        ASSERT_EQ(c.dim(1), 1);
        // [1*1+2*2+3*3, 4*1+5*2+6*3] = [14, 32]
        ASSERT_NEAR(c(0, 0), 14.0f, 1e-5f);
        ASSERT_NEAR(c(1, 0), 32.0f, 1e-5f);
    END_TEST

    TEST("Batched matrix multiply")
        // Two 2x2 matrices multiplied with two 2x1 vectors, batched in dim 2
        // A[:,:,0] = [[1,2],[3,4]], A[:,:,1] = [[5,6],[7,8]]
        std::vector<float> aData = {1, 3, 2, 4, 5, 7, 6, 8};
        Tensor a(2, 2, 2, 1, aData);
        // B[:,:,0] = [[1],[0]], B[:,:,1] = [[0],[1]]
        std::vector<float> bData = {1, 0, 0, 1};
        Tensor b(2, 1, 2, 1, bData);

        Tensor c = backend.matmul(a, b);
        ASSERT_EQ(c.dim(0), 2);
        ASSERT_EQ(c.dim(1), 1);
        ASSERT_EQ(c.dim(2), 2);
        // batch 0: [[1,2],[3,4]] * [[1],[0]] = [[1],[3]]
        ASSERT_NEAR(c(0, 0, 0), 1.0f, 1e-5f);
        ASSERT_NEAR(c(1, 0, 0), 3.0f, 1e-5f);
        // batch 1: [[5,6],[7,8]] * [[0],[1]] = [[6],[8]]
        ASSERT_NEAR(c(0, 0, 1), 6.0f, 1e-5f);
        ASSERT_NEAR(c(1, 0, 1), 8.0f, 1e-5f);
    END_TEST

    TEST("Element-wise add")
        std::vector<float> aData = {1, 2, 3};
        std::vector<float> bData = {4, 5, 6};
        Tensor a(3, 1, 1, 1, aData);
        Tensor b(3, 1, 1, 1, bData);
        Tensor c = backend.add(a, b);
        ASSERT_NEAR(c(0), 5.0f, 1e-6f);
        ASSERT_NEAR(c(1), 7.0f, 1e-6f);
        ASSERT_NEAR(c(2), 9.0f, 1e-6f);
    END_TEST

    TEST("Element-wise multiply")
        std::vector<float> aData = {2, 3, 4};
        std::vector<float> bData = {5, 6, 7};
        Tensor a(3, 1, 1, 1, aData);
        Tensor b(3, 1, 1, 1, bData);
        Tensor c = backend.multiply(a, b);
        ASSERT_NEAR(c(0), 10.0f, 1e-6f);
        ASSERT_NEAR(c(1), 18.0f, 1e-6f);
        ASSERT_NEAR(c(2), 28.0f, 1e-6f);
    END_TEST

    TEST("Scale")
        std::vector<float> aData = {1, 2, 3};
        Tensor a(3, 1, 1, 1, aData);
        Tensor c = backend.scale(a, 2.0f);
        ASSERT_NEAR(c(0), 2.0f, 1e-6f);
        ASSERT_NEAR(c(1), 4.0f, 1e-6f);
        ASSERT_NEAR(c(2), 6.0f, 1e-6f);
    END_TEST

    TEST("Add scalar")
        std::vector<float> aData = {1, 2, 3};
        Tensor a(3, 1, 1, 1, aData);
        Tensor c = backend.add_scalar(a, 10.0f);
        ASSERT_NEAR(c(0), 11.0f, 1e-6f);
        ASSERT_NEAR(c(1), 12.0f, 1e-6f);
        ASSERT_NEAR(c(2), 13.0f, 1e-6f);
    END_TEST

    TEST("Subtract from scalar")
        std::vector<float> aData = {0.2f, 0.8f, 0.5f};
        Tensor a(3, 1, 1, 1, aData);
        Tensor c = backend.subtract_from_scalar(1.0f, a);
        ASSERT_NEAR(c(0), 0.8f, 1e-6f);
        ASSERT_NEAR(c(1), 0.2f, 1e-6f);
        ASSERT_NEAR(c(2), 0.5f, 1e-6f);
    END_TEST

    TEST("ReLU activation")
        std::vector<float> aData = {-2, -1, 0, 1, 2};
        Tensor a(5, 1, 1, 1, aData);
        Tensor c = backend.apply_activation(a, Activations::ReLU);
        ASSERT_NEAR(c(0), 0.0f, 1e-6f);
        ASSERT_NEAR(c(1), 0.0f, 1e-6f);
        ASSERT_NEAR(c(2), 0.0f, 1e-6f);
        ASSERT_NEAR(c(3), 1.0f, 1e-6f);
        ASSERT_NEAR(c(4), 2.0f, 1e-6f);
    END_TEST

    TEST("LeakyReLU activation")
        std::vector<float> aData = {-2, 0, 2};
        Tensor a(3, 1, 1, 1, aData);
        Tensor c = backend.apply_activation(a, Activations::LeakyReLU);
        ASSERT_NEAR(c(0), -0.2f, 1e-6f);
        ASSERT_NEAR(c(1), 0.0f, 1e-6f);
        ASSERT_NEAR(c(2), 2.0f, 1e-6f);
    END_TEST

    TEST("Sigmoid activation")
        std::vector<float> aData = {0.0f};
        Tensor a(1, 1, 1, 1, aData);
        Tensor c = backend.apply_activation(a, Activations::Sigmoid);
        ASSERT_NEAR(c(0), 0.5f, 1e-5f);
    END_TEST

    TEST("Tanh activation")
        std::vector<float> aData = {0.0f};
        Tensor a(1, 1, 1, 1, aData);
        Tensor c = backend.apply_activation(a, Activations::Tanh);
        ASSERT_NEAR(c(0), 0.0f, 1e-5f);
    END_TEST

    TEST("Linear activation (identity)")
        std::vector<float> aData = {-1, 0, 1};
        Tensor a(3, 1, 1, 1, aData);
        Tensor c = backend.apply_activation(a, Activations::Linear);
        ASSERT_NEAR(c(0), -1.0f, 1e-6f);
        ASSERT_NEAR(c(1), 0.0f, 1e-6f);
        ASSERT_NEAR(c(2), 1.0f, 1e-6f);
    END_TEST

    TEST("Tile operation")
        std::vector<float> aData = {1, 2};
        Tensor a(2, 1, 1, 1, aData);
        Tensor c = backend.tile(a, 1, 1, 3, 1);
        ASSERT_EQ(c.dim(0), 2);
        ASSERT_EQ(c.dim(2), 3);
        ASSERT_NEAR(c(0, 0, 0), 1.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 0), 2.0f, 1e-6f);
        ASSERT_NEAR(c(0, 0, 1), 1.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 1), 2.0f, 1e-6f);
        ASSERT_NEAR(c(0, 0, 2), 1.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 2), 2.0f, 1e-6f);
    END_TEST

    TEST("Lookup along dim 2")
        // Tensor (2, 1, 3): three 2x1 vectors
        std::vector<float> aData = {10, 20, 30, 40, 50, 60};
        Tensor a(2, 1, 3, 1, aData);
        // Lookup indices [2, 0]
        std::vector<int> indices = {2, 0};
        Tensor c = backend.lookup(a, indices, 2);
        ASSERT_EQ(c.dim(0), 2);
        ASSERT_EQ(c.dim(2), 2);
        // Index 2: [50, 60], Index 0: [10, 20]
        ASSERT_NEAR(c(0, 0, 0), 50.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 0), 60.0f, 1e-6f);
        ASSERT_NEAR(c(0, 0, 1), 10.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 1), 20.0f, 1e-6f);
    END_TEST

    TEST("Slice along dim 2")
        // Tensor (2, 1, 4): four 2x1 vectors
        std::vector<float> aData = {1, 2, 3, 4, 5, 6, 7, 8};
        Tensor a(2, 1, 4, 1, aData);
        // Slice [1, 2]
        Tensor c = backend.slice(a, 2, 1, 2);
        ASSERT_EQ(c.dim(2), 2);
        ASSERT_NEAR(c(0, 0, 0), 3.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 0), 4.0f, 1e-6f);
        ASSERT_NEAR(c(0, 0, 1), 5.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 1), 6.0f, 1e-6f);
    END_TEST

    TEST("Join along dim 2")
        std::vector<float> aData = {1, 2};
        std::vector<float> bData = {3, 4, 5, 6};
        Tensor a(2, 1, 1, 1, aData);
        Tensor b(2, 1, 2, 1, bData);
        Tensor c = backend.join(2, a, b);
        ASSERT_EQ(c.dim(2), 3);
        ASSERT_NEAR(c(0, 0, 0), 1.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 0), 2.0f, 1e-6f);
        ASSERT_NEAR(c(0, 0, 1), 3.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 1), 4.0f, 1e-6f);
        ASSERT_NEAR(c(0, 0, 2), 5.0f, 1e-6f);
        ASSERT_NEAR(c(1, 0, 2), 6.0f, 1e-6f);
    END_TEST

    TEST("Assign slice")
        Tensor dest(2, 1, 3, 1);
        std::vector<float> srcData = {10, 20};
        Tensor src(2, 1, 1, 1, srcData);
        backend.assign_slice(dest, src, 2, 1, 1);
        ASSERT_NEAR(dest(0, 0, 0), 0.0f, 1e-6f);
        ASSERT_NEAR(dest(0, 0, 1), 10.0f, 1e-6f);
        ASSERT_NEAR(dest(1, 0, 1), 20.0f, 1e-6f);
        ASSERT_NEAR(dest(0, 0, 2), 0.0f, 1e-6f);
    END_TEST

    TEST("Add to slice")
        std::vector<float> destData = {1, 2, 3, 4, 5, 6};
        Tensor dest(2, 1, 3, 1, destData);
        std::vector<float> srcData = {10, 20};
        Tensor src(2, 1, 1, 1, srcData);
        backend.add_to_slice(dest, src, 2, 1, 1);
        ASSERT_NEAR(dest(0, 0, 1), 13.0f, 1e-6f);
        ASSERT_NEAR(dest(1, 0, 1), 24.0f, 1e-6f);
    END_TEST

    TEST("Greater than")
        std::vector<float> aData = {0.3f, 0.5f, 0.7f, 0.9f};
        Tensor a(4, 1, 1, 1, aData);
        Tensor c = backend.greater_than(a, 0.5f);
        ASSERT_NEAR(c(0), 0.0f, 1e-6f);
        ASSERT_NEAR(c(1), 0.0f, 1e-6f);
        ASSERT_NEAR(c(2), 1.0f, 1e-6f);
        ASSERT_NEAR(c(3), 1.0f, 1e-6f);
    END_TEST

    TEST("Constant")
        Tensor c = backend.constant(3.14f, 2, 3, 1, 1);
        ASSERT_EQ(c.dim(0), 2);
        ASSERT_EQ(c.dim(1), 3);
        for (int i = 0; i < c.elements(); ++i) {
            ASSERT_NEAR(c.data()[i], 3.14f, 1e-6f);
        }
    END_TEST
}

// ===========================================================================
// Neural Network Tests
// ===========================================================================
void test_neural_network() {
    std::cout << "\n=== Neural Network Tests ===\n";

    TEST("Constructor with topology")
        std::vector<int> topology = {3, 4, 2};
        std::vector<Activations> activations = {Activations::ReLU, Activations::Sigmoid};
        NeuralNetwork nn(topology, activations, -1.0f, 1.0f, true, 5);

        ASSERT_EQ(nn.size(), 3);
        ASSERT_EQ(nn.networks(), 5);

        auto topo = nn.topology();
        ASSERT_EQ(topo.size(), 3u);
        ASSERT_EQ(topo[0], 3);
        ASSERT_EQ(topo[1], 4);
        ASSERT_EQ(topo[2], 2);
    END_TEST

    TEST("Feed forward shape check")
        std::vector<int> topology = {3, 4, 2};
        std::vector<Activations> activations = {Activations::Tanh, Activations::Tanh};
        NeuralNetwork nn(topology, activations, -1.0f, 1.0f, true, 10);

        // Input: (3 inputs, 1 col, 10 networks, 1 batch)
        std::vector<float> inputData(3 * 10, 0.5f);
        Tensor input(3, 1, 10, 1, inputData);
        Tensor output = nn.feed_forward(input);

        ASSERT_EQ(output.dim(0), 2);  // 2 output neurons
        ASSERT_EQ(output.dim(1), 1);
        ASSERT_EQ(output.dim(2), 10); // 10 networks
        ASSERT_EQ(output.dim(3), 1);
    END_TEST

    TEST("Feed forward output range with Tanh")
        std::vector<int> topology = {2, 3, 1};
        std::vector<Activations> activations = {Activations::Tanh, Activations::Tanh};
        NeuralNetwork nn(topology, activations, -1.0f, 1.0f, true, 1);

        std::vector<float> inputData = {0.5f, -0.5f};
        Tensor input(2, 1, 1, 1, inputData);
        Tensor output = nn.feed_forward(input);

        // Tanh output must be in [-1, 1]
        for (int i = 0; i < output.elements(); ++i) {
            ASSERT_TRUE(output.data()[i] >= -1.0f && output.data()[i] <= 1.0f);
        }
    END_TEST

    TEST("Feed forward single network")
        std::vector<int> topology = {3, 4, 2};
        std::vector<Activations> activations = {Activations::Tanh, Activations::Tanh};
        NeuralNetwork nn(topology, activations, -1.0f, 1.0f, true, 5);

        std::vector<float> inputData = {0.1f, 0.2f, 0.3f};
        Tensor input(3, 1, 1, 1, inputData);
        Tensor output = nn.feed_forward_single(input, 2);

        ASSERT_EQ(output.dim(0), 2);
        ASSERT_EQ(output.dim(1), 1);
    END_TEST

    TEST("Breed does not crash")
        std::vector<int> topology = {3, 4, 2};
        std::vector<Activations> activations = {Activations::Tanh, Activations::Tanh};
        NeuralNetwork nn(topology, activations, -1.0f, 1.0f, true, 20);

        std::vector<float> fitness(20, 0.0f);
        for (int i = 0; i < 20; ++i) fitness[i] = static_cast<float>(i);
        nn.breed(fitness, 3, -0.1f, 0.1f);

        // Verify network still works after breeding
        ASSERT_EQ(nn.networks(), 20);
        std::vector<float> inputData(3 * 20, 0.5f);
        Tensor input(3, 1, 20, 1, inputData);
        Tensor output = nn.feed_forward(input);
        ASSERT_EQ(output.dim(0), 2);
        ASSERT_EQ(output.dim(2), 20);
    END_TEST

    TEST("Save and load")
        std::vector<int> topology = {3, 4, 2};
        std::vector<Activations> activations = {Activations::Tanh, Activations::Sigmoid};
        NeuralNetwork nn(topology, activations, -1.0f, 1.0f, true, 5);

        // Get output before save
        std::vector<float> inputData(3 * 5, 0.5f);
        Tensor input(3, 1, 5, 1, inputData);
        Tensor outputBefore = nn.feed_forward(input);

        // Save
        std::string path = "/tmp/test_nn_save.json";
        bool saved = nn.save(path, 5);
        ASSERT_TRUE(saved);

        // Load into new network
        NeuralNetwork nn2(topology, activations, -1.0f, 1.0f, true, 5);
        bool loaded = nn2.load(path);
        ASSERT_TRUE(loaded);

        // Verify topology
        auto topo = nn2.topology();
        ASSERT_EQ(topo.size(), 3u);
        ASSERT_EQ(topo[0], 3);
        ASSERT_EQ(topo[1], 4);
        ASSERT_EQ(topo[2], 2);

        // Remove test file
        std::remove(path.c_str());
    END_TEST

    TEST("Bytes calculation")
        std::vector<int> topology = {3, 4, 2};
        std::vector<Activations> activations = {Activations::Tanh, Activations::Tanh};
        NeuralNetwork nn(topology, activations, -1.0f, 1.0f, true, 1);

        size_t expectedBytes =
            (4 * 3 * 1 + 4 * 1 * 1 + 2 * 4 * 1 + 2 * 1 * 1) * sizeof(float);
        ASSERT_EQ(nn.bytes(), expectedBytes);
    END_TEST
}

// ===========================================================================
// Backend Factory Tests
// ===========================================================================
void test_backend_factory() {
    std::cout << "\n=== Backend Factory Tests ===\n";

    TEST("Default backend is CPU (no ArrayFire in test env)")
        auto backend = BackendFactory::getBackend();
        ASSERT_TRUE(backend != nullptr);
        std::string name = backend->name();
        std::cout << "(backend=" << name << ") ";
        ASSERT_TRUE(name == "CPU" || name == "ArrayFire");
    END_TEST

    TEST("Backend name matches")
        ASSERT_TRUE(!BackendFactory::getBackendName().empty());
    END_TEST

    TEST("Set custom backend")
        auto cpu = std::make_shared<CPUBackend>();
        BackendFactory::setBackend(cpu);
        ASSERT_EQ(BackendFactory::getBackend()->name(), std::string("CPU"));
    END_TEST
}

// ===========================================================================
// Utility Tests
// ===========================================================================
void test_utility() {
    std::cout << "\n=== Utility Tests ===\n";

    TEST("find_top_n")
        std::vector<float> v = {1.0f, 5.0f, 3.0f, 4.0f, 2.0f};
        auto top = Utility::find_top_n(v, 3);
        ASSERT_EQ(top.size(), 3u);
        ASSERT_EQ(top[0], 1); // index of 5.0
        ASSERT_EQ(top[1], 3); // index of 4.0
        ASSERT_EQ(top[2], 2); // index of 3.0
    END_TEST

    TEST("mapVectorToIndex")
        std::vector<float> v = {0.1f, 0.9f, 0.5f};
        ASSERT_EQ(Utility::mapVectorToIndex(v), 1);
    END_TEST

    TEST("mapIndexToVector")
        auto v = Utility::mapIndexToVector(2, 4);
        ASSERT_EQ(v.size(), 4u);
        ASSERT_NEAR(v[0], 0.0f, 1e-6f);
        ASSERT_NEAR(v[1], 0.0f, 1e-6f);
        ASSERT_NEAR(v[2], 1.0f, 1e-6f);
        ASSERT_NEAR(v[3], 0.0f, 1e-6f);
    END_TEST

    TEST("sizeToString")
        std::string s = Utility::sizeToString(1500);
        ASSERT_TRUE(!s.empty());
    END_TEST

    TEST("Activations backward compatibility")
        // Verify Utility::Activations still works
        Utility::Activations act = Utility::Activations::ReLU;
        ASSERT_EQ(static_cast<int>(act), 0);
    END_TEST
}

// ===========================================================================
// Main
// ===========================================================================
int main() {
    std::cout << "======================================\n";
    std::cout << "IntelliDrive Compute Tests\n";
    std::cout << "======================================\n";

    test_tensor();
    test_cpu_backend();
    test_neural_network();
    test_backend_factory();
    test_utility();

    std::cout << "\n======================================\n";
    std::cout << "Results: " << tests_passed << " passed, " << tests_failed << " failed\n";
    std::cout << "======================================\n";

    return tests_failed > 0 ? 1 : 0;
}
