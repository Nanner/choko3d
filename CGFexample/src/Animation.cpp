#include "Animation.h"

Animation::Animation() {
    
}

void Animation::init(unsigned long t) {
    this->startTime = t;
    this->doReset = false;
}