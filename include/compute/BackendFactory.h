//
// BackendFactory - creates and manages the active compute backend.
// Detects ArrayFire availability at runtime and falls back to CPU.
//

#ifndef INTELLIDRIVE_BACKENDFACTORY_H
#define INTELLIDRIVE_BACKENDFACTORY_H

#include "ComputeBackend.h"
#include <memory>
#include <string>

class BackendFactory {
public:
    // Create and initialize the best available backend
    static std::shared_ptr<ComputeBackend> create();

    // Get the current active backend (creates one if none exists)
    static std::shared_ptr<ComputeBackend> getBackend();

    // Override the active backend
    static void setBackend(std::shared_ptr<ComputeBackend> backend);

    // Check if ArrayFire is available at compile time
    static bool isArrayFireAvailable();

    // Get the name of the active backend
    static std::string getBackendName();

private:
    static std::shared_ptr<ComputeBackend> _instance;
};

#endif // INTELLIDRIVE_BACKENDFACTORY_H
