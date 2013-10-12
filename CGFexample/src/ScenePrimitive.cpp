#include "ScenePrimitive.h"
#include <iostream>

int Rectangle::rows = 10;
int Rectangle::columns = 10;

Rectangle::Rectangle(vector<float> xy1, vector<float> xy2) {
	matrix = NULL;
    
    x1 = xy1.at(0); y1 = xy1.at(1);
    x2 = xy2.at(0); y2 = xy2.at(1);
    
    xScaled = 1;
    yScaled = 1;
    
    distX = this->x2 - this->x1;
    distY = this->y2 - this->y1;
    deltaX = distX / rows;
    deltaY = distY / columns;
    texDeltaX = 1.0 / rows;
    texDeltaY = 1.0 / columns;
}

/*
 Auxiliary function to help tesselation
 */
bool reachedEnd(float start, float end, int direction) {
    if ( direction < 0 )
        return start < (end + MARGIN);
    
    if (direction > 0)
        return start > (end - MARGIN);
    
    return false;
}

void Rectangle::draw() {
    
    if( this->getAppearance() ) {
		Appearance* app = this->getAppearance();
        xScaled = fabs(x2 - x1) / app->getTexLength_s();
		yScaled = fabs(y2 - y1) / app->getTexLength_t();
	}
    
	glNormal3f(0,0,1);
    
    for (float bx = x1, tx = 0; !reachedEnd(bx, x2, distX); bx += deltaX, tx += texDeltaX)
    {
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2d(tx * xScaled, 0);
        glVertex3f(bx, y1, 0.0);
        for (float by = y1, ty = 0; !reachedEnd(by, y2, distY); by += deltaY, ty += texDeltaY)
        {
            glTexCoord2d( (tx + texDeltaX) * xScaled ,  ty * yScaled);
            glVertex3f(bx + deltaX, by, 0.0);
            
            glTexCoord2d( tx * xScaled, (ty + texDeltaY) * yScaled);
            glVertex3f(bx, by + deltaY, 0.0);
        }
        glTexCoord2d( (tx + texDeltaX) * xScaled, 1.0 * yScaled);
        glVertex3d(bx+deltaX, y2, 0.0);
        
        glEnd();
    }
}

Triangle::Triangle(vector<float> xyz1, vector<float> xyz2, vector<float> xyz3) {
	matrix = NULL;
    x1 = xyz1.at(0); x2 = xyz2.at(0); x3 = xyz3.at(0);
    y1 = xyz1.at(1); y2 = xyz2.at(1); y3 = xyz3.at(1);
    z1 = xyz1.at(2); z2 = xyz2.at(2); z3 = xyz3.at(2);

    this->calculateNormal();
    
    this->texelsReady = false;
}

void Triangle::calculateNormal(){
    float Ux = ( x2 - x1 );
    float Uy = ( y2 - y1 );
    float Uz = ( z2 - z1 );
    
    float Vx = ( x3 - x1 );
    float Vy = ( y3 - y1 );
    float Vz = ( z3 - z3 );
    
    xn = (Uy * Vz) - (Uz * Vy);
    yn = (Uz * Vx) - (Ux * Vz);
    zn = (Ux * Vy) - (Uy * Vx);
}

void Triangle::calculateTexels() {
	//First we calculate the distances between points A and B and points A and C
    float AC= sqrt((x3-x1)*(x3-x1) + (y3-y1)*(y3-y1) + (z3-z1)*(z3-z1));
    float AB= sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));

	//Now we calculate vectors ab and ac and normalize them
	float ab[3],ac[3];
    ab[0]=(x2-x1)/AB;  ab[1]=(y2-y1)/AB;  ab[2]=(z2-z1)/AB;
    ac[0]=(x3-x1)/AC;  ac[1]=(y3-y1)/AC;  ac[2]=(z3-z1)/AC;
    
    //Using the dot product between those two previous vectors, we can determine the cosine of the alpha angle,
	//which we assume being the angle between AB and AC
	float cosAlpha = (ab[0]*ac[0]+ab[1]*ac[1]+ab[2]*ac[2]);
	//Therefore,
	float alpha = acos(cosAlpha);

	//Now, using the always useful trigonometry, we can assume a point D such that ABD is a right triangle,
	//and calculate the distances between AD and BD (note here that depending on the triangle vertices,
	//the AD/BD notations may not make much sense, but the calculations still apply)
    float AD = AC*cosAlpha;
    float BD = AC*sin(alpha);
    
	//We can now use the calculated AD and BD distances to determine how to map the C point texel s and t coordinates,
	//also bearing in mind the texlength s and t of the texture being used
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        
        texelBs = AB / appearance->getTexLength_s();
        texelCs = AD / appearance->getTexLength_s() ;
        texelCt = BD / appearance->getTexLength_t() ;
	}
    
    this->texelsReady = true;
}

void Triangle::draw() {
    
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        
        if ( !texelsReady ) calculateTexels();
	}
    
	glBegin(GL_TRIANGLES);
    glNormal3f(xn, yn, zn);
    if(appearance) glTexCoord2f(0.0, 0.0);
    glVertex3f(x1, y1, z1);
    if(appearance) glTexCoord2f(texelBs, 0.0);
    glVertex3f(x2, y2, z2);
    if(appearance) glTexCoord2f(texelCs, texelCt );
    glVertex3f(x3, y3, z3);
    glEnd();
}

Cylinder::Cylinder(float base,
                   float top,
                   float height,
                   float slices,
                   float stacks) {
	matrix = NULL;
    this->base = base;
    this->top = top;
    this->height = height;
    this->slices = slices;
    this->stacks = stacks;
    
	quadratic = gluNewQuadric();
	gluQuadricTexture(quadratic, GL_TRUE);
}

void Cylinder::draw() {
	gluCylinder(quadratic, base, top, height, slices, stacks);
	glPushMatrix();
	glRotatef(180.0, 0.0, 1.0, 0.0);
	gluDisk(quadratic, 0, base, slices, stacks);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0, 0.0, height);
	gluDisk(quadratic, 0.0, top, slices, stacks);
	glPopMatrix();
}


Sphere::Sphere(float radius, int slices, int stacks) {
	matrix = NULL;
    this->radius = radius;
    this->slices = slices;
    this->stacks = stacks;
    
	quad = gluNewQuadric();
}

void Sphere::draw(){
	gluQuadricTexture(quad,1);

	gluSphere(quad, radius, slices, stacks);
}

Torus::Torus(float inner, float outer, int slices, int loops) {
	matrix = NULL;
    this->inner = inner;
    this->outer = outer;
    this->slices = slices;
    this->loops = loops;
    
	glTexGenf(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGenf(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
}

void Torus::draw(){glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
    
	glutSolidTorus(inner, outer, slices, loops);
    
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

