#include "main.h"

#ifndef LEG_H
#define LEG_H


class Leg {
public:
    Leg() {}
    Leg(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speedx;
    double speedy;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif //LEG_H
