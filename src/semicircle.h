#include "main.h"

#ifndef SEMICIRCLE_H
#define SEMICIRCLE_H


class Semicircle {
public:
    Semicircle() {}
    Semicircle(float x, float y, float radius, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedx;
    double speedy;
    double r;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Semicircle_H
