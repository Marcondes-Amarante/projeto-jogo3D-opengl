#ifndef OBJECT3D_HPP
#define OBJECT3D_HPP

#include <string>
#include <vector>
#include <map>
#include "point.hpp"

typedef struct {
    float x, y, z;
} Vertex;

typedef struct {
    float u, v;
} TexCoord;

typedef struct {
    float nx, ny, nz;
} Normal;

typedef struct {
    int vIdx, vtIdx, vnIdx;
    std::string material_name; 
} FaceIndex;

typedef struct {
    float Ka[3]; // ambiente
    float Kd[3]; // difusa
    float Ks[3]; // especular
    float Ns; // brilho
} Material;

typedef unsigned int GLuint;

class Object3D {
private:
    std::vector<Vertex> vertices;
    std::vector<TexCoord> texcoords;
    std::vector<Normal> normals;
    std::map<std::string, Material> materials;
    std::vector<std::vector<FaceIndex>> faces;
    GLuint textureID = 0;
    float scale = 1.0f;

    static unsigned int id_gen;
    unsigned int id;
    Point position;
    float color[3] = {0.5f, 1.0f, 0.0f};
    float diffuse_coef[3] = {0.5f, 1.0f, 0.0f};
    float specular_coef[3] = {1.0f, 1.0f, 1.0f};
    float shininess_coef = 10.0f;
    
public:
    Object3D();
    ~Object3D() = default;
    
    virtual void draw();
    virtual void setup_draw();
    bool load_model(std::string path);
    bool load_texture(const std::string& path);
    bool load_mtl(const std::string& path);
    float* get_diffuse_coef() const;
    float* get_specular_coef() const;
    float get_shininess_coef() const;
    void set_diffuse_coef(float coef_r, float coef_g, float coef_b);
    void set_specular_coef(float coef_r, float coef_g, float coef_b);
    void set_shininess_coef(float coef);
    const Point& get_position();
    void set_position(float x, float y, float z);
    void set_scale(float new_scale);
    void get_color(float &r, float &g, float &b);
    const float* get_color_vec();
    void set_color(float r, float g, float b);
    unsigned int get_id();
    bool equals(Object3D* obj);
    unsigned int get_VAO() const;
    const std::vector<unsigned int>& get_indices() const;
};

#endif