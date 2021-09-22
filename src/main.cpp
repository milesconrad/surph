#include <chrono>
#include <thread>
#include "objects.hpp"

// saves a lot of space in the GetTime() function
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
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) {
        (*game).restart(GetTime(), keys);
    }
}

void ScoreKeeper(Game *game) {
    while ((*game).window.isOpen()) {
        if ((*game).GameRunning) {
            (*game).score = (GetTime() - (*game).StartTime) / 1000;
        }
    } 
};

int main() {
    Game game;
    game.init(GetTime());
    std::thread ScoreKeeperThread(ScoreKeeper, &game);

    // PressedKeys[0] is left arrow, PressedKeys[1] is right arrow
    bool PressedKeys[2] = {false, false};
    double LastFrame = game.StartTime;
    float DeltaTime;
    while (game.window.isOpen()) {
        sf::Event event;
        if (game.window.pollEvent(event)) {
            HandleEvent(event, &game, &PressedKeys);
        }

        DeltaTime = (GetTime() - LastFrame) / 1000;
        LastFrame = GetTime();

        if (game.CollisionDetect()) {
            game.end();
        }

        game.run(DeltaTime, &PressedKeys);
        game.window.display();
    }

    ScoreKeeperThread.join();
    return 0;
}