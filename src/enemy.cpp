#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include "enemy.hpp"

#define M_PI 3.14159265359f

static float anim_speed = 3.0f;
static float knockback_force = 7.0f;

Enemy::Enemy(){
    position = Point(0, 0, 0);
    velocity = Point(0, 0, 0);
    posBaseY = 0.5f;
    animTime = 0.0f;
    foiAtingido = false;
    hitTime = 0.0f;
    health = 5.0f;
}

Enemy::Enemy(float x, float y, float z){
    position = Point(x, y, z);
    velocity = Point(0, 0, 0);
    posBaseY = y;
    animTime = 0.0f;
    foiAtingido = false;
    hitTime = 0.0f;
    health = 5.0f;
    set_scale(.2);
    if (!load_model("assets/haunter.obj"))
        throw std::invalid_argument("Carregamento da arena falhou");
    if (!load_model("assets/haunter.mtl"))
        throw std::invalid_argument("Carregamento da arena falhou");
}

void Enemy::desenhar(float playerX, float playerZ) {
    glPushMatrix();
        float dx = playerX - position.getX();
        float dz = playerZ - position.getZ();
        float angle = atan2(dz, dx) * 180.0f / M_PI;
        glTranslatef(position.getX(), position.getY(), position.getZ());
        glRotatef(-angle + 90.0f, 0.0f, 1.0f, 0.0f);
        setup_draw();
    glPopMatrix();
}

void Enemy::takeHit(const Point& knockbackDir){
    health -= 1.0f;
    foiAtingido = true;
    hitTime = 0.3f;
    applyKnockback(knockbackDir);
}

void Enemy::applyKnockback(const Point& direction){
    velocity = Point(direction.getX() * knockback_force, 
    0.5f,
    direction.getZ() * knockback_force);
}

void Enemy::update(float deltaTime, const Point& cameraPos){

    //decrementa e reseta duração do hit
    if(foiAtingido){
        hitTime -= deltaTime;
        if(hitTime <= 0.0f){
            foiAtingido = false;
        }
    }

    //calculando distância normalizada entre enemy e player
    Point direction = Point(cameraPos.getX() - position.getX(),
                    0, cameraPos.getZ() - position.getZ());
    
    float dist = sqrt(direction.getX() * direction.getX() + direction.getZ() * direction.getZ());
    
    if(dist > 0.1f){
        direction = Point(direction.getX() / dist, 0, direction.getZ()/dist);
    }

    //movimentando em direção ao player
    float speed;

    if(foiAtingido){
        speed = 1.0f;
    }else{
        speed = 2.0f;
    }

    position = Point(position.getX() + direction.getX() * speed * deltaTime,
               position.getY(), position.getZ() + direction.getZ() * speed * deltaTime);


    //aplicando knockback
    if(velocity.getX() !=0 || velocity.getY() !=0 || velocity.getZ()!=0){

        position = Point(position.getX() + velocity.getX() * deltaTime,
                   position.getY() + velocity.getY() * deltaTime,
                   position.getZ() + velocity.getZ() * deltaTime);

        //reduz velocidade
        velocity = Point(velocity.getX() * 0.9f, velocity.getY() * 0.9f, velocity.getZ() * 0.9f);

        //tratando velocidade muito pequena
        if(fabs(velocity.getX()) < 0.1f && fabs(velocity.getY()) < 0.1f && fabs(velocity.getZ()) < 0.1f){
            velocity = Point(0,0,0);
        }
    }

}

bool Enemy::verificarColisaoPlayer(const Point& cameraPos, float raio) const{
    float dx = position.getX() - cameraPos.getX();
    float dz = position.getZ() - cameraPos.getZ();
    float distance = sqrt(dx * dx + dz * dz);

    return distance < raio;
}

bool Enemy::verificarColisaoSword(const Point& swordPos, float raio) const{
    float dx = position.getX() - swordPos.getX();
    float dz = position.getZ() - swordPos.getZ();
    float distance = sqrt(dx * dx + dz * dz);

    return distance < raio;
}