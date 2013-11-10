#include "Waterline.h"

GLfloat Waterline::ctrlpoints[10][3] = {
	{ -0.5, 0.0,  2.0},
	{  0.5, 0.0,  2.0},
	{ -0.5, 0.0,  1.0},
	{  0.5, 0.0,  1.0},
	{ -0.5, 0.0,  0.0},
	{  0.5, 0.0,  0.0},
	{ -0.5, 0.0, -1.0},
	{  0.5, 0.0, -1.0},
	{ -0.5, 0.0, -2.0},
	{  0.5, 0.0, -2.0}
};

GLfloat Waterline::nrmlcompon[10][3] = {
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0},
	{  0.0, 1.0, 0.0}
};

GLfloat Waterline::texels[10][2] = {
	{0.0, 0.0},
	{1.0, 0.0},
	{0.0, 1.0},
	{1.0, 1.0},
	{0.0, 2.0},
	{1.0, 2.0},
	{0.0, 3.0},
	{1.0, 3.0},
	{0.0, 4.0},
	{1.0, 4.0}
};

Waterline::Waterline(string heightmap, string texturemap, string fragmentshader, string vertexshader) {
    matrix = NULL;
    this->shader = new WaterShader(heightmap, texturemap, fragmentshader, vertexshader);
}

void Waterline::draw() {
    shader->bind();
    drawEval();
    shader->unbind();
}

void Waterline::drawEval() {
	glGetIntegerv(GL_FRONT_FACE, &previousFrontFace);
	glFrontFace(GL_CW);

	glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glDisable(GL_MAP2_COLOR_4);
	glEnable(GL_MAP2_TEXTURE_COORD_2);

	glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 5,  &Waterline::ctrlpoints[0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 5,  &Waterline::nrmlcompon[0][0]);
	glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 5,  &Waterline::texels[0][0]);


	glMapGrid2f(WATERLINE_EVAL_W_PARTS, 0.0, 1.0, WATERLINE_EVAL_H_PARTS, 0.0, 1.0);

	glEvalMesh2(GL_FILL, 0, WATERLINE_EVAL_W_PARTS, 0, WATERLINE_EVAL_H_PARTS);

	glFrontFace(previousFrontFace);
}

WaterShader* Waterline::getShader() {
	return shader;
}
