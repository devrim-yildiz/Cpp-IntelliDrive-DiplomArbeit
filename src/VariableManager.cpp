//
// Created by Tobias on 14.01.2025.
//

#include "VariableManager.h"// Physics
float VariableManager::MAX_ACCELERATION_CONSTANT = 800.0f; // Units: pixels per second squared
float VariableManager::MAX_ANGULAR_ACCELERATION_CONSTANT = 200.0f; // Units: degrees per second squared
float VariableManager::MAX_SPEED = 450.0f; // Units: pixels per second
float VariableManager::ROTATIONAL_SPEED_MULTIPLIER = 0.00425;
float VariableManager::ANGULAR_DAMPING_MULTIPLIER = 0.99;

// Car
int VariableManager::SELECTED_CAR_INDEX = 0;

// World
float VariableManager::TILE_SIZE = 64;

// Resources
std::string VariableManager::FONT_RUBIK_REGULAR = "resources/Fonts/Rubik-Regular.ttf";
std::string VariableManager::FONT_UPHEAVAL_PRO = "resources/Fonts/UpheavalPRO.ttf";
std::string VariableManager::FONT_MENU_TITLE = "resources/Fonts/MenuTitle-Font.ttf";

std::string VariableManager::PATH_TO_CONFIG = "resources/config/config.json";
std::string VariableManager::PATH_TO_TILE_CONFIG = "resources/Tiles/Tiles.csv";
std::string VariableManager::PATH_TO_CAR_CONFIG = "resources/config/cars.csv";
std::string VariableManager::PATH_TO_LEVELS = "resources/Levels/";
std::string VariableManager::PATH_TO_BACKGROUNDS = "resources/Backgrounds/background";

std::string VariableManager::IMAGE_CAR_CHOOSING_ARROW_LEFT = "resources/GUI/arrowleft.png";
std::string VariableManager::IMAGE_CAR_CHOOSING_ARROW_RIGHT = "resources/GUI/arrowright.png";
std::string VariableManager::IMAGE_GAME_BACKGROUND = PATH_TO_BACKGROUNDS + "5.png";
std::string VariableManager::IMAGE_BACKGROUND_CAR_CHOOSING = "resources/Backgrounds/carchoosingstatebackground.png";

// AI
int VariableManager::RAY_AMOUNT = 8;
float VariableManager::AI_FOV = 270.0f;
int VariableManager::NETWORKS_AMOUNT = 200;
float VariableManager::AI_INITIAL_RANDOM_VALUES_MAX = 1.6f;
bool VariableManager::AI_INITIAL_RANDOM_VALUES_UNIFORM = true;
std::vector<int> VariableManager::AI_TOPOLOGY = {RAY_AMOUNT, 6, OUTPUT_NEURONS};
std::vector<Activations> VariableManager::AI_ACTIVATIONS = {
        Activations::Tanh,
        Activations::Tanh
};

// Evolutionary Algorithm
int VariableManager::AI_WINNERS = 5;
float VariableManager::CHECKPOINT_POINTS = 25;
float VariableManager::ROTATION_PENALTY = 35;
float VariableManager::CHECKPOINT_RADIUS = 140.0f;
float VariableManager::BACKWARDS_MOVEMENT_PENALTY = 5;
float VariableManager::RESTART_ON_DEAD_PERCENTAGE = 0.995;
float VariableManager::MUTATION_INDEX = 0.01f;

// DEBUG - RENDER
bool VariableManager::SHOW_COLLIDERS = false;
bool VariableManager::SHOW_CHECKPOINTS = false;
bool VariableManager::SHOW_RAYS = false;
bool VariableManager::SHOW_CARS_FIRST = false;

// Game
bool VariableManager::AI_MODE = false;

// Settings
std::string VariableManager::UNITS = "Metric";
std::string VariableManager::DISPLAY_MODE = "fullscreen";
std::string VariableManager::RESOLUTION = "1920x1080";
bool VariableManager::VSYNC = false;
int VariableManager::FPS_LIMIT = 120;
bool VariableManager::FPS_COUNTER = false;
std::string VariableManager::METRICS_MODE = "simple";

