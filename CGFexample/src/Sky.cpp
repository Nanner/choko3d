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