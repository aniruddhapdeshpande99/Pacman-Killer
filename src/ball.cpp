#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, float radius, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    speedx = 0.01;
    speedy = 0.01;
    r = radius;
    GLfloat vertex_buffer_data[360*9];
	double angle_add = 0.0174533; //1 Degree in Radians
	double angle = 0.0;
	int i;
	for (i=0; i<360*9; i+=9)
	{
		vertex_buffer_data[i] = 0.0;
		vertex_buffer_data[i+1] = 0.0;
		vertex_buffer_data[i+2] = 0.0;

		vertex_buffer_data[i+3] = radius*cos(angle);
		vertex_buffer_data[i+4] = radius*sin(angle);
		vertex_buffer_data[i+5] = 0.0;

		vertex_buffer_data[i+6] = radius*cos(angle - angle_add);
		vertex_buffer_data[i+7] = radius*sin(angle - angle_add);
		vertex_buffer_data[i+8] = 0.0;

		angle += angle_add;
	}

    this->object = create3DObject(GL_TRIANGLES, 360*3 , vertex_buffer_data, color, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    this->position.x -= speedx;
    this->position.y -= speedy;
}

bounding_box_t Ball::bounding_box() {
    float x = this->position.x, y = this->position.y;
    float radius = this->r;
    bounding_box_t bbox = { x, y, radius, radius};
    return bbox;
}
