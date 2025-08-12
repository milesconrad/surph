#include <random>
#include <SFML/Graphics.hpp>

class Wave {
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
            entity.setPosition(positionGen(rng), -height / 2);
        }

        void update(float dt, int score) {
            entity.move(0, (velocity + (score * 2)) * dt);
        }

    private:
        sf::Texture texture;
        float velocity = 80;

        std::random_device randomDevice;
        std::mt19937 rng{randomDevice()};
        // generation range makes sure boulders/waves are not directly at the edge
        std::uniform_int_distribution<int> positionGen{50, 850};
};