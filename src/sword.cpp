#include <GL/glut.h>
#include <iostream>
#include "player.hpp"
#include "sword.hpp"


Sword::Sword(){
    attacking = false;
    attackSpeed = 5.0f;
    swordAngle = 0.0f;
    offset = Point(0.4f, 0.1f, -1.0f);
    set_scale(.06);
    if (!load_model("assets/sword.obj"))
        throw std::invalid_argument("Carregamento da arena falhou");
    if(!load_texture("assets/textures/sword_texture_1024x1024.png"))
        throw std::invalid_argument("Carregamento da textura da arena falhou");
}

void Sword::desenhar(const Point& cameraPos, float pitch, float yaw) {
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glLoadIdentity();
        glTranslatef(offset.getX(), offset.getY(), offset.getZ());
        glRotatef(-swordAngle, 1.0f, 0.0f, 0.0f);
        glColor3f(0.7f, 0.7f, 0.9f);
        setup_draw();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void Sword::update(const Point& cameraPos, const Point& cameraDir){

    //atualizando posição da espada em relação a câmera
    //position = Point(cameraPos.getX() + offset.getX(), 
               //cameraPos.getY() + offset.getY(),
               //cameraPos.getZ() + offset.getZ()
    //);

    if (attacking) {
        swordAngle -= attackSpeed; // volta devagar
        if (swordAngle <= 0.0f) {
            swordAngle = 0.0f;
            attacking = false;
        }
    }

}

void Sword::ataque(){
    if(!attacking){
        attacking = true;
        swordAngle = 70.0f;
    }
}