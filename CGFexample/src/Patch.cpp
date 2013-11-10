#include "Patch.h"

Patch::Patch(int order, int partsU, int partsV, string compute, vector<float> controlPoints) {
    matrix = NULL;
    this->texelsInitialized = false;
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
    
    previousFrontFace = new GLint();
}

void Patch::initializeTexels() {
    float s = 1.0; float t = 1.0;
    Appearance* appearance = NULL;
    if(this->getAppearance() != NULL) {
		appearance = this->getAppearance();
        s = 1.0 / appearance->getTexLength_s();
        t = 1.0 / appearance->getTexLength_t();
    }
    
    texels.push_back(0); texels.push_back(0);
    texels.push_back(s); texels.push_back(0);
    texels.push_back(0); texels.push_back(t);
    texels.push_back(s); texels.push_back(t);
}

void Patch::draw() {
    
    if (!texelsInitialized) initializeTexels();
    
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        appearance->apply();
    }

    glEnable(GL_AUTO_NORMAL);
    
    glGetIntegerv(GL_FRONT_FACE, previousFrontFace);
    glFrontFace(GL_CW);
    
    glEnable(GL_MAP2_TEXTURE_COORD_2);
    
    glEnable(GL_MAP2_VERTEX_3);
    glDisable(GL_MAP2_NORMAL);

    glMap2f(GL_MAP2_VERTEX_3, 0.0, 1.0, 3, order+1,  0.0, 1.0, (order+1)*3, order+1,  &controlPoints[0]);
    glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &Patch::texels[0]);

	glMapGrid2f(partsU, 0.0, 1.0, partsV, 0.0, 1.0);
    
    glEvalMesh2(compute, 0, partsU, 0, partsV);
    
    glDisable(GL_AUTO_NORMAL);
    
    glFrontFace(*previousFrontFace);
}
