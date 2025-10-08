#ifndef CAMERA_MAP_HPP
#define CAMERA_MAP_HPP

#include <string>
using namespace std;
#include <GL/glut.h>
#include "point.hpp"

class Camera {
private:
    Point position;
    Point look_at;
    float pitch;
    float yaw;

public:
    Camera();
    Camera(float x, float y, float z, float pitch, float yaw);
    ~Camera() = default;

    bool keyState[256] = { false };

    void update_cam_direction();
    void mouse_motion(int x, int y);
    void update(float deltaTime);

    void setKeyState(unsigned char key, bool value);

    void display();
    float get_pitch() const;
    float get_yaw() const;
    const Point& get_position() const;
    
    Point get_direction() const;
    
    std::string get_str_position();

    void set_position(float x, float y, float z);
};

#endif