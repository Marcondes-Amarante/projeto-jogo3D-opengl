#include <cmath>
#include "player.hpp"
#include "camera.hpp"
#include "sword.hpp"

//static float anim_speed = 3.0f;
static float knockback_force = 20.0f;

Player::Player() {
    this->camera = Camera(.0, .0, .0, .0, -90.0);
    isAttacking = false;
    health = 10.0f;
    velocity = Point(0, 0, 0);
    foiAtingido = false;
    hitTime = 0.0f;
}

Player::Player(float x, float y, float z, float pitch, float yaw) {
    this->camera = Camera(x, y, z, pitch, yaw);
    init_player();
}

void Player::init_player() {
    isAttacking = false;
    health = 10.0f;
    velocity = Point(0, 0, 0);
    foiAtingido = false;
    hitTime = 0.0f;
}

const Point& Player::get_position() const {
    return camera.get_position();
}

const Camera& Player::get_camera() const {
    return camera;
}

void Player::update(float deltaTime) {

    //aplicando knockback no player caso vetor de força(velocity) != 0
    if(velocity.getX() != 0 || velocity.getY() != 0 || velocity.getZ() != 0){
        Point posAtual = camera.get_position();

        //posicao após empurrão
        Point newPos = Point(posAtual.getX() + velocity.getX() * deltaTime,
                            posAtual.getY() + velocity.getY() * deltaTime,
                            posAtual.getZ() + velocity.getZ() * deltaTime);
        
        camera.set_position(newPos.getX(), newPos.getY(), newPos.getZ());

        //Reduzindo velocity gradualmente (fricção)
        velocity = Point(
            velocity.getX() * 0.85f, 
            velocity.getY() * 0.85f,
            velocity.getZ() * 0.85f
        );
        
        // resetando velocity quando muito pequeno
        if (fabs(velocity.getX()) < 0.1f && fabs(velocity.getY()) < 0.1f && fabs(velocity.getZ()) < 0.1f) {
            velocity = Point(0, 0, 0);
        }
    }

    if (foiAtingido) {
        hitTime -= deltaTime;
        if (hitTime <= 0.0f) {
            foiAtingido = false;
        }
    }

    camera.update(deltaTime);
    sword.update(camera.get_position(), camera.get_direction());
}
    
//void Player::keyboard_movement(unsigned char key, int x, int y, float deltaTime) {
    //camera.keyboard(key, x, y, deltaTime);
//}

void Player::keyboard_up(unsigned char key) {
    camera.setKeyState(key, false);
}

void Player::keyboard_down(unsigned char key) {
    camera.setKeyState(key, true);
}
    
void Player::mouse_motion(int mouse_x, int mouse_y) {
    camera.mouse_motion(mouse_x, mouse_y);
}

void Player::display() {

    //efeito visual de hit
    if(foiAtingido){
        glClearColor(0.3f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    camera.display();
    sword.desenhar(camera.get_position(), camera.get_pitch(), camera.get_yaw());
}

void Player::playerAttack(){
    if(!isAttacking){
        isAttacking = true;
    }
    
    sword.ataque();

    isAttacking = verificarAtaque();
}

bool Player::verificarAtaque(){
    if(sword.attacking){
        isAttacking = true;
    }else{
        isAttacking = false;
    }

    return isAttacking;
}

//funções relacionadas ao tratamento de colisões do player com os inimigos
void Player::takeHit(const Point& knockbackDir){
    health -= 1.0f;
    foiAtingido = true;
    hitTime = 0.3f;
    applyKnockback(knockbackDir);
}

void Player::applyKnockback(const Point& direction){
    velocity = Point(direction.getX() * knockback_force, 
    0.5f,
    direction.getZ() * knockback_force);
}

//debug: getter player position
string Player::get_str_position() const {
    return "Player: (" + std::to_string(camera.get_position().getX()) + ", "
        + std::to_string(camera.get_position().getY()) + ", "
        + std::to_string(camera.get_position().getZ()) + ")";
}

bool Player::is_alive() {
    return health > 0;
}

int Player::get_health() {
    return health;
}