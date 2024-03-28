#include <random>

std::random_device randomDevice;
std::mt19937 rng(randomDevice());
// generation range makes sure boulders/waves are not directly at the edge
std::uniform_int_distribution<int> positionGen(50, 850);
std::uniform_int_distribution<int> boulderSpriteGen(1, 3);

#include "player.hpp"
#include "boulder.hpp"
#include "wave.hpp"

class Game {
    private:
        sf::Font arial;
        sf::Text scoreText;

    public:
        sf::RenderWindow window;
        int score = 0;
        Player player;

        Boulder boulders[4];
        int bouldersNum = sizeof(boulders) / sizeof(boulders[0]);
        Wave waves[5];
        int wavesNum = sizeof(waves) / sizeof(waves[0]);

        bool gameRunning = true;

        void init() {
            window.create(sf::VideoMode(900, 600), "Surph");
            window.setVerticalSyncEnabled(true);
            window.setKeyRepeatEnabled(false);

            player.init();

            for (int i = 0; i < wavesNum; i++) {
                waves[i].init();
                waves[i].entity.move(0, -125 * i);
            }

            for (int i = 0; i < bouldersNum; i++) {
                boulders[i].init();
                boulders[i].entity.move(0, -175 * i);
            }

            if (!arial.loadFromFile("assets/arial.ttf")) {
                window.close();
            }

            scoreText.setFont(arial);
            scoreText.setCharacterSize(30);
            scoreText.setOutlineColor(sf::Color::Black);
            scoreText.setOutlineThickness(0);
        }

        bool collisionDetect() {
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

        void run(float dt, int *direction) {
            window.clear(sf::Color(20, 37, 107, 255));
            if (gameRunning) {
                for (int i = 0; i < wavesNum; i++) {
                    if (waves[i].entity.getPosition().y > 600 + waves[i].height) {
                        waves[i].init();
                    }
                    waves[i].update(dt, score);
                    window.draw(waves[i].entity);
                }

                for (int i = 0; i < bouldersNum; i++) {
                    if (boulders[i].entity.getPosition().y > 600 + boulders[i].height) {
                        boulders[i].init();
                    }
                    boulders[i].update(dt, score);
                    window.draw(boulders[i].entity);
                }

                player.updateData(direction, score);
                player.updatePos(dt);
                window.draw(player.entity);

                // position needs to be redefined because text size changes as game progresses
                // use the height of the text box to determine padding
                scoreText.setString("Score: " + std::to_string(score));
                sf::FloatRect scoreBounds = scoreText.getLocalBounds();
                scoreText.setPosition(900 - scoreBounds.width - scoreBounds.height + scoreBounds.left, scoreBounds.height - scoreBounds.top);
                window.draw(scoreText);
            }
            else {
                for (int i = 0; i < wavesNum; i++) {
                    window.draw(waves[i].entity);
                }
                for (int i = 0; i < bouldersNum; i++) {
                    window.draw(boulders[i].entity);
                }
                window.draw(player.entity);
                
                window.draw(scoreText);
            }
        }

        void end() {
            gameRunning = false;
            // i know this is bad practice but i didn't feel like making a second object just for an end screen
            scoreText.setString("Game Over! Total Score: " + std::to_string(score) + "\n\n   Press Space to Restart!");

            scoreText.setOutlineThickness(5);
            
            sf::FloatRect scoreBounds = scoreText.getLocalBounds();
            scoreText.setPosition(450 - scoreBounds.width / 2, 300 - scoreBounds.height / 2);
        }

        void restart() {
            gameRunning = true;
        
            for (int i = 0; i < wavesNum; i++) {
                waves[i].init();
                waves[i].entity.move(0, -125 * i);
            }
            for (int i = 0; i < bouldersNum; i++) {
                boulders[i].init();
                boulders[i].entity.move(0, -175 * i);
            }
            player.init();

            scoreText.setOutlineThickness(0);
        }
};
