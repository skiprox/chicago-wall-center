#pragma once

#include "ofMain.h"
#include "Constants.hpp"
#include "ofxSimpleSerial.h"
#include "Line.hpp"
#include "DashedLine.hpp"
#include "DottedLine.hpp"
#include "HandMarker.hpp"
#include "ImageAnimation.hpp"
#include "ImageMovement.hpp"
#include "TextTyping.hpp"

class ofApp : public ofBaseApp{

	public:
		void setup();
        void setupImages();
        void setupAnimations();
		void update();
		void updateSerials();
		void draw();
		void drawBackground();
		void drawHandMarkers();
        void checkShouldRunAnimations(int index);
		void drawAnimations();
		void onNewMessage(string & message);
		void keyPressed(int key);
		void runAnimation(int animationNum);

		ofxSimpleSerial	mySerial;
		int baud = 57600;
        ofColor red = ofColor(220, 20, 30);
    	int serial1 = 0;
    	int serial2 = 0;

    	float width;
    	float height;

        float fixedWidth = 1920.0;
        float fixedHeight = 1080.0;

    	float universalCounter = 0.0;

    	string message;

    	vector<int> serialInput;

    	std::array<int, 9> touchThreshold = {{0, 0, 0, 0, 0, 0, 0, 0, 0}};

    	std::array<bool, 9> shouldRunAnimation = {{false, false, false, false, false, false, false, false, false}};
    	std::array<int, 9> animationCounter = {{0, 0, 0, 0, 0, 0, 0, 0, 0}};
    	std::array<int, 9> animationCounterMax = {{5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000, 5000}};

    	// All animations declared here

        // All arbitrary animation variables which we'll need to create animations
        vector<std::array<glm::vec2, 4>> _pts;

    	// All the hand markers declared here
    	std::array<HandMarker, 9> handMarkers;

        // All images declared here
        ofImage background;
        ofImage companies;
        ofImage building;

        // All Lines
        vector<DashedLine> buildingDashedLines;

        // All image animations
        ImageAnimation companiesAnimation;

        // All image movements
        ImageMovement buildingMovement;

        // All Text Typing animations
        // TextTyping buildingLeftText;
		
};
