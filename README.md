# Cpp-IntelliDrive-DiplomArbeit

This is the HTL Salzburg Diplomarbeit created by Devrim Yildiz and Tobias Huber. The Diplomarbeit is about a Generative AI, which learns driving.

## Table of Contents
- [Project Description](#project-description)
- [Features](#features)
- [Technologies Used](#technologies-used)
- [Architecture](#architecture)
- [Installation](#installation)
  - [Prerequisites](#prerequisites)
  - [macOS Setup](#macos-setup)
  - [Windows Setup](#windows-setup)
  - [Linux Setup](#linux-setup)
- [Backend Selection](#backend-selection)
- [Running Tests](#running-tests)
- [Usage](#usage)
- [Contributing](#contributing)
- [License](#license)
- [Screenshots](#screenshots)

## Project Description
Cpp-IntelliDrive-DiplomArbeit is a project aimed at developing a Generative AI that learns how to drive. Neural networks are trained using genetic algorithms to navigate a track, with support for both CPU and optional GPU-accelerated computation.

## Features
- **Generative AI**: Uses neural networks with genetic algorithms to learn and adapt driving behavior.
- **Car Customization**: Ability to customize car attributes such as speed, handling, and acceleration.
- **Multiple States**: Includes different states such as MenuState and CarChoosingState for a structured flow.
- **Graphical Interface**: Utilizes SFML for rendering graphics and handling user input.
- **Compute Backend Abstraction**: Pluggable backend system with CPU (default) and optional ArrayFire GPU support.
- **Cross-Platform**: Builds on macOS, Windows, and Linux.

## Technologies Used
- **C++17**: The core programming language used for development.
- **CMake**: Build system for managing the project's build process (minimum version 3.16).
- **SFML**: Simple and Fast Multimedia Library used for graphics rendering and handling user input.
- **nlohmann/json**: JSON library for configuration and neural network serialization.
- **ArrayFire** (optional): GPU-accelerated compute backend for neural network operations.

## Architecture

The project uses a clean abstraction layer for compute operations:

```
include/compute/
├── Activations.h       # Activation function enum (ReLU, Sigmoid, Tanh, etc.)
├── Tensor.h            # 4D tensor class (column-major, CPU-backed)
├── ComputeBackend.h    # Abstract interface for compute operations
├── CPUBackend.h        # Pure CPU implementation (default)
└── BackendFactory.h    # Runtime backend selection and management

vendors/ai/
├── NeuralNetwork/      # Neural network using Tensor + ComputeBackend
└── Utility/            # Pure CPU utility functions
```

**Key design decisions:**
- `Tensor` stores data as a flat `std::vector<float>` in column-major order (compatible with ArrayFire's memory layout).
- `ComputeBackend` is a pure virtual interface providing matrix operations, activations, and array manipulation.
- `CPUBackend` implements all operations using standard C++ (no external dependencies).
- `BackendFactory` detects available backends at compile time and selects the best one.
- No part of the game logic directly depends on ArrayFire.

## Installation

### Prerequisites
- CMake 3.16 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- Git (with submodule support)

### macOS Setup

```bash
# Install dependencies via Homebrew
brew install cmake sfml

# Clone the repository with submodules
git clone --recursive https://github.com/vor1c/Cpp-IntelliDrive-DiplomArbeit.git
cd Cpp-IntelliDrive-DiplomArbeit

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

For GPU support (optional):
```bash
brew install arrayfire
```

### Windows Setup

```bash
# Clone the repository with submodules
git clone --recursive https://github.com/vor1c/Cpp-IntelliDrive-DiplomArbeit.git
cd Cpp-IntelliDrive-DiplomArbeit

# Generate Visual Studio solution
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

For GPU support, install [ArrayFire](https://arrayfire.com/download/) and set `AF_PATH` environment variable.

### Linux Setup

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt update
sudo apt install -y cmake g++ libudev-dev libopenal-dev libvorbis-dev \
    libflac-dev libxrandr-dev libfreetype6-dev libgl1-mesa-dev \
    libxcursor-dev libxi-dev

# Clone and build
git clone --recursive https://github.com/vor1c/Cpp-IntelliDrive-DiplomArbeit.git
cd Cpp-IntelliDrive-DiplomArbeit
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

For GPU support:
```bash
sudo apt install arrayfire  # or build from source
```

## Backend Selection

The compute backend is selected automatically at **compile time**:

| Condition | Backend Used |
|-----------|-------------|
| ArrayFire found by CMake | ArrayFire (GPU) |
| ArrayFire not found | CPU (default) |

CMake will print which backend is being used:
```
-- ArrayFire found - building with GPU backend support
```
or:
```
-- ArrayFire NOT found - building with CPU backend only
```

To force CPU-only build even when ArrayFire is installed, you can configure without ArrayFire:
```bash
cmake .. -DCMAKE_DISABLE_FIND_PACKAGE_ArrayFire=TRUE
```

The CPU backend provides identical functionality to the ArrayFire backend - all neural network operations (forward propagation, genetic algorithm breeding, save/load) work the same way.

## Running Tests

```bash
cd build
cmake --build . --target IntelliDriveTests
ctest
# or directly:
./bin/IntelliDriveTests
```

Tests cover:
- Tensor creation and operations
- CPU backend (matmul, activations, array manipulation)
- Neural network (forward pass, breeding, save/load)
- Backend factory and switching

## Usage
After building the project, you can run the executable to start the AI learning process:

```bash
./bin/IntelliDrive
```

## Contributing
We welcome contributions to the project. Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch with the corresponding name.
4. Make your changes and commit them to your branch.
5. Open a pull request with a detailed description of your changes.

## License
This project is licensed under the MIT License. See the [LICENSE](https://github.com/vor1c/Cpp-IntelliDrive-DiplomArbeit/blob/main/LICENSE) file for more details.

## Screenshots
![Main Menu](/resources/Screenshots/Screen1.png)
![Car Select 1](/resources/Screenshots/Screen2.png)
![Car Select 2](/resources/Screenshots/Screen3.png)
