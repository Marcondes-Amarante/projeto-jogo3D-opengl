#ifndef SWORD_HPP
#define SWORD_HPP

#include "point.hpp"
#include "object3d.hpp"
#include <GL/glut.h>

class Sword: Object3D{

    Point offset;
    Point position;

    public:
        float swordAngle;     
        bool attacking;
        float attackSpeed;

        Sword();
        void desenhar(const Point& cameraPos, float pitch, float yaw);
        void update(const Point& cameraPos, const Point& cameraDir);
        void ataque();

        //getters
        const Point& getPosition() const {return position; }

};


#endif