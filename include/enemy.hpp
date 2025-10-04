#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "point.hpp"

class Enemy{

    private:

        Point position;
        Point velocity;

        float posBaseY;
        float animTime;

        bool foiAtingido;
        float hitTime;

        float health;

    public:

        Enemy();
        Enemy(float x, float y, float z);
        void desenhar();
        void update(float deltaTime, const Point& cameraPos);
        void takeHit(const Point& knockbackDir);
        void applyKnockback(const Point& direction);

        //getters
        const Point& getPosition() const {return position; }
        bool isAlive() const {return health > 0; }
        bool levandoDano() const {return foiAtingido; }

        //verificação de colisões
        bool verificarColisaoPlayer(const Point& cameraPos, float raio = 1.0f) const;
        bool verificarColisaoSword(const Point& swordPos, float raio = 2.0f) const;
};

#endif