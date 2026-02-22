//
// Created by Tobias on 14.01.2025.
//

#ifndef INTELLIDRIVE_VARIABLEMANAGER_H
#define INTELLIDRIVE_VARIABLEMANAGER_H

// Includes
#include "Car.h"
#include "../vendors/json/json.hpp"
#include "compute/Activations.h"
#include <unordered_map>
#include "Tile.h"
#include <fstream>
#include <vector>

// Definitions
#define OUTPUT_NEURONS 2
#define PI 3.14159265f

class VariableManager{
private:

    // Physics
    static float MAX_ACCELERATION_CONSTANT; // Units: pixels per second squared
    static float MAX_ANGULAR_ACCELERATION_CONSTANT; // Units: degrees per second squared
    static float MAX_SPEED; // Units: pixels per second
    static float ROTATIONAL_SPEED_MULTIPLIER;
    static float ANGULAR_DAMPING_MULTIPLIER;

    // Car
    static int SELECTED_CAR_INDEX;
    //static std::vector<carData> cars;

    // World
    static float TILE_SIZE;

    // Resources
    static std::string FONT_RUBIK_REGULAR;
    static std::string FONT_UPHEAVAL_PRO;
    static std::string FONT_MENU_TITLE;

    static std::string PATH_TO_CONFIG;
    static std::string PATH_TO_TILE_CONFIG;
    static std::string PATH_TO_CAR_CONFIG;
    static std::string PATH_TO_LEVELS;
    static std::string PATH_TO_BACKGROUNDS;

    static std::string IMAGE_CAR_CHOOSING_ARROW_LEFT;
    static std::string IMAGE_CAR_CHOOSING_ARROW_RIGHT;
    static std::string IMAGE_GAME_BACKGROUND;
    static std::string IMAGE_BACKGROUND_CAR_CHOOSING;

    // AI
    static int RAY_AMOUNT;
    static float AI_FOV;
    static int NETWORKS_AMOUNT;
    static std::vector<int> AI_TOPOLOGY;
    static float AI_INITIAL_RANDOM_VALUES_MAX;
    static bool AI_INITIAL_RANDOM_VALUES_UNIFORM;
    static std::vector<Activations> AI_ACTIVATIONS;

    // Evolutionary Algorithm
    static int AI_WINNERS;
    static float CHECKPOINT_POINTS;
    static float CHECKPOINT_RADIUS;
    static float ROTATION_PENALTY;
    static float BACKWARDS_MOVEMENT_PENALTY;
    static float RESTART_ON_DEAD_PERCENTAGE;
    static float MUTATION_INDEX;

    // DEBUG - RENDER
    static bool SHOW_COLLIDERS;
    static bool SHOW_CHECKPOINTS;
    static bool SHOW_RAYS;
    static bool SHOW_CARS_FIRST;

    // Game
    static bool AI_MODE;

    // Settings
    static std::string UNITS;
    static std::string DISPLAY_MODE;
    static std::string RESOLUTION;
    static bool VSYNC;
    static int FPS_LIMIT;
    static bool FPS_COUNTER;
    static std::string METRICS_MODE;
public:

    /// Load and Save functions
    static void loadFromJson(const std::string& filePath);
    static void saveToJson(const std::string& filePath);

    /// Getter and Setter functions

    // Math
    static float getPI() { return PI; }

    // Physics
    static float getMaxAccelerationConstant() { return MAX_ACCELERATION_CONSTANT; }
    static void setMaxAccelerationConstant(float value) { MAX_ACCELERATION_CONSTANT = value; }

    static float getMaxAngularAccelerationConstant() { return MAX_ANGULAR_ACCELERATION_CONSTANT; }
    static void setMaxAngularAccelerationConstant(float value) { MAX_ANGULAR_ACCELERATION_CONSTANT = value; }

    static float getMaxSpeed() { return MAX_SPEED; }
    static void setMaxSpeed(float value) { MAX_SPEED = value; }

    static float getRotationalSpeedMultiplier() { return ROTATIONAL_SPEED_MULTIPLIER; }
    static void setRotationalSpeedMultiplier(float value) { ROTATIONAL_SPEED_MULTIPLIER = value; }

    static float getAngularDampingMultiplier() { return ANGULAR_DAMPING_MULTIPLIER; }
    static void setAngularDampingMultiplier(float value) { ANGULAR_DAMPING_MULTIPLIER = value; }

    // Car
    static int getSelectedCarIndex() { return SELECTED_CAR_INDEX; }
    static void setSelectedCarIndex(int value) { SELECTED_CAR_INDEX = value; }

    // World
    static float getTileSize() { return TILE_SIZE; }
    static void setTileSize(float value) { TILE_SIZE = value; }

    // Resources
    static std::string getFontRubikRegular() { return FONT_RUBIK_REGULAR; }
    static void setFontRubikRegular(const std::string& value) { FONT_RUBIK_REGULAR = value; }

    static std::string getFontUpheavalPro() { return FONT_UPHEAVAL_PRO; }
    static void setFontUpheavalPro(const std::string& value) { FONT_UPHEAVAL_PRO = value; }

    static std::string getFontMenuTitle() { return FONT_MENU_TITLE; }
    static void setFontMenuTitle(const std::string& value) { FONT_MENU_TITLE = value; }

    static std::string getPathToConfig() { return PATH_TO_CONFIG; }
    static void setPathToConfig(const std::string& value) { PATH_TO_CONFIG = value; }

    static std::string getPathToTileConfig() { return PATH_TO_TILE_CONFIG; }
    static void setPathToTileConfig(const std::string& value) { PATH_TO_TILE_CONFIG = value; }

    static std::string getPathToCarConfig() { return PATH_TO_CAR_CONFIG; }
    static void setPathToCarConfig(const std::string& value) { PATH_TO_CAR_CONFIG = value; }

