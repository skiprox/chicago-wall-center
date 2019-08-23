//
//  DashedLine.cpp
//  DashedLines
//
//
//

#include "DashedLine.hpp"

DashedLine::DashedLine()
{
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

DashedLine::DashedLine(vector<std::array<glm::vec2, 2>> _pts, float _strokeWidth, ofColor _color, int _incMax, bool _arrow) {
	pts = _pts;
	strokeWidth = _strokeWidth;
	color = _color;
	incMax = _incMax;
	arrow = _arrow;
    setup();
}

void DashedLine::setup() {
	inc = 0;
	numSegments = pts.size();
	path.setCurveResolution(100);
	ofSetCircleResolution(100);
	inc = 0;
}

void DashedLine::update(int _inc) {
	inc = _inc;
}

void DashedLine::draw() {
	ofPushStyle();
	path.clear();
	path.setStrokeWidth(strokeWidth);
	path.setStrokeColor(color);
	path.setFilled(false);
	for (int i = 0; i < numSegments; i++) {
		if (inc > (incMax/(numSegments) * i) && inc <= incMax/(numSegments) * (i + 1)) {
			// Draw the lines that have already been animated
			for (int j = 0; j < i; j++) {
				float numLineSegments = floor(fabs(ofDist(pts[j][0].x, pts[j][0].y, pts[j][1].x, pts[j][1].y))/desiredStrokeLen);
				float distX = pts[j][0].x - pts[j][1].x;
				float distY = pts[j][0].y - pts[j][1].y;
				float segmentLenX = distX/((float)numLineSegments)/2.0;
				float segmentLenY = distY/((float)numLineSegments)/2.0;
				for (int k = 0; k < numLineSegments; k++) {
					float posX = ofLerp(pts[j][0].x, pts[j][1].x, (k + 1)/numLineSegments);
					float posY = ofLerp(pts[j][0].y, pts[j][1].y, (k + 1)/numLineSegments);
					path.moveTo(posX, posY);
					path.lineTo(posX + segmentLenX, posY + segmentLenY);
				}
				path.close();
				path.draw();
			}
			// Animate the last part of the line
			path.moveTo(pts[i][0]);
			float upperEaseEq = (float)inc - ((float)incMax/((float)numSegments) * i);
			float lowerEaseEq = ((float)incMax/((float)numSegments) * ((float)i + 1.0)) - ((float)incMax/((float)numSegments) * i);
			float easedIncrement = sineEaseOut(upperEaseEq/lowerEaseEq);
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
				path.moveTo(posX, posY);
				path.lineTo(posX + segmentLenX, posY + segmentLenY);
			}
			path.close();
			path.draw();
		} else if (inc >= incMax) {
			float numLineSegments = floor(fabs(ofDist(pts[i][0].x, pts[i][0].y, pts[i][1].x, pts[i][1].y))/desiredStrokeLen);
			float distX = pts[i][0].x - pts[i][1].x;
			float distY = pts[i][0].y - pts[i][1].y;
			float segmentLenX = distX/((float)numLineSegments)/2.0;
			float segmentLenY = distY/((float)numLineSegments)/2.0;
			for (int k = 0; k < numLineSegments; k++) {
				float posX = ofLerp(pts[i][0].x, pts[i][1].x, (k + 1)/numLineSegments);
				float posY = ofLerp(pts[i][0].y, pts[i][1].y, (k + 1)/numLineSegments);
				path.moveTo(posX, posY);
				path.lineTo(posX + segmentLenX, posY + segmentLenY);
			}
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
float DashedLine::quadEaseOut(float t) {
	return -1.0 *(t)*(t-2);
}

//--------------------------------------------------------------
float DashedLine::sineEaseOut(float t) {
	return sin(t * (PI/2));
}
