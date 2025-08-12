#include <thread>
#include "game.hpp"

sf::RenderWindow window;
sf::Text scoreText;

Game game{};
sf::Clock gameClock;
int playerDirection = 0; // -1 is left, 1 is right
float lastFrame = gameClock.getElapsedTime().asSeconds(); // if not reset when clock is reset, delta time goes negative

void scoreKeeper() {
    while (window.isOpen()) {
        if (game.gameRunning) {
            game.score = gameClock.getElapsedTime().asSeconds();
        }
    } 
}

void handleEvent(sf::Event event) {
    if (event.type == sf::Event::Closed) {
        game.gameRunning = false;
        window.close();
    }

    if (game.gameRunning) {
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                playerDirection -= 1;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                playerDirection += 1;
            }
        } else if (event.type == sf::Event::KeyReleased) {
            if (event.key.code == sf::Keyboard::Key::Left) {
                playerDirection += 1;
            }
            if (event.key.code == sf::Keyboard::Key::Right) {
                playerDirection -= 1;
            }
        }
    } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Key::Space) {
        lastFrame = 0;
        game.init();
	    gameClock.restart();
        scoreText.setOutlineThickness(0);
    }
}

int main() {
    std::thread scoreKeeperThread(scoreKeeper);

    float deltaTime;
    sf::Font arial;

    window.create(sf::VideoMode(900, 600), "Surph");
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    if (!arial.loadFromFile("assets/arial.ttf")) {
        window.close();
    }

    scoreText.setFont(arial);
    scoreText.setCharacterSize(30);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(0);

    game.init();

    while (window.isOpen()) {
        sf::Event event;
        if (window.pollEvent(event)) {
            handleEvent(event);
        }

        deltaTime = gameClock.getElapsedTime().asSeconds() - lastFrame;
        lastFrame = gameClock.getElapsedTime().asSeconds();

        if (game.collisionDetect()) {
            playerDirection = 0;
            game.gameRunning = false;
            // i know this is bad practice but i didn't feel like making a second object just for an end screen
            scoreText.setString("Game Over! Total Score: " + std::to_string(game.score) + "\n\n   Press Space to Restart!");

            scoreText.setOutlineThickness(5);
            
            sf::FloatRect scoreBounds = scoreText.getLocalBounds();
            scoreText.setPosition(450 - scoreBounds.width / 2, 300 - scoreBounds.height / 2);
        }

        window.clear(sf::Color(20, 37, 107, 255));

        if (game.gameRunning) {
            for (int i = 0; i < game.wavesNum; i++) {
                if (game.waves[i].entity.getPosition().y > 600 + game.waves[i].height) {
                    game.waves[i].init();
                }
                game.waves[i].update(deltaTime, game.score);
                window.draw(game.waves[i].entity);
            }

            for (int i = 0; i < game.bouldersNum; i++) {
                if (game.boulders[i].entity.getPosition().y > 600 + game.boulders[i].height) {
                    game.boulders[i].init();
                }
                game.boulders[i].update(deltaTime, game.score);
                window.draw(game.boulders[i].entity);
            }

            game.player.updateData(&playerDirection, game.score);
            game.player.updatePos(deltaTime);
            window.draw(game.player.entity);

            // position needs to be redefined because text size changes as game progresses
            // use the height of the text box to determine padding
            scoreText.setString("Score: " + std::to_string(game.score));
            sf::FloatRect scoreBounds = scoreText.getLocalBounds();
            scoreText.setPosition(900 - scoreBounds.width - scoreBounds.height + scoreBounds.left, scoreBounds.height - scoreBounds.top);
            window.draw(scoreText);
        } else {
            for (int i = 0; i < game.wavesNum; i++) {
                window.draw(game.waves[i].entity);
            }
            for (int i = 0; i < game.bouldersNum; i++) {
                window.draw(game.boulders[i].entity);
            }
            window.draw(game.player.entity);
            
            window.draw(scoreText);
        }

        window.display();
    }

    scoreKeeperThread.join();
    return 0;
}
