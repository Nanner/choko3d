#include "Plane.h"

GLfloat ctrlpoints[4][3] = {
                                { -0.5, 0.0,  0.5},
                                {  0.5, 0.0,  0.5},
                                { -0.5, 0.0, -0.5},
                                {  0.5, 0.0, -0.5}
                            };

GLfloat nrmlcompon[4][3] = {	{  0.0, 1.0, 0.0},
                                {  0.0, 1.0, 0.0},
                                {  0.0, 1.0, 0.0},
                                {  0.0, 1.0, 0.0} };

GLfloat colorpoints[4][4] = {	{ 0.0, 1.0, 1.0, 0},
                                { 0.0, 0.0, 1.0, 0},
                                { 0.0, 1.0, 0.0, 0},
                                { 1.0, 0.0, 0.0, 0} };

GLfloat textpoints[4][2] = {	{ 0.0, 0.0},
                                { 0.0, 1.0},
                                { 1.0, 0.0},
                                { 1.0, 1.0} };


Plane::Plane(int parts){
    matrix = NULL;
    this->parts = parts;
}

void Plane::init() {
    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &ctrlpoints[0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon[0][0]);
	glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &colorpoints[0][0]);
    glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints[0][0]);

    
    glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glEnable(GL_MAP2_COLOR_4);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    
	glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
}

void Plane::draw() {
    this->init();
    glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
    
    //glPushMatrix();
    
    //glRotated(180, 1, 0, 0);
    
    glEvalMesh2(GL_FILL, 0, 10, 0, 10);
    
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        appearance->apply();
    }
    
    glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < 4; i++)
	{
		glRasterPos3f(ctrlpoints[i][0],ctrlpoints[i][1],ctrlpoints[i][2]+0.5);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'+i);
	}
    
    glDisable(GL_COLOR_MATERIAL);
    
    //glPopMatrix();
}