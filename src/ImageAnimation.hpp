//
//  ImageAnimation.hpp
//  Image Animation Class
//
//

#pragma once
#include "ofMain.h"

class ImageAnimation
{
public:

	ImageAnimation();
    ImageAnimation(ofImage _img, glm::vec2 _pos, glm::vec2 _size, int _incMax);

    void setup();
	void update(int _inc);
	void draw();
	float bounceEaseOut(float t);
	float quadEaseOut(float t);

	ofImage img;
	glm::vec2 pos, size;
	int inc, incMax;
};
