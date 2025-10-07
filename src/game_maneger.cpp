#include "game_maneger.hpp"
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

GameManager* GameManager::game = nullptr;

void drawText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

GameManager* GameManager::get_instance() {
    if (!game)
        game = new GameManager();
    return game;
}   

GameManager::GameManager() {
    currentState = MENU;
}

void GameManager::startGame() {
    currentState = PLAYING;
    currentWave = 0;
    enemiesRemaining = 0;
    lastTime = glutGet(GLUT_ELAPSED_TIME);
    player.init_player();
    spawnWave(1);
}

void GameManager::spawnWave(int waveNumber) {
    currentWave = waveNumber;
    enemies.clear();
    int numEnemies = 1 + ((int) waveNumber * 1.4);
    enemiesRemaining = numEnemies;
    for (int i = 0; i < numEnemies; ++i) {
        float xPos = (rand() % 40) - 20;
        float zPos = (rand() % 40) - 20;
        enemies.push_back(Enemy(xPos, -.2f, zPos));
    }
}

void GameManager::drawHUD() {
    glMatrixMode(GL_PROJECTION); glPushMatrix(); glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    if (currentState == PLAYING || currentState == GAME_OVER || currentState == VICTORY) {
        int health = static_cast<int>(player.get_health());
        drawText(20, 560, "Vida: " + std::to_string(health > 0 ? health : 0));
        drawText(20, 530, "Onda: " + std::to_string(currentWave));
        drawText(20, 500, "Inimigos: " + std::to_string(enemiesRemaining));
    }
    if (currentState == MENU) {
        glColor3f(1.0, 0.8, 0.2); drawText(280, 400, "HAUNTER SLAYER");
        glColor3f(1.0, 1.0, 1.0); drawText(250, 300, "Pressione 'ESPACO' para comecar");
    }
    if (currentState == GAME_OVER) { 
        glColor3f(1.0, 0.0, 0.0); 
        drawText(320, 300, "GAME OVER");
        glColor3f(1.0, 1.0, 1.0); drawText(250, 270, "Pressione 'ESPACO' para recomecar");
    }
    if (currentState == VICTORY) { 
        glColor3f(0.0, 1.0, 0.0); 
        drawText(320, 300, "VITORIA!"); 
        glColor3f(1.0, 1.0, 1.0); drawText(250, 270, "Pressione 'ESPACO' para recomecar");
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

void GameManager::keyboard_down(unsigned char key, int x, int y) {
    if (key == 27) exit(0); // ESC
    if (currentState == MENU || currentState == GAME_OVER || currentState == VICTORY) {
        if (key == ' ' || key == ' ') startGame();
        return;
    }
    player.keyboard_down(key);
}

void GameManager::keyboard_up(unsigned char key, int x, int y) {
    if (currentState != PLAYING) return;
    player.keyboard_up(key);
}

void GameManager::mouse_motion(int x, int y) {
    if (currentState != PLAYING) return;
    player.mouse_motion(x, y);
}

void GameManager::mouse_click(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        player.playerAttack();
    }
}

void GameManager::display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    player.display();
    battle_arena.draw();
    
    if (currentState == PLAYING) {
        for (auto& enemy: enemies){
            if(enemy.isAlive()){
                enemy.desenhar(
                    player.get_camera().get_position().getX(),
                    player.get_camera().get_position().getZ()
                );
            }
        }

    }

    drawHUD();

    glutSwapBuffers();
}

void GameManager::timer(int) {
    if (currentState != PLAYING) {
        glutPostRedisplay();
        glutTimerFunc(16, [] (int a) { GameManager::get_instance()->timer(a); }, 0);
        return;
    }

    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = (currentTime - lastTime) / 1000.0f; // segundos
    lastTime = currentTime;

    player.update(deltaTime);   // <<< atualiza a câmera continuamente

    //atualiza informações dos inimigos
    int alive_enemies = 0;
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
            alive_enemies++;
        }
    }
    enemiesRemaining = alive_enemies;
    if (enemiesRemaining <= 0) {
        if (currentWave < 5)
            spawnWave(currentWave + 1);
        else
            currentState = VICTORY;
    }

    if (!player.is_alive()) {
        currentState = GAME_OVER;
    }

    glutPostRedisplay();
    glutTimerFunc(16, [] (int a) { GameManager::get_instance()->timer(a); }, 0);
}