#include <chrono>
#include <thread>
#include "objects.hpp"

using namespace std::chrono;

double GetTime() {
    double ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return ms;
}

void HandleEvent(sf::Event event, Game *game, bool (*keys)[2]) {
    if (event.type == sf::Event::Closed) {
        (*game).GameRunning = false;
        (*game).window.close();
    }

    if ((*game).GameRunning) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                (*keys)[0] = true;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                (*keys)[1] = true;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                (*keys)[0] = false;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                (*keys)[1] = false;
            }
        }
    }
}

void ScoreKeeper(bool *GameOpen, double GameStart, int *score) {
    while (*GameOpen) {
        *score = (GetTime() - GameStart) / 1000;
    } 
};

int main() {
    Game game;
    game.window.create(sf::VideoMode(900, 600), "Surph");
    game.window.setVerticalSyncEnabled(true);

    // PressedKeys[0] is left arrow, PressedKeys[1] is right arrow
    bool PressedKeys[2] = {false, false};
    double LastFrame = GetTime();
    float DeltaTime;

    game.init();
    int score = 0;
    std::thread ScoreKeeperThread(ScoreKeeper, &game.GameRunning, GetTime(), &score);

    while (game.window.isOpen()) {
        sf::Event event;
        if (game.window.pollEvent(event)) {
            HandleEvent(event, &game, &PressedKeys);
        }

        DeltaTime = (GetTime() - LastFrame) / 1000;
        LastFrame = GetTime();

        game.run(DeltaTime, PressedKeys, score);
        
        sf::Vector2f BoulderPos;
        sf::Vector2f *EdgePoints = game.player.GlobalEdgePoints;
        int distance;
        for (int i = 0; i < game.BouldersNum; i++) {
            BoulderPos = game.boulders[i].entity.getPosition();
            for (int j = 0; j < game.player.EdgePointsNum; j++) {
                distance = sqrt(pow(BoulderPos.x - EdgePoints[j].x, 2) + pow(BoulderPos.y - EdgePoints[j].y, 2));
                if (distance <= game.boulders[i].entity.getRadius()) {
                    game.end(score);
                }
            }
        }

        game.window.display();
    }

    ScoreKeeperThread.join();
    return 0;
}