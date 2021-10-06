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
            entity.setPosition(PositionGen(rng), -height / 2);
        }

        void update(float dt, int score) {
            entity.move(0, (velocity + (score * 2)) * dt);
        }
};