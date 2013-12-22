#include "Appearance.h"

int Appearance::currentTextIndex = 0;
unsigned int Appearance::currentID = 0;

float Appearance::getTexLength_s() {
	return texLength_s;
}

float Appearance::getTexLength_t() {
	return texLength_t;
}

void Appearance::setTexture(string t) {
	CGFappearance::setTexture(t);
	currentTextIndex++;
}

void Appearance::setTexLength_s(float s) {
	texLength_s = s;
}

void Appearance::setTexLength_t(float t) {
	texLength_t = t;
}

void Appearance::apply() {
	CGFappearance::apply();
	glMaterialfv(GL_FRONT, GL_EMISSION, emissivity);
}

Appearance::Appearance(float* a, float* d, float* sp, float* emis, float sh):CGFappearance(a, d, sp, sh) {
	emissivity[0] = emis[0];
	emissivity[1] = emis[1];
	emissivity[2] = emis[2];
	emissivity[3] = emis[3];
	id = currentID;
	currentID++;
}

Appearance::~Appearance() {}
