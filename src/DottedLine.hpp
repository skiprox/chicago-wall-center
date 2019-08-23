//
//  DottedLine.hpp
//  Dashed Line Class
//
//

#pragma once
#include "ofMain.h"

class DottedLine
{
public:

	DottedLine();
	DottedLine(vector<std::array<glm::vec2, 2>> _pts, float _circleRadius, ofColor _color, int _incMax, bool _arrow);

	void setup();
	void update(int _inc);
	void draw();
	float quadEaseOut(float t);
	float sineEaseOut(float t);

	vector<std::array<glm::vec2, 2>> pts;

	float circleRadius;

	int inc, incMax;

	bool arrow;

	float numSegments;

	int desiredStrokeLen = 20;

	ofColor color;
};
