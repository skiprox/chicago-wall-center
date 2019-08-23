//
//  HandMarker.hpp
//  Hand Marker Class
//
//

#pragma once
#include "ofMain.h"

class HandMarker
{
public:

	HandMarker();
    HandMarker(glm::vec2 center, ofColor _color);

    void setup();
	void update(float _inc);
	void draw();

    glm::vec2 center;

    float inc;

	ofColor color;
};
