#include <GL/glut.h>
#include <iostream>
#include "include/player.hpp"
#include "include/sword.hpp"


Sword::Sword(){
    attacking = false;
    attackSpeed = 5.0f;
    swordAngle = 0.0f;

    offset = Point(0.3f, 0.2f, -1.0f);
}

void Sword::desenhar(const Point& cameraPos, float pitch, float yaw) {
    
    glPushMatrix();
    
    // Reseta apenas para a posição da câmera, mantendo a orientação
    glLoadIdentity();
    
    // Move para a posição relativa da câmera (coordenadas de tela)
    glTranslatef(offset.getX(), offset.getY(), offset.getZ());
    
    // Aplica APENAS a rotação do ataque da espada
    glRotatef(-swordAngle, 1.0f, 0.0f, 0.0f);

    glColor3f(0.7f, 0.7f, 0.9f);
    glScalef(0.1f, 0.7f, 0.1f);
    glutSolidCube(1.0f);
    
    glPopMatrix();
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