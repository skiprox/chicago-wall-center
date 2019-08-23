//
//  TextTyping.hpp
//  Dashed Line Class
//
//

#pragma once
#include "ofMain.h"

class TextTyping
{
public:

	TextTyping();
	TextTyping(string _text, glm::vec2 _pos, ofColor _color, int _fontSize, int _incMax);

	void setup();
	void update(int _inc);
	void draw();

	ofTrueTypeFont myfont;

	string text;

	glm::vec2 pos;

	int inc, incMax, fontSize;

	ofColor color;
};
