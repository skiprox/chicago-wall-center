//
//  ImageMovement.hpp
//  Image Movement Class
//
//

#pragma once
#include "ofMain.h"

class ImageMovement
{
public:

	ImageMovement();
	ImageMovement(ofImage _img, glm::vec2 _size, glm::vec2 _startPos, glm::vec2 _endPos, int _incMax);

	void setup();
	void update(int _inc);
	void draw();
	float quadEaseOut(float t);

	ofImage img;

	glm::vec2 size, startPos, endPos;

	int inc, incMax;

};
