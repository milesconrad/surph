#include <random>
#include "game.hpp"

void Game::init() {
    gameRunning = true;

    player.init();

    for (int i = 0; i < wavesNum; i++) {
        waves[i].init();
        waves[i].entity.move(0, -125 * i);
    }

    for (int i = 0; i < bouldersNum; i++) {
        boulders[i].init();
        boulders[i].entity.move(0, -175 * i);
    }
}

bool Game::collisionDetect() {
    sf::Vector2f boulderPos;
    sf::Vector2f *edgePoints = player.edgePoints;
    int distance;
    for (int i = 0; i < bouldersNum; i++) {
        boulderPos = boulders[i].entity.getPosition();
        for (int j = 0; j < player.edgePointsNum; j++) {
            // finds distance between current edge point and the center of the boulder
            distance = sqrt(pow(boulderPos.x - edgePoints[j].x, 2) + pow(boulderPos.y - edgePoints[j].y, 2));
            if (distance <= boulders[i].entity.getRadius()) {
                return true;
            }
        }
    }
    return false;
}