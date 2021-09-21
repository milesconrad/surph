#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>

std::random_device RandomDevice;
std::mt19937 rng(RandomDevice());
// generation range makes sure boulders/waves are not directly at the edge
std::uniform_int_distribution<int> PosGen(50, 850);
std::uniform_int_distribution<int> BoulderSpriteGen(1, 3);

class Wave {
    private:
        sf::Texture texture;
        float velocity = 80;

    public:
        sf::RectangleShape entity;
        int height = 40;

        void init() {
            if (!texture.loadFromFile("assets/wave.png")) {
                texture.loadFromFile("assets/wave.png");
            };
            entity.setTexture(&texture);
            entity.setSize(sf::Vector2f(400, height));
            entity.setOrigin(100, height / 2);
            entity.setPosition(PosGen(rng), -height / 2);
        }

        void update(float dt, int score) {
            entity.move(0, (velocity + (score * 2)) * dt);
        }
};

class Boulder {
    private:
        sf::Texture texture;
        float velocity = 80;

    public:
        sf::CircleShape entity;
        int height = 150;

        void init() {
            if (!texture.loadFromFile("assets/boulder_" + std::to_string(BoulderSpriteGen(rng)) + ".png")) {
                texture.loadFromFile("assets/boulder_" + std::to_string(BoulderSpriteGen(rng)) + ".png");
            }
            entity.setTexture(&texture);

            entity.setRadius(height / 2);
            entity.setOrigin(height / 2, height / 2);
            entity.setPosition(PosGen(rng), -height / 2);
        }

        void update(float dt, int score) {
            entity.move(0, (velocity + (score * 2)) * dt);
        }
};

class Player {
    private:
        sf::Vector2f GlobalPosition;
        float radians;
        sf::Texture texture;
        float velocity = 0;

    public:
        sf::CircleShape entity;
        float height = 15.5;

        sf::FloatRect bounds;
        
        sf::Vector2f EdgePoints[7];
        int EdgePointsNum = sizeof(EdgePoints) / sizeof(EdgePoints[0]);

        void init() {
            if (!texture.loadFromFile("assets/surfer.png")) {
                texture.loadFromFile("assets/surfer.png");
            }
            entity.setTexture(&texture);

            entity.setRadius(height);
            // makes the circle a 31, ~100 ellipse
            entity.scale(1, 3.2);

            entity.setOrigin(height, height);
            entity.setPosition(450, 450);

            bounds = entity.getLocalBounds();
            // bounds still think the ellipse is a circle
            bounds.height = bounds.height * entity.getScale().y;
        }

        void RotatePoints(float factor) {
            for (int i = 0; i < EdgePointsNum; i++) {
                EdgePoints[i].x = -EdgePoints[i].y * sin(factor) + EdgePoints[i].x * cos(factor);
                EdgePoints[i].y = EdgePoints[i].y * cos(factor) + EdgePoints[i].x * sin(factor);

                EdgePoints[i].x = GlobalPosition.x + EdgePoints[i].x;
                EdgePoints[i].y = GlobalPosition.y + EdgePoints[i].y;
            }
        }

