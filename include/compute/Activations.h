//
// Activation function enum - decoupled from any compute backend.
//

#ifndef INTELLIDRIVE_ACTIVATIONS_H
#define INTELLIDRIVE_ACTIVATIONS_H

enum class Activations : int {
    ReLU,
    LeakyReLU,
    Sigmoid,
    Linear,
    Tanh
};

#endif // INTELLIDRIVE_ACTIVATIONS_H