// Save function
void VariableManager::saveToJson(const std::string& filePath) {
    nlohmann::json jsonData;
    // Physics
    jsonData["MAX_ACCELERATION_CONSTANT"] = MAX_ACCELERATION_CONSTANT;
    jsonData["MAX_ANGULAR_ACCELERATION_CONSTANT"] = MAX_ANGULAR_ACCELERATION_CONSTANT;
    jsonData["MAX_SPEED"] = MAX_SPEED;
    jsonData["ROTATIONAL_SPEED_MULTIPLIER"] = ROTATIONAL_SPEED_MULTIPLIER;
    jsonData["ANGULAR_DAMPING_MULTIPLIER"] = ANGULAR_DAMPING_MULTIPLIER;
    jsonData["SELECTED_CAR_INDEX"] = SELECTED_CAR_INDEX;

    // World
    jsonData["TILE_SIZE"] = TILE_SIZE;

    // Resources
    jsonData["FONT_RUBIK_REGULAR"] = FONT_RUBIK_REGULAR;
    jsonData["FONT_UPHEAVAL_PRO"] = FONT_UPHEAVAL_PRO;
    jsonData["FONT_MENU_TITLE"] = FONT_MENU_TITLE;

    jsonData["PATH_TO_CONFIG"] = PATH_TO_CONFIG;
    jsonData["PATH_TO_TILE_CONFIG"] = PATH_TO_TILE_CONFIG;
    jsonData["PATH_TO_CAR_CONFIG"] = PATH_TO_CAR_CONFIG;
    jsonData["PATH_TO_LEVELS"] = PATH_TO_LEVELS;
    jsonData["PATH_TO_BACKGROUNDS"] = PATH_TO_BACKGROUNDS;

    jsonData["IMAGE_CAR_CHOOSING_ARROW_LEFT"] = IMAGE_CAR_CHOOSING_ARROW_LEFT;
    jsonData["IMAGE_CAR_CHOOSING_ARROW_RIGHT"] = IMAGE_CAR_CHOOSING_ARROW_RIGHT;
    jsonData["IMAGE_GAME_BACKGROUND"] = IMAGE_GAME_BACKGROUND;
    jsonData["IMAGE_BACKGROUND_CAR_CHOOSING"] = IMAGE_BACKGROUND_CAR_CHOOSING;

    // AI
    jsonData["RAY_AMOUNT"] = RAY_AMOUNT;
    jsonData["AI_FOV"] = AI_FOV;
    jsonData["NETWORKS_AMOUNT"] = NETWORKS_AMOUNT;
    jsonData["AI_TOPOLOGY"] = AI_TOPOLOGY;
    jsonData["AI_INITIAL_RANDOM_VALUES_MAX"] = AI_INITIAL_RANDOM_VALUES_MAX;
    jsonData["AI_INITIAL_RANDOM_VALUES_UNIFORM"] = AI_INITIAL_RANDOM_VALUES_UNIFORM;

    // Evolutionary Algorithm
    jsonData["AI_WINNERS"] = AI_WINNERS;
    jsonData["CHECKPOINT_POINTS"] = CHECKPOINT_POINTS;
    jsonData["CHECKPOINT_RADIUS"] = CHECKPOINT_RADIUS;
    jsonData["ROTATION_PENALTY"] = ROTATION_PENALTY;
    jsonData["BACKWARDS_MOVEMENT_PENALTY"] = BACKWARDS_MOVEMENT_PENALTY;
    jsonData["RESTART_ON_DEAD_PERCENTAGE"] = RESTART_ON_DEAD_PERCENTAGE;
    jsonData["MUTATION_INDEX"] = MUTATION_INDEX;

    // Debug - Render
    jsonData["SHOW_COLLIDERS"] = SHOW_COLLIDERS;
    jsonData["SHOW_CHECKPOINTS"] = SHOW_CHECKPOINTS;
    jsonData["SHOW_RAYS"] = SHOW_RAYS;
    jsonData["SHOW_CARS_FIRST"] = SHOW_CARS_FIRST;

    // Game
    jsonData["AI_MODE"] = AI_MODE;

    // Settings
    jsonData["UNITS"] = UNITS;
    jsonData["DISPLAY_MODE"] = DISPLAY_MODE;
    jsonData["RESOLUTION"] = RESOLUTION;
    jsonData["VSYNC"] = VSYNC;
    jsonData["FPS_LIMIT"] = FPS_LIMIT;
    jsonData["FPS_COUNTER"] = FPS_COUNTER;
    jsonData["METRICS_MODE"] = METRICS_MODE;

    std::ofstream file(filePath);
    file << jsonData.dump(4);
}

