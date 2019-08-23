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
    setup();
}

HandMarker::HandMarker(glm::vec2 _center, ofColor _color) {
	center = _center;
	color = _color;
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
	float radius = 30 + 10 * sin(inc);
	ofFill();		// draw "filled shapes"
	ofDrawCircle(center.x, center.y, radius);
	ofNoFill();
	ofSetColor(color);
	ofDrawCircle(center.x, center.y, 44);
	ofPopStyle();
}
