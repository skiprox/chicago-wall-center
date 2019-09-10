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

    	std::array<int, 10> touchThreshold = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    	std::array<bool, 10> shouldRunAnimation = {{false, false, false, false, false, false, false, false, false, false}};
    	std::array<int, 10> animationCounter = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    	int universalAnimationCounter = 0;
        int universalAnimationCounterMax = 5000;

    	// All the hand markers declared here
    	std::array<HandMarker, 10> handMarkers;

        // All images declared here
        ofImage background,
            companies,
            buildingBottom,
            buildingTop,
            aerial,
            people;

        // All Lines
        vector<DashedLine> buildingTopDashedLines;
        DashedLine aerialToBuildingLine;

        // All image animations
        ImageAnimation companiesAnimation, aerialAnimation, peopleAnimation;

        // All image movements
        ImageMovement buildingTopMovement;

        // All Text Typing animations
        TextTyping buildingBottomText,
            buildingTopText,
            companiesCenterText,
            companiesLeftText,
            peopleText,
            aerialLeftText,
            aerialCenterText,
            aerialRightText;
		
};