// Load function
void VariableManager::loadFromJson(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) return;

    nlohmann::json jsonData;
    file >> jsonData;

    // Physics
    MAX_ACCELERATION_CONSTANT = jsonData.value("MAX_ACCELERATION_CONSTANT", MAX_ACCELERATION_CONSTANT);
    MAX_ANGULAR_ACCELERATION_CONSTANT = jsonData.value("MAX_ANGULAR_ACCELERATION_CONSTANT", MAX_ANGULAR_ACCELERATION_CONSTANT);
    MAX_SPEED = jsonData.value("MAX_SPEED", MAX_SPEED);
    ROTATIONAL_SPEED_MULTIPLIER = jsonData.value("ROTATIONAL_SPEED_MULTIPLIER", ROTATIONAL_SPEED_MULTIPLIER);
    ANGULAR_DAMPING_MULTIPLIER = jsonData.value("ANGULAR_DAMPING_MULTIPLIER", ANGULAR_DAMPING_MULTIPLIER);

    // Car
    SELECTED_CAR_INDEX = jsonData.value("SELECTED_CAR_INDEX", SELECTED_CAR_INDEX);

    // World
    TILE_SIZE = jsonData.value("TILE_SIZE", TILE_SIZE);

    // Resources
    FONT_RUBIK_REGULAR = jsonData.value("FONT_RUBIK_REGULAR", FONT_RUBIK_REGULAR);
    FONT_UPHEAVAL_PRO = jsonData.value("FONT_UPHEAVAL_PRO", FONT_UPHEAVAL_PRO);
    FONT_MENU_TITLE = jsonData.value("FONT_MENU_TITLE", FONT_MENU_TITLE);

    PATH_TO_CONFIG = jsonData.value("PATH_TO_CONFIG", PATH_TO_CONFIG);
    PATH_TO_TILE_CONFIG = jsonData.value("PATH_TO_TILE_CONFIG", PATH_TO_TILE_CONFIG);
    PATH_TO_CAR_CONFIG = jsonData.value("PATH_TO_CAR_CONFIG", PATH_TO_CAR_CONFIG);
    PATH_TO_LEVELS = jsonData.value("PATH_TO_LEVELS", PATH_TO_LEVELS);
    PATH_TO_BACKGROUNDS = jsonData.value("PATH_TO_BACKGROUNDS", PATH_TO_BACKGROUNDS);

    IMAGE_CAR_CHOOSING_ARROW_LEFT = jsonData.value("IMAGE_CAR_CHOOSING_ARROW_LEFT", IMAGE_CAR_CHOOSING_ARROW_LEFT);
    IMAGE_CAR_CHOOSING_ARROW_RIGHT = jsonData.value("IMAGE_CAR_CHOOSING_ARROW_RIGHT", IMAGE_CAR_CHOOSING_ARROW_RIGHT);
    IMAGE_GAME_BACKGROUND = jsonData.value("IMAGE_GAME_BACKGROUND", IMAGE_GAME_BACKGROUND);
    IMAGE_BACKGROUND_CAR_CHOOSING = jsonData.value("IMAGE_BACKGROUND_CAR_CHOOSING", IMAGE_BACKGROUND_CAR_CHOOSING);

    // AI
    RAY_AMOUNT = jsonData.value("RAY_AMOUNT", RAY_AMOUNT);
    AI_FOV = jsonData.value("AI_FOV", AI_FOV);
    NETWORKS_AMOUNT = jsonData.value("NETWORKS_AMOUNT", NETWORKS_AMOUNT);
    AI_TOPOLOGY = jsonData.value("AI_TOPOLOGY", AI_TOPOLOGY);
    AI_INITIAL_RANDOM_VALUES_MAX = jsonData.value("AI_INITIAL_RANDOM_VALUES_MAX", AI_INITIAL_RANDOM_VALUES_MAX);
    AI_INITIAL_RANDOM_VALUES_UNIFORM = jsonData.value("AI_INITIAL_RANDOM_VALUES_UNIFORM",AI_INITIAL_RANDOM_VALUES_UNIFORM);

    // Evolutionary Algorithm
    AI_WINNERS = jsonData.value("AI_WINNERS", AI_WINNERS);
    CHECKPOINT_POINTS = jsonData.value("CHECKPOINT_POINTS", CHECKPOINT_POINTS);
    CHECKPOINT_RADIUS = jsonData.value("CHECKPOINT_RADIUS", CHECKPOINT_RADIUS);
    ROTATION_PENALTY = jsonData.value("ROTATION_PENALTY", ROTATION_PENALTY);
    BACKWARDS_MOVEMENT_PENALTY = jsonData.value("BACKWARDS_MOVEMENT_PENALTY", BACKWARDS_MOVEMENT_PENALTY);
    RESTART_ON_DEAD_PERCENTAGE = jsonData.value("RESTART_ON_DEAD_PERCENTAGE", RESTART_ON_DEAD_PERCENTAGE);
    MUTATION_INDEX = jsonData.value("MUTATION_INDEX", MUTATION_INDEX);

    // Debug - Render
    SHOW_COLLIDERS = jsonData.value("SHOW_COLLIDERS", SHOW_COLLIDERS);
    SHOW_CHECKPOINTS = jsonData.value("SHOW_CHECKPOINTS", SHOW_CHECKPOINTS);
    SHOW_RAYS = jsonData.value("SHOW_RAYS", SHOW_RAYS);
    SHOW_CARS_FIRST = jsonData.value("SHOW_CARS_PERCENTAGE", SHOW_CARS_FIRST);

    // Game
    if(jsonData.contains("AI_MODE")) AI_MODE = jsonData["AI_MODE"].get<bool>();

    // Settings
    if(jsonData.contains("UNITS")) UNITS = jsonData["UNITS"].get<std::string>();
    if(jsonData.contains("DISPLAY_MODE")) DISPLAY_MODE = jsonData["DISPLAY_MODE"].get<std::string>();
    if(jsonData.contains("RESOLUTION")) RESOLUTION = jsonData["RESOLUTION"].get<std::string>();
    if(jsonData.contains("VSYNC")) VSYNC = jsonData["VSYNC"].get<bool>();
    if(jsonData.contains("FPS_LIMIT")) FPS_LIMIT = jsonData["FPS_LIMIT"].get<int>();
    if(jsonData.contains("FPS_COUNTER")) FPS_COUNTER = jsonData["FPS_COUNTER"].get<bool>();
    if(jsonData.contains("METRICS_MODE")) METRICS_MODE = jsonData["METRICS_MODE"].get<std::string>();
}