#include <SFML/Graphics.hpp>

class Player {
    private:
        sf::Vector2f globalPosition;
        sf::Texture texture;
        float velocity;
        float radians;

    public:
        sf::CircleShape entity;
        float height = 15.5;

        sf::FloatRect bounds;
        
        sf::Vector2f edgePoints[7];
        int edgePointsNum = sizeof(edgePoints) / sizeof(edgePoints[0]);

        void init() {
            // makes a fresh circle shape (for restarting game)
            entity = sf::CircleShape(height);
            if (!texture.loadFromFile("assets/surfer.png")) {
                texture.loadFromFile("assets/surfer.png");
            }
            entity.setTexture(&texture);

            // makes the circle a 31 x ~100 ellipse
            entity.scale(1, 3.2);

            entity.setOrigin(height, height);
            entity.setPosition(450, 450);

            bounds = entity.getLocalBounds();
            // bounds still think the ellipse is a circle
            bounds.height = bounds.height * entity.getScale().y;

            velocity = 0;
        }

        void rotatePoints(float factor) {
            for (int i = 0; i < edgePointsNum; i++) {
                edgePoints[i].x = -edgePoints[i].y * sin(factor) + edgePoints[i].x * cos(factor);
                edgePoints[i].y = edgePoints[i].y * cos(factor) + edgePoints[i].x * sin(factor);

                edgePoints[i].x = globalPosition.x + edgePoints[i].x;
                edgePoints[i].y = globalPosition.y + edgePoints[i].y;
            }
        }

        void updateData(int *direction, int score) {
            // defining the outline of the ellipse relative to the center
            // only drawing points near the front of the surfboard, because touching the rock from the back would not kill you
            edgePoints[0] = sf::Vector2f(-(bounds.width / 2), 0);
            edgePoints[1] = sf::Vector2f(0, -(bounds.height / 2));
            edgePoints[2] = sf::Vector2f(bounds.width / 2, 0);

            edgePoints[3] = sf::Vector2f(-(bounds.width / 3), -(bounds.height / 4));
            edgePoints[4] = sf::Vector2f(bounds.width / 3, -(bounds.height / 4));

            edgePoints[5] = sf::Vector2f(-(bounds.width / 5), -(bounds.height / 2.5));
            edgePoints[6] = sf::Vector2f(bounds.width / 5, -(bounds.height / 2.5));

            
            globalPosition = entity.getPosition();
            // if the left or right key is pressed, accelerate and tilt in that direction
            // score makes everything move faster
            if ((*direction) < 0) {
                entity.setRotation(-20);

                radians = -20 * 3.1415926 / 180;
                rotatePoints(radians);

                if (velocity > (-200 - (score * 2))) {
                    velocity -= 5 + (score * 2);
                }
            }
            else if ((*direction) > 0) {
                entity.setRotation(20);

                radians = 20 * 3.1415926 / 180;
                rotatePoints(radians);

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
                        rotatePoints(radians);
                    }
                    else if (entity.getRotation() > 0) {
                        radians = -20 * 3.1415926 / 180;
                        rotatePoints(radians);
                    }
                    entity.setRotation(0);
                }
                else {
                    for (int i = 0; i < edgePointsNum; i++) {
                        edgePoints[i].x = globalPosition.x + edgePoints[i].x;
                        edgePoints[i].y = globalPosition.y + edgePoints[i].y;
                    }
                }
            }
        }

        void updatePos(float dt) {
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