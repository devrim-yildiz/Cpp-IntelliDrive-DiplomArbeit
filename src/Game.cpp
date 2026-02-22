//
// Created by Voric and tobisdev on 11/08/2024.
//

#include "../include/Game.h"
#include "../include/MenuState.h"
#include "../include/ResourceManager.h"
#include "compute/BackendFactory.h"

Game::Game() : window(sf::VideoMode(1920, 1080), "IntelliDrive", sf::Style::Fullscreen)
{
    // Load Variables
    VariableManager::loadFromJson(VariableManager::getPathToConfig());
    // Resource Manager
    ResourceManager::loadAllKnownResources();

    // Apply VSync and FPS limit
    window.setVerticalSyncEnabled(VariableManager::getVSync());
    window.setFramerateLimit(VariableManager::getFpsLimit());

    car = {};
    BackendFactory::create();
    loadCarData("resources/config/cars.csv");
    std::cout << "[DEBUG] Loading tiles from CSV\n";
    car.applyData(cars[0]);
    pushState(std::make_shared<MenuState>());

    initializeText(fpsText, 100.f, 100.f);
    initializeText(avgText, 100.f, 140.f);
    initializeText(lowsText, 100.f, 170.f);

    window.setFramerateLimit(144);
}

Game::~Game() {
    std::cout << "Game is exiting..." << std::endl;
}

void Game::run() {
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (window.isOpen()) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - previousTime);
        dt = elapsedTime.count() / 1e9;
        previousTime = currentTime;

        if (auto currentState = getCurrentState()) {
            currentState->handleInput(*this);
            currentState->update(*this);
            window.clear();
            currentState->render(*this);
            //calculateAndDisplayFPS();
            window.display();
        }
    }
}

void Game::pushState(std::shared_ptr<State> state) {
    states.push_back(state);
}

void Game::popState() {
    states.pop_back();
}

void Game::changeState(std::shared_ptr<State> state){

    if (!states.empty()) {
        states.pop_back();
    }
    states.push_back(state);
}

std::shared_ptr<State> Game::getCurrentState() {
    return states.empty() ? nullptr : states.back();
}

void Game::loadCarData(std::string path) {
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        std::cerr << "ERROR: Error opening file!" << "\n";
        return;
    }

    std::string line;
    int row = 0;

    while (getline(inputFile, line)) {
        if (row != 0) {
            carData data;
            parseCarDataLine(line, data);
            cars.emplace_back(data);
        }
        row++;
    }
    inputFile.close();
}

void Game::calculateAndDisplayFPS() {
    if (frameTimes.size() >= maxFrameSamples) {
        frameTimes.pop_front();
    }
    frameTimes.push_back(dt);

    float avgFPS = calculateAverageFPS();
    float onePercentLowsFPS = calculateOnePercentLowsFPS();

    updateText(fpsText, "FPS: ", 1.0f / dt);
    updateText(avgText, "Avg FPS: ", avgFPS);
    updateText(lowsText, "1% Lows: ", onePercentLowsFPS);

    window.draw(fpsText);
    window.draw(avgText);
    window.draw(lowsText);
}

void Game::initializeText(sf::Text& text, float x, float y) {
    font = ResourceManager::getFont("Rubik-Regular");

    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(x, y);
}

void Game::parseCarDataLine(const std::string& line, carData& data) {
    std::stringstream ss(line);
    std::string token;
    int entry = 0;

    while (getline(ss, token, ',')) {
        if (!token.empty()) {
            switch (entry) {
            case 0:
                data.name = token;
                break;
            case 1:
                ResourceManager::loadTexture("CarTexture_" + data.name, token);
                data.carTexture = ResourceManager::getTexture("CarTexture_" + data.name);
                break;
            case 2:
                data.MaxSpeed = std::stof(token);
                break;
            case 3:
                data.Handling = std::stof(token);
                break;
            case 4:
                data.Acceleration = std::stof(token);
                break;
            case 5:
                data.weight = std::stof(token);
                break;
            case 6:
                data.power = std::stof(token);
                break;
            case 7:
                data.torque = std::stof(token);
                break;
            case 8:
                data.driveType = token;
                break;
            case 9:
                ResourceManager::loadTexture("LogoTexture_" + data.name, token);
                data.logoTexture = ResourceManager::getTexture("LogoTexture_" + data.name);
                break;
            }
        }
        entry++;
    }
}

float Game::calculateAverageFPS() const {
    float sumFrameTimes = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f);
    return frameTimes.empty() ? 0.0f : 1.0f / (sumFrameTimes / frameTimes.size());
}

float Game::calculateOnePercentLowsFPS() const {
    if (frameTimes.empty()) return 0.0f;

    std::vector<float> sortedFrameTimes(frameTimes.begin(), frameTimes.end());
    std::sort(sortedFrameTimes.begin(), sortedFrameTimes.end());

    size_t onePercentIndex = static_cast<size_t>(sortedFrameTimes.size() * 0.01f);
    float onePercentLowTime = std::accumulate(sortedFrameTimes.begin(), sortedFrameTimes.begin() + onePercentIndex, 0.0f) / onePercentIndex;

    return 1.0f / onePercentLowTime;
}

void Game::updateText(sf::Text& text, const std::string& label, float value) {
    text.setString(label + std::to_string(static_cast<float>(value)));
}
