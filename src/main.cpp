#include <chrono>
#include <thread>
#include "game.hpp"

double getTime();
void handleEvent(sf::Event event, Game *game, int *direction);
void scoreKeeper(Game *game);

int main() {
    Game game;
    game.init(getTime());
    std::thread scoreKeeperThread(scoreKeeper, &game);

    // -1 is left, 1 is right
    int playerDirection = 0;
    double LastFrame = game.startTime;
    float DeltaTime;
    while (game.window.isOpen()) {
        sf::Event event;
        if (game.window.pollEvent(event)) {
            handleEvent(event, &game, &playerDirection);
        }

        DeltaTime = (getTime() - LastFrame) / 1000;
        LastFrame = getTime();

        if (game.collisionDetect()) {
            game.end();
        }

        game.run(DeltaTime, &playerDirection);
        game.window.display();
    }

    scoreKeeperThread.join();
    return 0;
}

void scoreKeeper(Game *game) {
    while ((*game).window.isOpen()) {
        if ((*game).gameRunning) {
            (*game).score = (getTime() - (*game).startTime) / 1000;
        }
    } 
}

void handleEvent(sf::Event event, Game *game, int *direction) {
    if (event.type == sf::Event::Closed) {
        (*game).gameRunning = false;
        (*game).window.close();
    }

    if ((*game).gameRunning) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                (*direction) -= 1;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                (*direction) += 1;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                (*direction) += 1;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                (*direction) -= 1;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) {
        (*game).restart(getTime());
    }
}

double getTime() {
    double ms = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    return ms;
}