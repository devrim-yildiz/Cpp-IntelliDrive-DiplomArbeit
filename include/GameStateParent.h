//
// Created by Tobias on 17.12.2024.
//

#ifndef INTELLIDRIVE_GAMESTATEPARENT_H
#define INTELLIDRIVE_GAMESTATEPARENT_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "State.h"
#include "Tile.h"
#include "ResourceManager.h"
#include "PauseState.h"
#include "DeathState.h"
#include "Car.h"
#include "Game.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class GameStateParent : public State {
private:
    float timeSinceLastPrint = 0.0f;
    sf::Sprite backgroundSprite;

    sf::Vector2i boundaries;

    std::vector<std::vector<int>> placedTileIDs;
    std::vector<std::vector<sf::Sprite>> placedTileSprites;
    std::vector<Tile> tiles;

    sf::Vector2f spawnPointPosition;
    sf::Vector2f spawnPointDirection;
    bool hasSpawnPoint = false;
    float debugTimer = 0.0f;

    void loadLevelFromCSV(const std::string &filename, Game &game);
    void loadBackground(Game &game);

public:
    bool getLineIntersection(sf::Vector2f p0, sf::Vector2f p1,
                             sf::Vector2f p2, sf::Vector2f p3,
                             sf::Vector2f& intersectionPoint);

    virtual void initializeCar() = 0;

    bool isPointInPolygon(const sf::Vector2f& point, const sf::ConvexShape& polygon);
    bool isPointInPolygon(const sf::Vector2f& point, const sf::ConvexShape& polygon, const sf::Transform& transform);

    bool isPauseKeyPressed(const sf::Event& event) const;

    GameStateParent(Game& game, const std::string& levelFile);
    explicit GameStateParent(Game& game) {};

    // Getter and Setter
    float &getDebugTimer() {return debugTimer;};
    sf::Vector2i &getBoundaries() {return boundaries;};
    std::vector<std::vector<int>> &getPlacedTileIDs() {return this->placedTileIDs;};
    std::vector<std::vector<sf::Sprite>> &getPlacedTileSprites() {return this->placedTileSprites;};
    std::vector<Tile> &getTiles() {return this->tiles;};
    sf::Sprite &getBackgroundSprite() {return this->backgroundSprite;};
    sf::Vector2f &getSpawnPointPosition() {return this->spawnPointPosition;};
    sf::Vector2f &getSpawnPointDirection() {return this->spawnPointDirection;};
    bool getHasSpawnpoint() {return this->hasSpawnPoint;};

    void setDebugTimer(float val) {this->debugTimer = val;};

};


#endif //INTELLIDRIVE_GAMESTATEPARENT_H
