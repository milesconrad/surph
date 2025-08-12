#include <random>
#include <SFML/Graphics.hpp>

class Boulder {
    public:
        sf::CircleShape entity;
        int height = 150;

        void init() {
            if (!texture.loadFromFile("assets/boulder_" + std::to_string(boulderSpriteGen(rng)) + ".png")) {
                texture.loadFromFile("assets/boulder_" + std::to_string(boulderSpriteGen(rng)) + ".png");
            }
            entity.setTexture(&texture);

            entity.setRadius(height / 2);
            entity.setOrigin(height / 2, height / 2);
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
        std::uniform_int_distribution<int> boulderSpriteGen{1, 3};
};