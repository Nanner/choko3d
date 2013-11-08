#include "Plane.h"

GLfloat Plane::ctrlpoints[4][3] = {
                                    { -0.5, 0.0,  0.5},
                                    {  0.5, 0.0,  0.5},
                                    { -0.5, 0.0, -0.5},
                                    {  0.5, 0.0, -0.5}
                                };

GLfloat Plane::nrmlcompon[4][3] = {
                                {  0.0, 1.0, 0.0},
                                {  0.0, 1.0, 0.0},
                                {  0.0, 1.0, 0.0},
                                {  0.0, 1.0, 0.0} };

GLfloat Plane::colorpoints[4][4] = {
                                { 0.0, 1.0, 1.0, 0},
                                { 0.0, 0.0, 1.0, 0},
                                { 0.0, 1.0, 0.0, 0},
                                { 1.0, 0.0, 0.0, 0} };




Plane::Plane(int parts){
    matrix = NULL;
    this->parts = parts;
    this->texelsInitialized = false;
    this->previousFrontFace = new GLint();
}

void Plane::initializeTexels() {
    float s = 1.0; float t = 1.0;
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        s = 1.0 / appearance->getTexLength_s();
        t = 1.0 / appearance->getTexLength_t();
    }
    
    texels.push_back(0); texels.push_back(0);
    texels.push_back(s); texels.push_back(0);
    texels.push_back(0); texels.push_back(t);
    texels.push_back(s); texels.push_back(t);
}

void Plane::draw() {
    
    if (!texelsInitialized) initializeTexels();
    
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        appearance->apply();
    }
    
    glGetIntegerv(GL_FRONT_FACE, previousFrontFace);
    glFrontFace(GL_CW);
    
    glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	glDisable(GL_MAP2_COLOR_4);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    
    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &Plane::ctrlpoints[0][0]);
	glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &Plane::nrmlcompon[0][0]);
	//glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &Plane::colorpoints[0][0]);
    glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &texels[0]);

    
	glMapGrid2f(parts, 0.0, 1.0, parts, 0.0, 1.0);
    
    glEvalMesh2(GL_FILL, 0, parts, 0, parts);
    
    glFrontFace(*previousFrontFace);
    
    // prints vertex numbers for debugging purposes
    glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < 4; i++)
	{
		glRasterPos3f(ctrlpoints[i][0],ctrlpoints[i][1],ctrlpoints[i][2]);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'+i);
	}
    
}