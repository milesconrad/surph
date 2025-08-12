#include <random>
#include <SFML/Graphics.hpp>

class Player {
    public:
        sf::CircleShape entity;
        float height = 15.5;

        sf::FloatRect bounds;
        
        sf::Vector2f edgePoints[7];
        int edgePointsNum = 7;

        void init();

        void rotatePoints(float factor);

        void updateData(int *direction, int score);

        void updatePos(float dt);

    private:
        sf::Vector2f globalPosition;
        sf::Texture texture;
        float velocity;
        float radians;

        std::random_device randomDevice;
        std::mt19937 rng{randomDevice()};
        // generation range makes sure boulders/waves are not directly at the edge
        std::uniform_int_distribution<int> positionGen{50, 850};
};