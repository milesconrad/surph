class Boulder {
    private:
        sf::Texture texture;
        float velocity = 80;

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
};