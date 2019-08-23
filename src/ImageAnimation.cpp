//
//  ImageAnimation.cpp
//  ImageAnimations
//
//
//

#include "ImageAnimation.hpp"

//--------------------------------------------------------------
ImageAnimation::ImageAnimation()
{
	pos = glm::vec2(0, 0);
	size = glm::vec2(0, 0);
	incMax = 1;
    setup();
}

//--------------------------------------------------------------
ImageAnimation::ImageAnimation(ofImage _img, glm::vec2 _pos, glm::vec2 _size, int _incMax) {
	img = _img;
	pos = _pos;
	size = _size;
	incMax = _incMax;
    setup();
}

//--------------------------------------------------------------
void ImageAnimation::setup() {
	inc = 0;
}

//--------------------------------------------------------------
void ImageAnimation::update(int _inc) {
	inc = _inc;
}

//--------------------------------------------------------------
void ImageAnimation::draw() {
	ofPushStyle();
	ofPushMatrix();
	// The *~*magic*~*
	ofTranslate(pos);
	float easedValue = quadEaseOut(ofClamp(float(inc)/float(incMax), 0.0, 1.0));
	// Change the opacity
	ofSetColor(255, 255, 255, ofMap(easedValue, 0.0, 1.0, 0, 255));
	img.draw(-size.x/2.0, -size.y/2.0, size.x, size.y);
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
float ImageAnimation::bounceEaseOut(float t) {
	if (t < (1/2.75f)) {
		return (7.5625f*t*t);
	} else if (t < (2/2.75f)) {
		float postFix = t-=(1.5f/2.75f);
		return (7.5625f*(postFix)*t + .75f);
	} else if (t < (2.5/2.75)) {
			float postFix = t-=(2.25f/2.75f);
		return (7.5625f*(postFix)*t + .9375f);
	} else {
		float postFix = t-=(2.625f/2.75f);
		return (7.5625f*(postFix)*t + .984375f);
	}
}

//--------------------------------------------------------------
float ImageAnimation::quadEaseOut(float t) {
	return -1.0 *(t)*(t-2);
}
