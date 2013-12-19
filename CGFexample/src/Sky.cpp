#include <gl/glew.h> //For GL_CLAMP_TO_EDGE
#include "Sky.h"

Sky::Sky() {
    matrix = NULL;
}

void Sky::draw() {
    glPushAttrib(GL_ENABLE_BIT);
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_BLEND);
    
	glPushMatrix();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glNormal3d(0,0,-1);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-HALF_SIZE, -HALF_SIZE, 0.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-HALF_SIZE,  HALF_SIZE, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( HALF_SIZE,  HALF_SIZE, 0.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( HALF_SIZE, -HALF_SIZE, 0.0f);
    glEnd();
	glPopMatrix();
    
    glPopAttrib();
}