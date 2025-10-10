#include "game_maneger.hpp"
#include <GL/glut.h>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#define LIMITX 10
#define LIMITZ 8

int max_waves = 3; 
float speed_boost = 1;
float scale_boost = 1;
int extra_health = 0;

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
    player.add_extra_health(extra_health);
    spawnWave(1);
}

void GameManager::spawnWave(int waveNumber) {
    currentWave = waveNumber;
    int numEnemies = 1 + ((int) waveNumber * 1.5);
    enemiesRemaining = numEnemies;
    if (true) {
        spawn_health = true;
    } else {
        spawn_health = false;
    }
    for (int i = 0; i < numEnemies; i++) {
        float xPos, zPos;
        while (1) {
            xPos = (rand() % 40) - 20;
            zPos = (rand() % 40) - 20;
            if ((player.get_position().getX() - xPos) * (player.get_position().getX() - xPos) + \
            (player.get_position().getZ() - zPos) * (player.get_position().getZ() - zPos) > 25) {
                break;
            }
        }
        if (i < enemies.size()) {
            enemies[i].reset(xPos, -.2f, zPos);
        } else {
            enemies.push_back(Enemy(xPos, -.2f, zPos));
        }
        enemies[i].set_speed_boost(speed_boost);
        enemies[i].set_scale_boost(scale_boost);
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
        drawText(20, 530, "Onda: " + std::to_string(currentWave) + '/' + std::to_string(max_waves));
        drawText(20, 500, "Inimigos: " + std::to_string(enemiesRemaining));
    }
    if (currentState == MENU) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColor4f(0.0f, 0.0f, 0.0f, 0.5f); 
        glBegin(GL_QUADS);
        glVertex2f(0, 0);
        glVertex2f(800, 0);
        glVertex2f(800, 600);
        glVertex2f(0, 600);
        glEnd();
        glDisable(GL_BLEND); // Desativa o blending para outros elementos


        glColor3f(1.0, 0.85, 0.1);
        
        drawText(280, 450, "HAUNTER SLAYER");

        std::vector<std::string> options = { "Iniciar Novo Jogo" };
        if (hasSavedGame) options.push_back("Continuar");
        options.push_back("Sair");

        for (int i = 0; i < (int)options.size(); i++) {
            if (i == selectedOption)
                glColor3f(1.0, 1.0, 0.2); // destaque
            else
                glColor3f(0.6, 0.8, 1.0);
            drawText(330, 350 - i * 40, options[i]);
        }
    }

   


    if (currentState == GAME_OVER) {
        glColor3f(1.0, 0.2, 0.1);
        drawText(300, 300, "GAME OVER");
        glColor3f(0.8, 0.8, 0.8);
        drawText(240, 270, "Pressione 'ESPACO' para recomecar");
    }

    if (currentState == VICTORY) {
        glColor3f(0.0, 0.9, 0.4);
        drawText(320, 300, "VITORIA!");
        glColor3f(1.0, 0.95, 0.6);
        drawText(240, 270, "Pressione 'ESPACO' para recomecar");
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION); glPopMatrix();
    glMatrixMode(GL_MODELVIEW); glPopMatrix();
}

void GameManager::keyboard_down(unsigned char key, int x, int y) {
    // ESC → pausa ou sai do jogo
    if (key == 27) { // ESC
    if (currentState == PLAYING) {
        hasSavedGame = true;     
        currentState = MENU;      
        return;
    } else if (currentState == MENU) {
        exit(0);
    }
}


    // === MENU PRINCIPAL ===
    if (currentState == MENU) {
        if (key == 'w' || key == 'W') {
            selectedOption--;
            if (selectedOption < 0) selectedOption = 2;
        } else if (key == 's' || key == 'S') {
            selectedOption++;
            if (selectedOption > 2) selectedOption = 0;
        } else if (key == 13 || key == ' ') { // Enter ou Espaço
            if (selectedOption == 0) { // Iniciar
                startGame();
            } else if (selectedOption == 1 && hasSavedGame) { // Continuar
                currentState = PLAYING;
            } else { // Sair
                exit(0);
            }
        }
        return;
    }

 

    // === GAME OVER / VITÓRIA ===
    if (currentState == GAME_OVER || currentState == VICTORY) {
        if (key == ' ' || key == 13) startGame();
        return;
    }

    // === JOGANDO ===
    if (currentState == PLAYING) {
        if (key == 'r' || key == 'R') {
            startGame();
            return;
        }
        player.keyboard_down(key);
    }
}
void GameManager::special_key(int key, int x, int y) {
    if (currentState == MENU) {
        switch (key) {
            case GLUT_KEY_UP:
                selectedOption--;
                if (selectedOption < 0) selectedOption = 2; // último item
                break;

            case GLUT_KEY_DOWN:
                selectedOption++;
                if (selectedOption > 2) selectedOption = 0; // volta pro topo
                break;

            
            case 13: // Enter normal
                if (selectedOption == 0) { // Iniciar
                    startGame();
                } else if (selectedOption == 1 && hasSavedGame) { // Continuar
                    currentState = PLAYING;
                } else { // Sair
                    exit(0);
                }
                break;
        }
    }

    
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

    if (false) {
        glPushMatrix();
            // glRotatef(cube_angle, 1.0f, 1.0f, 0.0f);
            glTranslated(1.0, 0.0, 1.0);
            GLfloat diffuse[]  = { 0.8, 0, 0, 1.0f };
            GLfloat specular[] = { 0.9, 0.9, 0.9, 1.0f };
            GLfloat shininess = 10;
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  diffuse);
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
            glutSolidSphere(1.0f, 40, 40);
        glPopMatrix();
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
        if (currentWave < max_waves)
            spawnWave(currentWave + 1);
        else {
            currentState = VICTORY;
            scale_boost += .6;
            speed_boost += .6;
            max_waves++;
            extra_health += 5;
        }
    }

    if (!player.is_alive()) {
        currentState = GAME_OVER;
    }

    glutPostRedisplay();
    glutTimerFunc(16, [] (int a) { GameManager::get_instance()->timer(a); }, 0);
}