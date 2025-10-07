#include <cmath>
#include "point.hpp"
#include "camera.hpp"

#define M_PI 3.14159265359f
#define SENSITIVITY 0.1f
#define SPEED 3.0f

#define LIMITX 10
#define LIMITZ 8

int last_mouse_x = -1;
int last_mouse_y = -1;


bool keyState[256] = {false};

float dist(float x1, float y1, float x2, float y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

Camera::Camera(float x, float y, float z, float pitch, float yaw) {
    this->position = Point(x, y, z);
    this->look_at = Point(0, 1.7, 0);
    this->pitch = pitch;
    this->yaw = yaw;
}

Camera::Camera() : position(Point(0, 0, 0)), pitch(0), yaw(0) {}

void Camera::update_cam_direction() {
    float rad_yaw = yaw * M_PI / 180.0f;
    float rad_pitch = pitch * M_PI / 180.0f;

    look_at.setX(position.getX() + cos(rad_yaw) * cos(rad_pitch));
    look_at.setY(position.getY() + sin(rad_pitch));
    look_at.setZ(position.getZ() + sin(rad_yaw) * cos(rad_pitch));
}

void Camera::mouse_motion(int x, int y) {
    int center_x = 800 / 2;
    int center_y = 600 / 2;
    if (last_mouse_x == -1) {
        glutWarpPointer(center_x, center_y);
        last_mouse_x = x;
        last_mouse_y = y;
    }

    float delta_x = x - last_mouse_x;
    float delta_y = last_mouse_y - y;

    last_mouse_x = x;
    last_mouse_y = y;

    yaw += delta_x * SENSITIVITY;
    pitch += delta_y * SENSITIVITY;

    // Limita o pitch para evitar virar de cabeça para baixo
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    update_cam_direction();
    glutWarpPointer(center_x, center_y);
    last_mouse_x = center_x;
    last_mouse_y = center_y;
    glutPostRedisplay();
}

void Camera::setKeyState(unsigned char key, bool value){
    keyState[key] = value;
}

void Camera::update(float deltaTime) {
    float rad_yaw = yaw * M_PI / 180.0f;
    float moveSpeed = SPEED * deltaTime;

    if(keyState['w'] || keyState['W']){
        position.setX(position.getX() + moveSpeed * cos(rad_yaw));
        position.setZ(position.getZ() + moveSpeed * sin(rad_yaw));
    }
    if(keyState['s'] || keyState['S']){
        position.setX(position.getX() - moveSpeed * cos(rad_yaw));
        position.setZ(position.getZ() - moveSpeed * sin(rad_yaw));
    }
    if(keyState['d'] || keyState['D']){
        position.setX(position.getX() - moveSpeed * sin(rad_yaw));
        position.setZ(position.getZ() + moveSpeed * cos(rad_yaw));
    }
    if(keyState['a'] || keyState['A']){
        position.setX(position.getX() + moveSpeed * sin(rad_yaw));
        position.setZ(position.getZ() - moveSpeed * cos(rad_yaw));
    }

    if(position.getX() > LIMITX) position.setX(LIMITX);
    if(position.getX() < -LIMITX) position.setX(-LIMITX);
    if(position.getZ() > LIMITZ) position.setZ(LIMITZ);
    if(position.getZ() < -LIMITZ) position.setZ(-LIMITZ);

    update_cam_direction();
    glutPostRedisplay();
}

void Camera::display() {
    gluLookAt(
        position.getX(), position.getY(), position.getZ(),
        look_at.getX(), look_at.getY(), look_at.getZ(),
        0.0f, 1.0f, 0.0f
    );
}

float Camera::get_pitch() const {
    return pitch;
}

float Camera::get_yaw() const {
    return yaw;
}

const Point& Camera::get_position() const {
    return position;
}

Point Camera::get_direction() const {
    return Point(
        look_at.getX() - position.getX(),
        look_at.getY() - position.getY(),
        look_at.getZ() - position.getZ()
    );
}

string Camera::get_str_position() {
    return "Camera: (" + std::to_string(position.getX()) + ", "
        + std::to_string(position.getY()) + ", "
        + std::to_string(position.getZ()) + ")";
}

void Camera::set_position(float x, float y, float z){

    position.setX(x);
    position.setY(y);
    position.setZ(z);

    update_cam_direction();
    glutPostRedisplay();
}


