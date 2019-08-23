//
//  Line.hpp
//  Line Class
//
//

#pragma once
#include "ofMain.h"

class Line
{
public:

	Line();
    Line(vector<std::array<glm::vec2, 2>> _pts, float _strokeWidth, ofColor _color, int _incMax, bool _arrow);

    void setup();
	void update(int _inc);
	void draw();
	float quadEaseOut(float t);

	ofPath path;

    vector<std::array<glm::vec2, 2>> pts;

    float strokeWidth;

    int inc, incMax, numSegments;

	ofColor color;

	bool arrow;
};
