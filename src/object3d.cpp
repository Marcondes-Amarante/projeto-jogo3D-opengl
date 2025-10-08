#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "object3d.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int Object3D::id_gen = 0;

Object3D::Object3D(): id(id_gen++) {}

void Object3D::draw() {
    glDisable(GL_LIGHTING);
    glPushMatrix();
        glTranslated(
            get_position().getX(), 
            get_position().getY(),
            get_position().getZ()
        );
        setup_draw();
    glPopMatrix();
    glEnable(GL_LIGHTING);
}

void Object3D::setup_draw() {
    glScalef(get_scale(), get_scale(), get_scale());

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glBegin(GL_TRIANGLES);
    for (const auto& face : faces) {
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,  get_diffuse_coef());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, get_specular_coef());
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, get_shininess_coef());
    
        for (const auto& f : face) {
            if (f.vtIdx >= 0 && f.vtIdx < texcoords.size())
                glTexCoord2f(texcoords[f.vtIdx].u, texcoords[f.vtIdx].v);
            if (f.vnIdx >= 0 && f.vnIdx < normals.size())
                glNormal3f(normals[f.vnIdx].nx, normals[f.vnIdx].ny, normals[f.vnIdx].nz);

            glVertex3f(vertices[f.vIdx].x, vertices[f.vIdx].y, vertices[f.vIdx].z);
        }
    }
    glEnd();
}

bool Object3D::load_model(std::string path) {
    std::ifstream file_obj(path);
    if (!file_obj.is_open()) {
        std::cerr << "Erro ao abrir o arquivo OBJ: " << path << std::endl;
        return false;
    }

    std::string currentMaterialName;
    std::string linha;
    while (std::getline(file_obj, linha)) {
        std::istringstream ss(linha);
        std::string tipo;
        ss >> tipo;

        if (tipo == "v") {
            Vertex v;
            ss >> v.x >> v.y >> v.z;
            vertices.push_back(v);
        } 
        else if (tipo == "vt") {
            TexCoord t;
            ss >> t.u >> t.v;
            t.v = 1.0f - t.v;  
            texcoords.push_back(t);
        }
        else if (tipo == "vn") {
            Normal n;
            ss >> n.nx >> n.ny >> n.nz;
            normals.push_back(n);
        }
        else if (tipo == "f") {
            std::vector<FaceIndex> face;
            std::string vertStr;
            while (ss >> vertStr) {
                FaceIndex f{};
                int matches = sscanf(
                    vertStr.c_str(),
                    "%d/%d/%d",
                    &f.vIdx,
                    &f.vtIdx,
                    &f.vnIdx
                );

                if (matches == 1) { 
                    f.vtIdx = f.vnIdx = -1;
                } else if (matches == 2) { 
                    f.vnIdx = -1;
                }

                f.vIdx--;  
                if (f.vtIdx > 0) f.vtIdx--;
                if (f.vnIdx > 0) f.vnIdx--;
                f.material_name = currentMaterialName;
                face.push_back(f);
            }
            faces.push_back(face);
        } else if (tipo == "usemtl") {
            ss >> currentMaterialName;
        }
    }

    file_obj.close();

    return true;
}

bool Object3D::load_texture(const std::string& path) {
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        std::cerr << "Erro ao carregar textura: " << path << std::endl;
        return false;
    }

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Configura filtros e repetição
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    gluBuild2DMipmaps(GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    return true;
}

bool Object3D::load_mtl(const std::string& path) {
    std::ifstream file_mtl(path);
    if (!file_mtl.is_open()) {
        std::cerr << "Erro ao abrir o arquivo MTL: " << path << std::endl;
        return false;
    }

    std::string line;
    std::string currentMatName;
    Material mat;

    while (std::getline(file_mtl, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "newmtl") {
            if (!currentMatName.empty()) {
                materials[currentMatName] = mat;
            }
            ss >> currentMatName;
            mat = Material();
            mat.name = currentMatName;
        } 
        else if (type == "Ka") ss >> mat.Ka[0] >> mat.Ka[1] >> mat.Ka[2];
        else if (type == "Kd") ss >> mat.Kd[0] >> mat.Kd[1] >> mat.Kd[2];
        else if (type == "Ks") ss >> mat.Ks[0] >> mat.Ks[1] >> mat.Ks[2];
        else if (type == "Ns") ss >> mat.Ns;
    }

    if (!currentMatName.empty()) {
        materials[currentMatName] = mat;
    }

    file_mtl.close();
    return true;
}

bool Object3D::equals(Object3D* obj) {
    return this->id == obj->id;
}

float* Object3D::get_diffuse_coef() const {
    return const_cast<float*>(diffuse_coef);
}

float* Object3D::get_specular_coef() const {
    return const_cast<float*>(specular_coef);
}

float Object3D::get_shininess_coef() const {
    return shininess_coef;
}

const Point& Object3D::get_position() {
    return position;
}

float Object3D::get_scale() {
    return scale;
}

void Object3D::get_color(float &r, float &g, float &b) {
    r = color[0], g = color[1], b = color[2];
}

const float* Object3D::get_color_vec() {
    return color;
}

unsigned int Object3D::get_id() {
    return this->id;
}

// ===== Setters =====

void Object3D::set_diffuse_coef(float coef_r, float coef_g, float coef_b) {
    diffuse_coef[0] = coef_r;
    diffuse_coef[1] = coef_g;
    diffuse_coef[2] = coef_b;
}

void Object3D::set_specular_coef(float coef_r, float coef_g, float coef_b) {
    specular_coef[0] = coef_r;
    specular_coef[1] = coef_g;
    specular_coef[2] = coef_b;
}

void Object3D::set_shininess_coef(float coef) {
    shininess_coef = coef;
}

void Object3D::set_position(float x, float y, float z) {
    position.setX(x);
    position.setY(y);
    position.setZ(z);
}

void Object3D::set_color(float r, float g, float b) {
    color[0] = r, color[1] = g, color[2] = b;
}

void Object3D::set_scale(float new_scale) {
    this->scale = new_scale;
}