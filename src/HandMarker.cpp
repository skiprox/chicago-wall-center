//
//  HandMarker.cpp
//  HandMarkers
//
//
//

#include "HandMarker.hpp"

HandMarker::HandMarker()
{
	center = glm::vec2(ofGetWidth()/2.0, ofGetHeight()/2.0);
	color = ofColor(240, 60, 40);
	isCircle = true;
    setup();
}

HandMarker::HandMarker(glm::vec2 _center, ofColor _color, bool _isCircle) {
	center = _center;
	color = _color;
	isCircle = _isCircle;
    setup();
}

void HandMarker::setup() {
	ofSetCircleResolution(100);
}

void HandMarker::update(float _inc) {
	inc = _inc;
}

void HandMarker::draw() {
	ofPushStyle();
	ofSetColor(color);
	float radius = 20 + 8 * sin(inc);
	if (isCircle) {
		ofFill();
		ofDrawCircle(center.x, center.y, radius);
		ofNoFill();
		ofSetColor(color);
		ofDrawCircle(center.x, center.y, 30);
	} else {
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofFill();
		ofDrawRectangle(center.x, center.y, radius, radius);
		ofNoFill();
		ofSetColor(color);
		ofDrawRectangle(center.x, center.y, 30, 30);
	}
	ofPopStyle();
}
