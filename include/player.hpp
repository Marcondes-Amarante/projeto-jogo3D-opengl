#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "camera.hpp"
#include "sword.hpp"
#include "point.hpp"

class Player {
private:
    Camera camera;
    Sword sword;
    bool isAttacking;
    
    Point velocity;
    bool foiAtingido;
    float hitTime;

    float health;

public:

    Player();
    Player(float x, float y, float z, float pitch, float yaw);
    ~Player() = default;

    void display();
    
    const Point& get_position() const;
    string get_str_position() const;
    const Camera& get_camera() const;
    void keyboard_movement(unsigned char key, int x, int y, float deltaTime);

    void keyboard_up(unsigned char);
    void keyboard_down(unsigned char);
    void update(float deltaTime);

    void mouse_motion(int mouse_x, int mouse_y);
    void playerAttack();

    bool verificarAtaque();
    
    void takeHit(const Point& knockbackDir);
    bool is_alive();
    int get_health();
    void add_extra_health(int value);
    void applyKnockback(const Point& direction);
    void init_player();
};

#endif