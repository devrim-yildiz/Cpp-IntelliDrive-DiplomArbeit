//
// Created by Voric and tobisdev on 11/08/2024.
//

#include "../include/Car.h"
#include <iostream>
#include <cmath>

Car::Car()
    : current_position(0.0f, 0.0f),
      velocity(0.0f, 0.0f),
      rotation_angle(0.0f),
      angular_velocity(0.0f),
      acceleration(0.0f),
      angular_acceleration(0.0f),
      friction_coefficient(0.9f), // Adjust between 0 (no friction) and 1 (full stop instantly)
      acceleration_constant(10.1f),
      angular_acceleration_constant(0.1f),
      max_speed(0.0f),
      maxSpeedValue(5.0f),
      handlingValue(5.0f),
      accelerationValue(5.0f)
{

}

void Car::setAcceleration(float value) {
    acceleration = value;
}

void Car::setAngularAcceleration(float value) {
    angular_acceleration = value;
}

float Car::getAccelerationConstant() const {
    return acceleration_constant;
}

float Car::getAngularAccelerationConstant() const {
    return angular_acceleration_constant;
}


void Car::resetVelocity() {
    velocity.x = 0.0f;
    velocity.y = 0.0f;
}

void Car::resetAngularAcceleration() {
    angular_acceleration = 0.0f;
    acceleration = 0.0f;
    angular_velocity = 0.0f;
    angular_damping = 0.0f;
}

void Car::update(float dt) {

    // Calculating the current angle in rad
    float radian_angle = rotation_angle * (PI / 180.0f);
    sf::Vector2f forward_direction(sinf(radian_angle), -cosf(radian_angle));
    // Velocity calculation based on acceleration and dt
    velocity += forward_direction * acceleration * dt;

    // Velocity reduction by friction
    velocity -= velocity * friction_coefficient * dt;

    // Speed
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    // Applying speed boundaries to the velocity
    if (speed > max_speed) {
        velocity = (velocity / speed) * max_speed;
    }
    // This line was previously: velocity = (max_speed > max_speed) ? ... which was always false
    // Removed as the speed clamping is already handled in the if statement above

    // Setting the new position
    current_position += velocity * dt;

    // Detection if car moves forward or backwards
    float forwardFactor = velocity.x * forward_direction.x + velocity.y * forward_direction.y;
    float backwardDistance = 0.0f;
    if (forwardFactor < 0.0f) {
        float backwardSpeed = -forwardFactor;
        backwardDistance = backwardSpeed * dt;
    }
    distanceMovedBackwards += backwardDistance;

    angular_velocity += angular_acceleration * speed * VariableManager::getRotationalSpeedMultiplier() * dt;
    float angular_velocity_new = angular_velocity * VariableManager::getAngularDampingMultiplier();

    if (angular_velocity > 0 && angular_velocity_new < 0 || angular_velocity < 0 && angular_velocity_new > 0) {
        //angular_velocity = 0; // (optional, aktuell auskommentiert)
    } else {
        angular_velocity = angular_velocity_new;
    }

    rotation_angle += angular_velocity * dt;
    distanceRotated += std::abs(angular_velocity * dt);

    // Angle should be between 0 and 360°
    if (rotation_angle >= 360.0f) {
        rotation_angle -= 360.0f;
    } else if (rotation_angle < 0.0f) {
        rotation_angle += 360.0f;
    }

    carSprite.setPosition(current_position);
    carSprite.setRotation(rotation_angle);
    previous_position = current_position;
}

void Car::resetRotationAngle() {
    rotation_angle = 0.0f;
}

void Car::render(sf::RenderWindow& window) {
    window.draw(carSprite);
    carSprite.setScale(0.35f, 0.35f);
}

sf::FloatRect Car::getBounds() const {
    return carSprite.getGlobalBounds();
}

void Car::applyData(carData &data) {
    carSprite = {};
    carSprite.setTexture(data.carTexture);

    maxSpeedValue = static_cast<float>(data.MaxSpeed);
    handlingValue = static_cast<float>(data.Handling);
    accelerationValue = static_cast<float>(data.Acceleration);

    acceleration_constant = accelerationValue * (VariableManager::getMaxAccelerationConstant() / 10.0f);
    angular_acceleration_constant = handlingValue * (VariableManager::getMaxAngularAccelerationConstant() / 10.0f);
    max_speed = maxSpeedValue * (VariableManager::getMaxSpeed() / 10.0f);
}

float Car::getRotationAngle() const {
    return rotation_angle;
}

sf::Vector2f Car::getCurrentPosition() const {
    return current_position;
}

void Car::setPreviousPosition(const sf::Vector2f& position) {
    previous_position = position;
}

void Car::setCurrentPosition(const sf::Vector2f& position) {
    current_position = position;
}

void Car::setRotationAngle(float angle) {
    rotation_angle = angle;
}

sf::Sprite& Car::getCarSprite() {
    return carSprite;
}
void Car::setPosition(const sf::Vector2f& position) {
    carSprite.setPosition(position);
}