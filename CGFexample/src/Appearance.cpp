#include "Appearance.h"

float Appearance::getTexLength_s() {
	return texLength_s;
}


float Appearance::getTexLength_t() {
	return texLength_t;
}

void Appearance::setTexLength_s(float s) {
	texLength_s = s;
}

void Appearance::setTexLength_t(float t) {
	texLength_t = t;
}

Appearance::Appearance(float* a, float* d, float* sp, float sh):CGFappearance(a, d, sp, sh) {}