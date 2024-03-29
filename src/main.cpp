#include <thread>
#include "game.hpp"

void handleEvent(sf::Event event);
void scoreKeeper();

Game game;
sf::Clock gameClock;
// -1 is left, 1 is right
int playerDirection = 0;
// if lastFrame is not reset when the clock is reset, deltaTime goes negative which messes with obstacle generation
float lastFrame = gameClock.getElapsedTime().asSeconds();

int main() {
    game.init();
    std::thread scoreKeeperThread(scoreKeeper);

    float deltaTime;
    while (game.window.isOpen()) {
        sf::Event event;
        if (game.window.pollEvent(event)) {
            handleEvent(event);
        }

        deltaTime = gameClock.getElapsedTime().asSeconds() - lastFrame;
        lastFrame = gameClock.getElapsedTime().asSeconds();

        if (game.collisionDetect()) {
            playerDirection = 0;
            game.end();
        }

        game.run(deltaTime, &playerDirection);
        game.window.display();
    }

    scoreKeeperThread.join();
    return 0;
}

void scoreKeeper() {
    while (game.window.isOpen()) {
        if (game.gameRunning) {
            game.score = gameClock.getElapsedTime().asSeconds();
        }
    } 
}

void handleEvent(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        game.gameRunning = false;
        game.window.close();
    }

    if (game.gameRunning) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                playerDirection -= 1;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                playerDirection += 1;
            }
        }
        else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                playerDirection += 1;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                playerDirection -= 1;
            }
        }
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) {
        lastFrame = 0;
        game.restart();
	    gameClock.restart();
    }
}
