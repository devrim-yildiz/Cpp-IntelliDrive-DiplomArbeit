//
// BackendFactory - creates and manages the active compute backend.
//

#include "compute/BackendFactory.h"
#include "compute/CPUBackend.h"
#include <iostream>

#ifdef HAS_ARRAYFIRE
#include "compute/ArrayFireBackend.h"
#endif

std::shared_ptr<ComputeBackend> BackendFactory::_instance = nullptr;

std::shared_ptr<ComputeBackend> BackendFactory::create() {
    std::shared_ptr<ComputeBackend> backend;

#ifdef HAS_ARRAYFIRE
    try {
        backend = std::make_shared<ArrayFireBackend>();
        backend->setup();
        std::cout << "Using ArrayFire backend.\n";
    } catch (...) {
        std::cout << "ArrayFire initialization failed. Falling back to CPU backend.\n";
        backend = std::make_shared<CPUBackend>();
        backend->setup();
    }
#else
    backend = std::make_shared<CPUBackend>();
    backend->setup();
#endif

    _instance = backend;
    return backend;
}

std::shared_ptr<ComputeBackend> BackendFactory::getBackend() {
    if (!_instance) {
        create();
    }
    return _instance;
}

void BackendFactory::setBackend(std::shared_ptr<ComputeBackend> backend) {
    _instance = backend;
}

bool BackendFactory::isArrayFireAvailable() {
#ifdef HAS_ARRAYFIRE
    return true;
#else
    return false;
#endif
}

std::string BackendFactory::getBackendName() {
    if (_instance) {
        return _instance->name();
    }
    return "None";
}
