//
//  Line.cpp
//  Lines
//
//
//

#include "Line.hpp"

Line::Line()
{
	float width = ofGetWidth();
	float height = ofGetHeight();
	array<glm::vec2, 2> newPoints1 = {{glm::vec2(0, 0), glm::vec2(400, 400)}};
	pts.push_back(newPoints1);
	array<glm::vec2, 2> newPoints2 = {{glm::vec2(400, 400), glm::vec2(800, 100)}};
	pts.push_back(newPoints2);
	strokeWidth = 10.0;
	color = ofColor(240, 60, 40);
	incMax = 250;
	arrow = true;
    setup();
}

Line::Line(vector<std::array<glm::vec2, 2>> _pts, float _strokeWidth, ofColor _color, int _incMax, bool _arrow) {
	pts = _pts;
	strokeWidth = _strokeWidth;
	color = _color;
	incMax = _incMax;
	arrow = _arrow;
    setup();
}

void Line::setup() {
	inc = 0;
	numSegments = pts.size();
	path.setCurveResolution(100);
	ofSetCircleResolution(100);
}

void Line::update(int _inc) {
	inc = _inc;
}

void Line::draw() {
	ofPushStyle();
	path.clear();
	path.setStrokeWidth(strokeWidth);
	path.setStrokeColor(color);
	path.setFilled(false);
	for (int i = 0; i < numSegments; i++) {
		if (inc > (incMax/(numSegments) * i) && inc <= incMax/(numSegments) * (i + 1)) {
			for (int j = 0; j < i; j++) {
				path.moveTo(pts[j][0]);
				path.lineTo(pts[j][1]);
				path.close();
				path.draw();
			}
			path.moveTo(pts[i][0]);
			float upperEq = (float)inc - ((float)incMax/((float)numSegments) * i);
			float lowerEq = ((float)incMax/((float)numSegments) * ((float)i + 1.0)) - ((float)incMax/((float)numSegments) * i);
			float easedIncrement = quadEaseOut(upperEq/lowerEq);
			path.lineTo(ofLerp(pts[i][0].x, pts[i][1].x, easedIncrement), ofLerp(pts[i][0].y, pts[i][1].y, easedIncrement));
			path.close();
			path.draw();
		} else if (inc >= incMax) {
			path.moveTo(pts[i][0]);
			path.lineTo(pts[i][1]);
			path.close();
			path.draw();
			// We should draw an arrow here!
			if (i == (numSegments - 1) && arrow) {
				ofPushStyle();
				ofPushMatrix();
				ofSetColor(color);
				ofFill();
				ofTranslate(pts[i][1]);
				float slope = (pts[i][1].y - pts[i][0].y)/(pts[i][1].x - pts[i][0].x);
				float atanGuy = atan (slope);
				float angle = atanGuy * 180 / PI;
				// This seems to fix the issue of sometimes arrows going in wrong directions...
				// if our line is heading towards the left we flip the arrow
				if (pts[i][1].x > pts[i][0].x) {
					ofRotate(angle - 90);
				} else {
					ofRotate(angle + 90);
				}
				if (strokeWidth > 8) {
					ofDrawTriangle(glm::vec2(0 - 20, 0), glm::vec2(0, 20), glm::vec2(20, 0));
				} else if (strokeWidth > 3) {
					ofDrawTriangle(glm::vec2(0 - 14, 0), glm::vec2(0, 14), glm::vec2(14, 0));
				} else {
					ofDrawTriangle(glm::vec2(0 - 10, 0), glm::vec2(0, 10), glm::vec2(10, 0));
				}
				ofPopMatrix();
				ofPopStyle();
			}
		}
	}
	ofPopStyle();
}

//--------------------------------------------------------------
float Line::quadEaseOut(float t) {
	return -1.0 *(t)*(t-2);
}
