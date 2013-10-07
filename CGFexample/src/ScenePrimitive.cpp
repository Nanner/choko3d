#include "ScenePrimitive.h"
#include <iostream>

int Rectangle::rows = 10;
int Rectangle::columns = 10;

Rectangle::Rectangle(vector<float> xy1, vector<float> xy2) {
	matrix = NULL;
    
    x1 = xy1.at(0); y1 = xy1.at(1);
    x2 = xy2.at(0); y2 = xy2.at(1);
    
    if ( x1 > x2 && y1 > y2) {
        float x3 = x1, y3 = y1;
        this->x1 = x2; this->y1 = y2;
        this->x2 = x3; this->y2 = y3;
    }
    
    xScaled = 1;
    yScaled = 1;
    
    distX = this->x2 - this->x1;
    distY = this->y2 - this->y1;
    deltaX = distX / rows;
    deltaY = distY / columns;
    texDeltaX = 1.0 / rows;
    texDeltaY = 1.0 / columns;
}

void Rectangle::draw() {
    
    if( this->getAppearance() ) {
		Appearance* app = this->getAppearance();
        xScaled = (x2 - x1) / app->getTexLength_s();
		yScaled = (y2 - y1) / app->getTexLength_t();

	}
    
	glNormal3f(0,0,1);
    
    for (float bx = x1, tx = 0; bx < x2-0.01; bx += deltaX, tx += texDeltaX)
    {
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2d(tx * xScaled, 0);
        glVertex3f(bx, y1, 0.0);
        for (float by = y1, ty = 0; by < y2-0.01; by += deltaY, ty += texDeltaY)
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

bool Rectangle::operator==( const Rectangle &r2 ) const {
	return (this->x1 == r2.x1) && (this->y1 == r2.y1) &&
    (this->x2 == r2.x2) && (this->y2 == r2.y2);
}

bool Rectangle::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Rectangle* rect2 = dynamic_cast< const Rectangle* >( &p2 );
	return rect2 != NULL && *this == *rect2;
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
    float acN= sqrt( (x3-x1)*(x3-x1) + (y3-y1)*(y3-y1) + (z3-z1)*(z3-z1));
    float abN= sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1));
	float ab[3],ac[3];
    ab[0]=(x2-x1)/abN;  ab[1]=(y2-y1)/abN;  ab[2]=(z2-z1)/abN;
    ac[0]=(x3-x1)/acN;  ac[1]=(y3-y1)/acN;  ac[2]=(z3-z1)/acN;
    
    float cosAlfa = (ab[0]*ac[0]+ab[1]*ac[1]+ab[2]*ac[2]);
    float AD = acN*cosAlfa;
    float AE = acN*sin(acos(cosAlfa));
    
    Appearance* appearance = NULL;
    if( this->getAppearance() ) {
		appearance = this->getAppearance();
        
        texelBs = abN / appearance->getTexLength_s();
        texelCs = AD / appearance->getTexLength_s() ;
        texelCt = AE / appearance->getTexLength_t() ;
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

bool Triangle::operator==( const Triangle &t2 ) const {
	return (this->x1 == t2.x1) && (this->y1 == t2.y1) && (this->z1 == t2.z1) &&
    (this->x2 == t2.x2) && (this->y2 == t2.y2) && (this->z2 == t2.z2) &&
    (this->x3 == t2.x3) && (this->y3 == t2.y3) && (this->z3 == t2.z3);
}

bool Triangle::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Triangle* triangle2 = dynamic_cast< const Triangle* >( &p2 );
	return triangle2 != NULL && *this == *triangle2;
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

bool Cylinder::operator==( const Cylinder &c2 ) const {
	return (this->base == c2.base) && (this->top == c2.top) && (this->height == c2.height) && (this->slices == c2.slices) && (this->stacks == c2.stacks);
}

bool Cylinder::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Cylinder* cyl2 = dynamic_cast< const Cylinder* >( &p2 );
	return cyl2 != NULL && *this == *cyl2;
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

bool Sphere::operator==( const Sphere &s2 ) const {
	return (this->radius == s2.radius) && (this->slices == s2.slices) && (this->stacks == s2.stacks);
}

bool Sphere::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Sphere* sph2 = dynamic_cast< const Sphere* >( &p2 );
	return sph2 != NULL && *this == *sph2;
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

void Torus::draw(){
	glPushMatrix();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
    
	glutSolidTorus(inner, outer, slices, loops);
    
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
    
	glPopMatrix();
}

bool Torus::operator==( const Torus &t2 ) const {
	return (this->inner == t2.inner) && (this->outer == t2.outer) && (this->slices == t2.slices) && (this->loops == t2.loops);
}

bool Torus::isSamePrimitive( const ScenePrimitive &p2 ) const {
	const Torus* tor2 = dynamic_cast< const Torus* >( &p2 );
	return tor2 != NULL && *this == *tor2;
}

//TODO this may not be needed
//Based on the GLUT implementation
void solidTorus(GLfloat r, GLfloat R, GLint nsides, GLint rings) {
		int i, j;
		GLfloat theta, phi, theta1;
		GLfloat cosTheta, sinTheta;
		GLfloat cosTheta1, sinTheta1;
		GLfloat ringDelta, sideDelta;

		ringDelta = 2.0 * M_PI / rings;
		sideDelta = 2.0 * M_PI / nsides;

		theta = 0.0;
		cosTheta = 1.0;
		sinTheta = 0.0;
		for (i = rings - 1; i >= 0; i--) {
			theta1 = theta + ringDelta;
			cosTheta1 = cos(theta1);
			sinTheta1 = sin(theta1);
			glBegin(GL_QUAD_STRIP);
			phi = 0.0;
			for (j = nsides; j >= 0; j--) {
				GLfloat cosPhi, sinPhi, dist;

				phi += sideDelta;
				cosPhi = cos(phi);
				sinPhi = sin(phi);
				dist = R + r * cosPhi;

				glNormal3f(cosTheta1 * cosPhi, -sinTheta1 * cosPhi, sinPhi);
				glVertex3f(cosTheta1 * dist, -sinTheta1 * dist, r * sinPhi);
				glNormal3f(cosTheta * cosPhi, -sinTheta * cosPhi, sinPhi);
				glVertex3f(cosTheta * dist, -sinTheta * dist,  r * sinPhi);
			}
			glEnd();
			theta = theta1;
			cosTheta = cosTheta1;
			sinTheta = sinTheta1;
		}
}