        void UpdateData(bool PressedKeys[], int score) {
            // defining the outline of the ellipse relative to the center
            // only drawing points near the front of the surfboard, because touching the rock from the back would not kill you
            EdgePoints[0] = sf::Vector2f(-(bounds.width / 2), 0);
            EdgePoints[1] = sf::Vector2f(0, -(bounds.height / 2));
            EdgePoints[2] = sf::Vector2f(bounds.width / 2, 0);

            EdgePoints[3] = sf::Vector2f(-(bounds.width / 3), -(bounds.height / 4));
            EdgePoints[4] = sf::Vector2f(bounds.width / 3, -(bounds.height / 4));

            EdgePoints[5] = sf::Vector2f(-(bounds.width / 5), -(bounds.height / 2.5));
            EdgePoints[6] = sf::Vector2f(bounds.width / 5, -(bounds.height / 2.5));

            // score increases all velocities
            GlobalPosition = entity.getPosition();
            if (PressedKeys[0]) {
                entity.setRotation(-20);

                radians = -20 * 3.1415926 / 180;
                RotatePoints(radians);

                if (velocity > (-200 - (score * 2))) {
                    velocity -= 5 + (score * 2);
                }
            }
            else if (PressedKeys[1]) {
                entity.setRotation(20);

                radians = 20 * 3.1415926 / 180;
                RotatePoints(radians);

                if (velocity < (200 + (score * 2))) {
                    velocity += 5 + (score * 2);
                }
            }
            else {
                if (velocity < 0) {
                    velocity += 1.5;
                }
                else if (velocity > 0) {
                    velocity -= 1.5;
                }

                // rotates points and player back to 0
                if (entity.getRotation() != 0) {
                    if (entity.getRotation() < 0) {
                        radians = 20 * 3.1415926 / 180;
                        RotatePoints(radians);
                    }
                    else if (entity.getRotation() > 0) {
                        radians = -20 * 3.1415926 / 180;
                        RotatePoints(radians);
                    }
                    entity.setRotation(0);
                }
                else {
                    for (int i = 0; i < EdgePointsNum; i++) {
                        EdgePoints[i].x = GlobalPosition.x + EdgePoints[i].x;
                        EdgePoints[i].y = GlobalPosition.y + EdgePoints[i].y;
                    }
                }
            }
        }

        void UpdatePos(float dt) {
            if (entity.getPosition().x > 80 && velocity < 0) {
                entity.move(velocity * dt, 0);
            }
            else if (entity.getPosition().x < 820 && velocity > 0) {
                entity.move(velocity * dt, 0);
            }
            else {
                velocity = 0;
            }
        }
};

class Game {
    private:
        sf::Font arial;
        sf::Text ScoreText;

    public:
        sf::RenderWindow window;
        Player player;

        Boulder boulders[4];
        int BouldersNum = sizeof(boulders) / sizeof(boulders[0]);
        Wave waves[5];
        int WavesNum = sizeof(waves) / sizeof(waves[0]);

        bool GameRunning = true;

        void init() {
            window.create(sf::VideoMode(900, 600), "Surph");
            window.setVerticalSyncEnabled(true);

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
        }

        bool CollisionDetect() {
            sf::Vector2f BoulderPos;
            sf::Vector2f *EdgePoints = player.EdgePoints;
            int distance;
            for (int i = 0; i < BouldersNum; i++) {
                BoulderPos = boulders[i].entity.getPosition();
                for (int j = 0; j < player.EdgePointsNum; j++) {
                    distance = sqrt(pow(BoulderPos.x - EdgePoints[j].x, 2) + pow(BoulderPos.y - EdgePoints[j].y, 2));
                    if (distance <= boulders[i].entity.getRadius()) {
                        return true;
                    }
                }
            }
            return false;
        }

        void run(float dt, bool *keys, int score) {
            if (GameRunning) {
                window.clear(sf::Color(20, 37, 107, 255));

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
                sf::FloatRect bounds = ScoreText.getLocalBounds();
                ScoreText.setPosition(900 - bounds.width - bounds.height, bounds.height);
                window.draw(ScoreText);
            }
            else {
                window.clear(sf::Color(20, 37, 107, 255));

                for (int i = 0; i < WavesNum; i++) {
                    window.draw(waves[i].entity);
                }
                for (int i = 0; i < BouldersNum; i++) {
                    window.draw(boulders[i].entity);
                }
                window.draw(player.entity);
                
                window.draw(ScoreText);
            }
        }

        void end(int score) {
            GameRunning = false;
            ScoreText.setString("Game Over! Total Score: " + std::to_string(score));

            ScoreText.setCharacterSize(40);
            ScoreText.setFillColor(sf::Color::White);
            ScoreText.setOutlineThickness(3);
            ScoreText.setOutlineColor(sf::Color::Black);
            
            sf::FloatRect bounds = ScoreText.getLocalBounds();
            ScoreText.setPosition(450 - bounds.width / 2, 300 - bounds.height / 2);
        }
};
