#include "player.hpp"
#include "boulder.hpp"
#include "wave.hpp"

class Game {
    public:
        void init();
        bool collisionDetect();
        void restart();

        int score{0};
        bool gameRunning;

        Player player;

        Boulder boulders[4];
        int bouldersNum{sizeof(boulders) / sizeof(boulders[0])};
        Wave waves[5];
        int wavesNum{sizeof(waves) / sizeof(waves[0])};
};
