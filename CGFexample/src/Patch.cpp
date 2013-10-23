#include "Patch.h"

GLfloat nrmlcompon2[4][3] = {	{  0.0, -1.0, 0.0},
                                {  0.0, -1.0, 0.0},
                                {  0.0, -1.0, 0.0},
                                {  0.0, -1.0, 0.0} };

GLfloat colorpoints2[4][4] = {	{ 0.0, 1.0, 1.0, 0},
                                { 0.0, 0.0, 1.0, 0},
                                { 0.0, 1.0, 0.0, 0},
                                { 1.0, 0.0, 0.0, 0} };

GLfloat textpoints2[4][2] = {	{ 0.0, 1.0},
                                { 1.0, 1.0},
                                { 0.0, 0.0},
                                { 1.0, 0.0} };


Patch::Patch(int order, int partsU, int partsV, string compute, vector<float> controlPoints) {
    matrix = NULL;
    this->order = order;
    
    this->partsU = partsU;
    this->partsV = partsV;

    int controlPointsSize = (order+1)*(order+1) * 3;
    
    if (controlPointsSize != controlPoints.size() ) {
        char errorMessage[ERROR_LEN];
        sprintf(errorMessage, "Order (patch) number of controls points in patch, expected %i, found %lu", controlPointsSize, controlPoints.size());
		throw(InvalidAttributeValueException(errorMessage));
    }
    
    this->controlPoints = controlPoints;
    
    if (compute.compare("point") == 0 )
        this->compute = GL_POINT;
    else if (compute.compare("line") == 0)
        this->compute = GL_LINE;
    else if (compute.compare("fill") == 0)
        this->compute = GL_FILL;
    else {
        char errorMessage[ERROR_LEN];
        sprintf(errorMessage, "Compute (patch) expected \"point\", \"line\" or \"fill\", got %s", compute.c_str());
		throw(InvalidAttributeValueException(errorMessage));
    }
    
}

void Patch::init() {
    // TODO what the hell do the teachers want with the order?!?
    
    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order,  0.0, 1.0, 6, order,  &controlPoints[0]);
	//glMap2f(GL_MAP2_NORMAL,   0.0, 1.0, 3, 2,  0.0, 1.0, 6, 2,  &nrmlcompon2[0][0]);
	//glMap2f(GL_MAP2_COLOR_4,  0.0, 1.0, 4, 2,  0.0, 1.0, 8, 2,  &colorpoints2[0][0]);
    //glMap2f(GL_MAP2_TEXTURE_COORD_2,  0.0, 1.0, 2, 2,  0.0, 1.0, 4, 2,  &textpoints2[0][0]);

    
    glEnable(GL_MAP2_VERTEX_3);
	glEnable(GL_MAP2_NORMAL);
	//glEnable(GL_MAP2_COLOR_4);
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    
	glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);
}

void Patch::draw() {
    this->init();
    
    glPushMatrix();
    
    // TODO i think glrotated is an incorrect aproach,
    // because it messes with the control points
    // maybe changing the cullface?
    glRotated(180, 1, 0, 0);
    
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        appearance->apply();
    }
    
    glEvalMesh2(compute, 0, partsU, 0, partsV);

    
     // prints vertex numbers for debugging purposes
    glColor3f(1.0, 1.0, 0.0);
	for (int i = 0; i < controlPoints.size(); i+=3)
	{
		glRasterPos3f(controlPoints[i], controlPoints[i+1], controlPoints[i+2]);
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, '0'+i/3);
	}
    
    
    glPopMatrix();

}
