#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <vector>
#include "enemy.hpp"
#include "player.hpp"
#include "arena.hpp"

enum GameState { MENU, PLAYING, GAME_OVER, VICTORY };

class GameManager {
private:
    std::vector<Enemy> enemies;
    bool spawn_health;
    GameState currentState;
    int currentWave;
    float playerHealth;
    int enemiesRemaining;
    Player player;
    int lastTime = 0;
    Arena battle_arena;
    static GameManager* game;

    void drawHUD();
    void spawnWave(int waveNumber);
    void startGame();

    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;
public:
    static GameManager* get_instance();
    void keyboard_down(unsigned char key, int x, int y);
    void keyboard_up(unsigned char key, int x, int y);
    void mouse_motion(int x, int y);
    void mouse_click(int button, int state, int x, int y);
    void display();
    void timer(int);
};
#endif