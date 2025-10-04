#include <GL/glut.h>
#include <vector>
#include <cmath>
#include <iostream>

#include "include/player.hpp"
#include "include/enemy.hpp"

// globals
Player player;
float cube_angle = 0.0f;
int lastTime = 0;
std::vector<Enemy> enemies;

// init
void initGL() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.1f,0.1f,0.1f,1.0f);
    glutSetCursor(GLUT_CURSOR_NONE);

    GLfloat lightPos[]     = { 2.0f, 2.0f, 2.0f, 1.0f };
    GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularLight[]= { 1.0f, 1.0f, 1.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

    GLfloat mat_specular[] = {1.0,1.0,1.0,1.0};
    GLfloat mat_shininess[]= {50.0};
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    //adicionando inimigos
    enemies.push_back(Enemy(30.0f, 0.0f, 30.0f));
    enemies.push_back(Enemy(-20.0f, 0.0f, 10.0f));
    enemies.push_back(Enemy(2.0f, 0.0f, 50.0f));

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}   

// per-frame
void display() {

    // render
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    player.display();

    //desenhando arena
    glPushMatrix();
        glColor3f(0.2f,0.7f,1.0f);
        glNormal3f(0.0f, 1.0f, 0.0f); // normal apontando para cima (luz difusa correta)

        glBegin(GL_QUADS);
            glVertex3f(-50.0f, -1.0f, -50.0f);
            glVertex3f(-50.0f, -1.0f,  50.0f); 
            glVertex3f( 50.0f, -1.0f,  50.0f);
            glVertex3f( 50.0f, -1.0f, -50.0f);
        glEnd();
    glPopMatrix();

    // desenha cubo (posicionado na origem)
    glPushMatrix();
        glRotatef(cube_angle, 1.0f, 1.0f, 0.0f);
        glColor3f(0.2f,0.7f,1.0f);
        glutSolidCube(1.5f);
    glPopMatrix();

    //desenhando inimigos
    for (auto& enemy: enemies){
        if(enemy.isAlive()){
            enemy.desenhar();
        }
    }

    glutSwapBuffers();
}

void timer(int) {
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f; // segundos
    lastTime = currentTime;

    player.update(deltaTime);   // <<< atualiza a câmera continuamente

    cube_angle += 1.0f;
    if (cube_angle > 360.0f) cube_angle -= 360.0f;

    //atualiza informações dos inimigos
    for(auto& enemy: enemies){
        if(enemy.isAlive()){
            enemy.update(deltaTime, player.get_position());

            //checando colisão com o player
            if(enemy.verificarColisaoPlayer(player.get_position())){
                //se true aplica knockback no player
                Point dirEnemy = Point(player.get_position().getX() - enemy.getPosition().getX(),
                                  0, player.get_position().getZ() - enemy.getPosition().getZ());

                //normalizando vetor de knockback para obter direção
                float dist = sqrt(dirEnemy.getX() * dirEnemy.getX() + dirEnemy.getZ() * dirEnemy.getZ());
                if(dist > 0){
                    dirEnemy = Point(dirEnemy.getX()/dist, 0, dirEnemy.getZ()/dist);
                }

                player.takeHit(dirEnemy);
            }

            //checando colisão com a espada
            if(player.verificarAtaque() && enemy.verificarColisaoSword(player.get_position())){

                Point knockbackDir = Point(enemy.getPosition().getX() - player.get_position().getX(),
                                    0, enemy.getPosition().getZ() - player.get_position().getZ());

                enemy.takeHit(knockbackDir);
            }
        }
    }

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void reshape(int w, int h) {
    if (h==0) h=1;
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (double)w/h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard_down(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC
    player.keyboard_down(key);
}

void keyboard_up(unsigned char key, int x, int y) {
    player.keyboard_up(key);
}

void mouse_motion(int x, int y) {
    player.mouse_motion(x, y);
}

void mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        player.playerAttack();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800,600);
    glutCreateWindow("jogo");
    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(keyboard_down);
    glutKeyboardUpFunc(keyboard_up);
    glutPassiveMotionFunc(mouse_motion);
    glutMouseFunc(mouse_click);

    glutTimerFunc(0, timer, 0);
    lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutMainLoop();
    return 0;
}
