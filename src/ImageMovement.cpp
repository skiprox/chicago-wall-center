//
//  ImageMovement.cpp
//  ImageMovements
//
//
//

#include "ImageMovement.hpp"

//--------------------------------------------------------------
ImageMovement::ImageMovement()
{
	startPos = glm::vec2(0, 0);
	endPos = glm::vec2(ofGetWidth(), ofGetHeight());
	size = glm::vec2(100, 100);
    setup();
}

//--------------------------------------------------------------
ImageMovement::ImageMovement(ofImage _img, glm::vec2 _size, glm::vec2 _startPos, glm::vec2 _endPos, int _incMax) {
	img = _img;
	size = _size;
	startPos = _startPos;
	endPos = _endPos;
	incMax = _incMax;
    setup();
}

//--------------------------------------------------------------
void ImageMovement::setup() {
	ofSetCircleResolution(100);
	inc = 0;
}

//--------------------------------------------------------------
void ImageMovement::update(int _inc) {
	inc = _inc;
}

//--------------------------------------------------------------
void ImageMovement::draw() {
	ofPushMatrix();
	ofPushStyle();
	float easedIncrement = quadEaseOut(ofClamp((float)inc/(float)incMax, 0, 1.0));
	float newX = ofLerp(startPos.x, endPos.x, easedIncrement);
	float newY = ofLerp(startPos.y, endPos.y, easedIncrement);
	ofTranslate(newX, newY);
	img.draw(-size.x/2.0, -size.y/2.0, size.x, size.y);
	ofPopStyle();
	ofPopMatrix();
}

//--------------------------------------------------------------
float ImageMovement::quadEaseOut(float t) {
	return -1.0 *(t)*(t-2);
}
