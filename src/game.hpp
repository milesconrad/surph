#include <random>
#include "player.hpp"
#include "boulder.hpp"
#include "wave.hpp"

std::random_device RandomDevice;
std::mt19937 rng(RandomDevice());
// generation range makes sure boulders/waves are not directly at the edge
std::uniform_int_distribution<int> PositionGen(50, 850);
std::uniform_int_distribution<int> BoulderSpriteGen(1, 3);

class Game {
    private:
        sf::Font arial;
        sf::Text ScoreText;
        sf::Text RestartText;

    public:
        sf::RenderWindow window;
        int score;
        Player player;

        Boulder boulders[4];
        int BouldersNum = sizeof(boulders) / sizeof(boulders[0]);
        Wave waves[5];
        int WavesNum = sizeof(waves) / sizeof(waves[0]);

        bool GameRunning = true;
        double StartTime;

        void init(double CurrentTime) {
            window.create(sf::VideoMode(900, 600), "Surph");
            window.setVerticalSyncEnabled(true);
            StartTime = CurrentTime;
            score = 0;

            player.init();

            for (int i = 0; i < WavesNum; i++) {
                waves[i].init();
                waves[i].entity.move(0, -125 * i);
            }

            for (int i = 0; i < BouldersNum; i++) {
                boulders[i].init();
                boulders[i].entity.move(0, -175 * i);
            }

            if (!arial.loadFromFile("assets/arial.ttf")) {
                window.close();
            }

            ScoreText.setFont(arial);
            ScoreText.setCharacterSize(30);
            ScoreText.setOutlineColor(sf::Color::Black);
            ScoreText.setOutlineThickness(0);
            
            RestartText.setFont(arial);
            RestartText.setCharacterSize(30);
            RestartText.setString("Press Space to Restart");
            sf::FloatRect RestartBounds = RestartText.getLocalBounds();
            RestartText.setPosition(450 - RestartBounds.width / 2, 300 - RestartBounds.height / 2 + 60);
            RestartText.setOutlineColor(sf::Color::Black);
            RestartText.setOutlineThickness(5);
        }

        bool CollisionDetect() {
            sf::Vector2f BoulderPos;
            sf::Vector2f *EdgePoints = player.EdgePoints;
            int distance;
            for (int i = 0; i < BouldersNum; i++) {
                BoulderPos = boulders[i].entity.getPosition();
                for (int j = 0; j < player.EdgePointsNum; j++) {
                    // finds distance between current edge point and the center of the boulder
                    distance = sqrt(pow(BoulderPos.x - EdgePoints[j].x, 2) + pow(BoulderPos.y - EdgePoints[j].y, 2));
                    if (distance <= boulders[i].entity.getRadius()) {
                        return true;
                    }
                }
            }
            return false;
        }

        void run(float dt, bool (*keys)[2]) {
            window.clear(sf::Color(20, 37, 107, 255));
            if (GameRunning) {
                for (int i = 0; i < WavesNum; i++) {
                    if (waves[i].entity.getPosition().y > 600 + waves[i].height) {
                        waves[i].init();
                    }
                    waves[i].update(dt, score);
                    window.draw(waves[i].entity);
                }

                for (int i = 0; i < BouldersNum; i++) {
                    if (boulders[i].entity.getPosition().y > 600 + boulders[i].height) {
                        boulders[i].init();
                    }
                    boulders[i].update(dt, score);
                    window.draw(boulders[i].entity);
                }

                player.UpdateData(keys, score);
                player.UpdatePos(dt);
                window.draw(player.entity);

                // use the height of the text box to determine padding
                ScoreText.setString("Score: " + std::to_string(score));
                sf::FloatRect ScoreBounds = ScoreText.getLocalBounds();
                ScoreText.setPosition(900 - ScoreBounds.width - ScoreBounds.height, ScoreBounds.height);
                window.draw(ScoreText);
            }
            else {
                for (int i = 0; i < WavesNum; i++) {
                    window.draw(waves[i].entity);
                }
                for (int i = 0; i < BouldersNum; i++) {
                    window.draw(boulders[i].entity);
                }
                window.draw(player.entity);
                
                window.draw(ScoreText);
                window.draw(RestartText);
            }
        }

        void end() {
            GameRunning = false;
            ScoreText.setString("Game Over! Total Score: " + std::to_string(score));

            ScoreText.setCharacterSize(30);
            ScoreText.setOutlineThickness(5);
            
            sf::FloatRect ScoreBounds = ScoreText.getLocalBounds();
            ScoreText.setPosition(450 - ScoreBounds.width / 2, 300 - ScoreBounds.height / 2 - 60);
        }

        void restart(double CurrentTime, bool (*PressedKeys)[2]) {
            (*PressedKeys)[0] = false;
            (*PressedKeys)[1] = false;
            GameRunning = true;
            StartTime = CurrentTime;
            score = 0;
        
            for (int i = 0; i < WavesNum; i++) {
                waves[i].init();
                waves[i].entity.move(0, -125 * i);
            }
            for (int i = 0; i < BouldersNum; i++) {
                boulders[i].init();
                boulders[i].entity.move(0, -175 * i);
            }
            player.init();

            ScoreText.setCharacterSize(30);
            ScoreText.setOutlineThickness(0);
        }
};
