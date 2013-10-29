#include "Waterline.h"

Waterline::Waterline(string heightmap, string texturemap, string fragmentshader, string vertexshader) {
    matrix = NULL;
    this->shader = new WaterShader(heightmap, texturemap, fragmentshader, vertexshader);
    this->plane = new Plane(50);
    plane->setAppearance(NULL);
    plane->setAnimation(NULL);
    
}

void Waterline::draw() {
    shader->bind();
    plane->draw();
    shader->unbind();
}
