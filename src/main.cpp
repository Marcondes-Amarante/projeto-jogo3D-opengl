#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>
#include "game_maneger.hpp"

void initGL();
void reshape(int w, int h);
void lighting();

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("Haunter Slayer");
    glutPositionWindow(200, 50);
    initGL();

    glutDisplayFunc([] () { GameManager::get_instance()->display(); });
    glutReshapeFunc(reshape);

    glutKeyboardFunc([] (unsigned char key, int x, int y) { GameManager::get_instance()->keyboard_down(key, x, y); });
    glutKeyboardUpFunc([] (unsigned char key, int x, int y) { GameManager::get_instance()->keyboard_up(key, x, y); });
    glutPassiveMotionFunc([] (int x, int y) { GameManager::get_instance()->mouse_motion(x, y); });
    glutMouseFunc([] (int b, int s, int x, int y) { GameManager::get_instance()->mouse_click(b, s, x, y); });

    glutTimerFunc(0, [] (int a) { GameManager::get_instance()->timer(a); }, 0);

    glutMainLoop();
    return 0;
}

// init
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glutSetCursor(GLUT_CURSOR_NONE);

    lighting();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}   

void lighting() {
    GLfloat light_pos[]     = { 2.0f, 1.0f, 2.0f, 1.0f };
    GLfloat ambient_light[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuse_light[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat specular_light[]= { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    //glLightfv(GL_LIGHT0, GL_AMBIENT,  ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);

    GLfloat global_ambient[] = { 0.15f, 0.15f, 0.15f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glShadeModel(GL_SMOOTH);
}


void reshape(int w, int h) {
    if (h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}
