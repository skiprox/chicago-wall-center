//
//  DottedLine.cpp
//  DottedLines
//
//
//

#include "DottedLine.hpp"

DottedLine::DottedLine()
{
	array<glm::vec2, 2> newPoints1 = {{glm::vec2(0, 0), glm::vec2(400, 400)}};
	pts.push_back(newPoints1);
	array<glm::vec2, 2> newPoints2 = {{glm::vec2(400, 400), glm::vec2(800, 100)}};
	pts.push_back(newPoints2);
	circleRadius = 10.0;
	color = ofColor(240, 60, 40);
	incMax = 250;
	arrow = true;
    setup();
}

DottedLine::DottedLine(vector<std::array<glm::vec2, 2>> _pts, float _circleRadius, ofColor _color, int _incMax, bool _arrow) {
	pts = _pts;
	circleRadius = _circleRadius;
	color = _color;
	incMax = _incMax;
	arrow = _arrow;
    setup();
}

void DottedLine::setup() {
	inc = 0;
	numSegments = pts.size();
	ofSetCircleResolution(100);
	desiredStrokeLen = circleRadius * 4.0;
	inc = 0;
}

void DottedLine::update(int _inc) {
	inc = _inc;
}

void DottedLine::draw() {
	ofPushStyle();
	for (int i = 0; i < numSegments; i++) {
		if (inc > (incMax/(numSegments) * i) && inc <= incMax/(numSegments) * (i + 1)) {
			for (int j = 0; j < i; j++) {
				float numLineSegments = floor(fabs(ofDist(pts[j][0].x, pts[j][0].y, pts[j][1].x, pts[j][1].y))/desiredStrokeLen);
				float distX = pts[j][0].x - pts[j][1].x;
				float distY = pts[j][0].y - pts[j][1].y;
				float segmentLenX = distX/((float)numLineSegments)/2.0;
				float segmentLenY = distY/((float)numLineSegments)/2.0;
				for (int k = 0; k < numLineSegments; k++) {
					float posX = ofLerp(pts[j][0].x, pts[j][1].x, (k + 1)/numLineSegments);
					float posY = ofLerp(pts[j][0].y, pts[j][1].y, (k + 1)/numLineSegments);
					ofSetColor(color);
					ofFill();
					ofDrawCircle(posX, posY, circleRadius);
				}
			}
			float upperEq = (float)inc - ((float)incMax/((float)numSegments) * i);
			float lowerEq = ((float)incMax/((float)numSegments) * ((float)i + 1.0)) - ((float)incMax/((float)numSegments) * i);
			float easedIncrement = sineEaseOut(upperEq/lowerEq);
			float totalDist = ofDist(pts[i][0].x, pts[i][0].y, pts[i][1].x, pts[i][1].y);
			float totalNumSegments = floor(fabs(totalDist)/desiredStrokeLen);
			float segmentsToDraw = floor(totalNumSegments * easedIncrement);
			float distX = pts[i][0].x - pts[i][1].x;
			float distY = pts[i][0].y - pts[i][1].y;
			int segmentLenX = (int)(distX/((float)totalNumSegments)/2.0);
			int segmentLenY = (int)(distY/((float)totalNumSegments)/2.0);
			for (int k = 0; k < segmentsToDraw; k++) {
				float posX = ofLerp(pts[i][0].x, pts[i][1].x, (k + 1)/totalNumSegments);
				float posY = ofLerp(pts[i][0].y, pts[i][1].y, (k + 1)/totalNumSegments);
				ofSetColor(color);
				ofFill();
				ofDrawCircle(posX, posY, circleRadius);
			}
		} else if (inc >= incMax) {
			float numLineSegments = floor(fabs(ofDist(pts[i][0].x, pts[i][0].y, pts[i][1].x, pts[i][1].y))/desiredStrokeLen);
			float segBreaker = (float)incMax/(float)numLineSegments;
			float distX = pts[i][0].x - pts[i][1].x;
			float distY = pts[i][0].y - pts[i][1].y;
			float segmentLenX = distX/((float)numLineSegments)/2.0;
			float segmentLenY = distY/((float)numLineSegments)/2.0;
			for (int k = 0; k < numLineSegments; k++) {
				float posX = ofLerp(pts[i][0].x, pts[i][1].x, (k + 1)/numLineSegments);
				float posY = ofLerp(pts[i][0].y, pts[i][1].y, (k + 1)/numLineSegments);
				ofSetColor(color);
				ofFill();
				ofDrawCircle(posX, posY, circleRadius);
			}
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
				if (circleRadius > 4) {
					ofDrawTriangle(glm::vec2(0 - 16, 0), glm::vec2(0, 16), glm::vec2(16, 0));
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
float DottedLine::quadEaseOut(float t) {
	return -1.0 *(t)*(t-2);
}

//--------------------------------------------------------------
float DottedLine::sineEaseOut(float t) {
	return sin(t * (PI/2));
}
