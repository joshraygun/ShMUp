#ifndef SHMUP_H
#define SHMUP_H

#include <SFML/graphics.hpp>
#include <sstream>
#include <queue>

using namespace sf;

class ShMUp
{
    public:

        RenderWindow window;
        Sprite player;
        std::deque<Sprite> enemies;
        Texture playTex;
        Texture enTex;
        Clock clock;
        Time change;
        int lives;
        long score;
        Text HUD;
        Font displayFont;
        View camera;
        std::stringstream ss;
        std::vector<float> enX;
        std::deque<CircleShape> bullets;
        bool isMovingLeft = false;
        bool isMovingRight = false;
        bool hasFired;

        void updateHUD();
        void takeLife();
        void fire();
        void spawnEnemy(Vector2f);
        void processEvents();
        void render();
        void run();

        ShMUp();
    protected:
    private:
};

#endif // SHMUP_H
