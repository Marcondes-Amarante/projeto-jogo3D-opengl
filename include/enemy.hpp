#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "point.hpp"
#include "object3d.hpp"

class Enemy : Object3D{

private:
    Point position;
    Point velocity;

    float posBaseY;
    float animTime;

    bool foiAtingido;
    float hitTime;

    float health;
    float scale_boost;
    float speed_boost;

public:

    Enemy();
    Enemy(float x, float y, float z);
    void desenhar(float playerX, float playerZ);
    void update(float deltaTime, const Point& cameraPos);
    void takeHit(const Point& knockbackDir);
    void applyKnockback(const Point& direction);
    void reset(float x, float y, float z);

    //getters
    const Point& getPosition() const {return position; }
    float get_scale() override;
    bool isAlive() const {return health > 0; }
    bool levandoDano() const {return foiAtingido; }

    void set_scale_boost(float boost);
    void set_speed_boost(float boost);

    //verificação de colisões
    bool verificarColisaoPlayer(const Point& cameraPos, float raio = 1.0f) const;
    bool verificarColisaoSword(const Point& swordPos, float raio = 2.0f) const;
};

#endif