    static std::string getPathToLevels() { return PATH_TO_LEVELS; }
    static void setPathToLevels(const std::string& value) { PATH_TO_LEVELS = value; }

    static std::string getPathToBackgrounds() { return PATH_TO_BACKGROUNDS; }
    static void setPathToBackgrounds(const std::string& value) { PATH_TO_BACKGROUNDS = value; }

    static std::string getImageCarChoosingArrowLeft() { return IMAGE_CAR_CHOOSING_ARROW_LEFT; }
    static void setImageCarChoosingArrowLeft(const std::string& value) { IMAGE_CAR_CHOOSING_ARROW_LEFT = value; }

    static std::string getImageCarChoosingArrowRight() { return IMAGE_CAR_CHOOSING_ARROW_RIGHT; }
    static void setImageCarChoosingArrowRight(const std::string& value) { IMAGE_CAR_CHOOSING_ARROW_RIGHT = value; }

    static std::string getImageGameBackground() { return IMAGE_GAME_BACKGROUND; }
    static void setImageGameBackground(const std::string& value) { IMAGE_GAME_BACKGROUND = value; }

    static std::string getImageBackgroundCarChoosing() { return IMAGE_BACKGROUND_CAR_CHOOSING; }
    static void setImageBackgroundCarChoosing(const std::string& value) { IMAGE_BACKGROUND_CAR_CHOOSING = value; }

    // AI
    static int getRayAmount() { return RAY_AMOUNT; }
    static void setRayAmount(int value) { RAY_AMOUNT = value; }

    static float getAiFov() { return AI_FOV; }
    static void setAiFov(float value) { AI_FOV = value; }

    static int getNetworksAmount() { return NETWORKS_AMOUNT; }
    static void setNetworksAmount(int value) { NETWORKS_AMOUNT = value; }

    static std::vector<int> &getAiTopology() { return AI_TOPOLOGY; }
    static void setAiTopology(std::vector<int> &value) { AI_TOPOLOGY = value; }

    static std::vector<Activations> &getAiActivations() { return AI_ACTIVATIONS; }
    static void setAiActivations(std::vector<Activations> &value) { AI_ACTIVATIONS = value; }

    static float getAiInitialRandomValuesMax() { return AI_INITIAL_RANDOM_VALUES_MAX; }
    static void setAiInitialRandomValuesMax(float value) { AI_INITIAL_RANDOM_VALUES_MAX = value; }

    static bool getAiInitialRandomValuesUniform() { return AI_INITIAL_RANDOM_VALUES_UNIFORM; }
    static void setAiInitialRandomValuesUniform(bool value) { AI_INITIAL_RANDOM_VALUES_UNIFORM = value; }

    // Evolutionary Algorithm
    static  int getAiWinners() { return AI_WINNERS; }
    static void setAiWinners(int value) { AI_WINNERS = value; }

    static float getCheckpointPoints() { return CHECKPOINT_POINTS; }
    static void setCheckpointPoints(float value) { CHECKPOINT_POINTS = value; }

    static float getCheckpointRadius() { return CHECKPOINT_RADIUS; }
    static void setCheckpointRadius(float value) { CHECKPOINT_RADIUS = value; }

    static float getRotationPenalty() { return ROTATION_PENALTY; }
    static void setRotationPenalty(float value) { ROTATION_PENALTY = value; }

    static float getBackwardsMovementPenalty() { return BACKWARDS_MOVEMENT_PENALTY; }
    static void setBackwardsMovementPenalty(float value) { BACKWARDS_MOVEMENT_PENALTY = value; }

    static float getRestartOnDeadPercentage() { return RESTART_ON_DEAD_PERCENTAGE; }
    static void setRestartOnDeadPercentage(float value) { RESTART_ON_DEAD_PERCENTAGE = value; }

    static float getMutationIndex() { return MUTATION_INDEX; }
    static void setMutationIndex(float value) { MUTATION_INDEX = value; }

    // DEBUG - RENDER
    static bool getShowColliders() { return SHOW_COLLIDERS; }
    static void setShowColliders(bool value) { SHOW_COLLIDERS = value; }

    static bool getShowCheckpoints() { return SHOW_CHECKPOINTS; }
    static void setShowCheckpoints(bool value) { SHOW_CHECKPOINTS = value; }

    static bool getShowRays() { return SHOW_RAYS; }
    static void setShowRays(bool value) { SHOW_RAYS = value; }

    static bool getShowCarsFirst() { return SHOW_CARS_FIRST; }
    static void setShowCarsFirst(bool value) { SHOW_CARS_FIRST = value; }

    // Game
    static bool getAiMode() { return AI_MODE; };
    static void setAiMode(bool v) { AI_MODE = v; };

    // Settings
    static std::string getUnits() { return UNITS; };
    static void setUnits(const std::string& u) { UNITS = u; };

    static std::string getDisplayMode() { return DISPLAY_MODE; };
    static void setDisplayMode(const std::string& m) { DISPLAY_MODE = m; };

    static std::string getResolution() {return RESOLUTION; };
    static void setResolution(const std::string& r) { RESOLUTION = r; };

    static bool getVSync() { return VSYNC; };
    static void setVSync(bool v) { VSYNC = v; };

    static int getFpsLimit() { return FPS_LIMIT; };
    static void setFpsLimit(int f) { FPS_LIMIT = f; };

    static bool getFpsCounter() { return FPS_COUNTER; };
    static void setFpsCounter(bool v) { FPS_COUNTER = v; };

    static std::string getMetricsMode() { return METRICS_MODE; };
    static void setMetricsMode(const std::string& m) { METRICS_MODE = m; };

};

#endif //INTELLIDRIVE_VARIABLEMANAGER_H