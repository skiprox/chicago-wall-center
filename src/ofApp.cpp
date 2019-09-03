#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0);
	ofSetCircleResolution(100);
	width = ofGetWidth();
	height = ofGetHeight();
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	mySerial.setup(0, baud); //open the first device
	mySerial.startContinuousRead();
	ofAddListener(mySerial.NEW_MESSAGE,this,&ofApp::onNewMessage);
	message = "";
	setupImages();
	setupAnimations();
}

//--------------------------------------------------------------
void ofApp::setupImages(){
	background.load("images/background.png");
	companies.load("images/companies.png");
	buildingBottom.load("images/building-bottom.png");
	buildingTop.load("images/building-top.png");
}

//--------------------------------------------------------------
void ofApp::setupAnimations(){
	/**
	 * ALL THE LINE ANIMATIONS GO HERE
	 */
	vector<std::array<glm::vec2, 2>> pts;
	// Upper Building Dashed Lines
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1615, 1026), glm::vec2(1615, 346)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1670, 1072), glm::vec2(1670, 392)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1725, 1092), glm::vec2(1725, 412)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1780, 1000), glm::vec2(1780, 320)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1835, 967), glm::vec2(1835, 287)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1890, 902), glm::vec2(1890, 230)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	/**
	 * ALL THE IMAGE ANIMATIONS GO HERE
	 */
	companiesAnimation = ImageAnimation(
		companies,
		glm::vec2(fixedWidth/2.0, fixedHeight - 120),
		glm::vec2(800, 200),
		250
	);
	/**
	 * ALL THE IMAGE MOVEMENTS GO HERE
	 */
	buildingTopMovement = ImageMovement(
		buildingTop,
		glm::vec2(338, 268),
		glm::vec2(1751, 1058),
		glm::vec2(1751, 274),
		250
	);
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// MILLENIUM FALCON RIGHT (1)
	handMarkers[0] = HandMarker(glm::vec2(fixedWidth - 100, fixedHeight - 100), red, true);
	// MILLENIUM FALCON LEFT (2)
	handMarkers[1] = HandMarker(glm::vec2(fixedWidth - 200, fixedHeight - 40), red, true);
	// CENTER OF THE COMPANIES
	handMarkers[2] = HandMarker(glm::vec2(fixedWidth/2.0 - 50, fixedHeight - 120), red, false);
	// LEFT OF THE COMPANIES
	handMarkers[3] = HandMarker(glm::vec2(fixedWidth/3.0 - 150, fixedHeight - 160), red, false);
	// BOTTOM BUILDING
	handMarkers[4] = HandMarker(glm::vec2(1752, 652), red, true);
	// TOP BUILDING
	handMarkers[5] = HandMarker(glm::vec2(1752, 230), red, true);
}

//--------------------------------------------------------------
void ofApp::update(){
	updateSerials();
	if (fabs(universalCounter - TWO_PI) < 0.0001) {
		universalCounter = 0.0f;
	} else {
		universalCounter = (universalCounter + PI/100.0);
	}
	for (int i = 0; i < handMarkers.size(); i++) {
		handMarkers[i].update(universalCounter);
	}
}

//--------------------------------------------------------------
void ofApp::updateSerials(){
	// Loop through the serial inputs
	for (int i = 0; i < serialInput.size(); i++) {
		// If we have something going on with the serial input...
		if (serialInput[i] == 1) {
			// Increase the threshold number,
			touchThreshold[i]++;
			// and once that reaches 10 we can run the animation
			if (touchThreshold[i] >= 50) {
				// Reset the threshold
				touchThreshold[i] = 0;
				checkShouldRunAnimations(i);
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::onNewMessage(string & message)
{
	vector<string> input = ofSplitString(message, ",");
	serialInput.clear();
	for (int i = 0; i < input.size() - 1; i++) {
		serialInput.push_back(ofToInt(input[i]));
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	drawBackground();
	drawAnimations();
	drawHandMarkers();
}

//--------------------------------------------------------------
void ofApp::drawBackground(){
	ofBackground(255);
	ofSetColor(255);
	// Draw up and down lines
	for (int i = 0; i < 2; i++) {
		ofDrawRectangle(width/3.0 * (i + 1) - 1, 0, 2, fixedHeight);
		float divConst = 5.0;
		if (i == 0) {
			ofDrawRectangle(0, fixedHeight/divConst, fixedWidth, 2);
		} else {
			ofDrawRectangle(0, fixedHeight - fixedHeight/divConst, fixedWidth, 2);
		}
	}
	background.draw(0, 0, fixedWidth, fixedHeight);
	buildingBottom.draw(1752 - 161, 652 - 130, 333, 260);
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	handMarkers[0].draw();
	handMarkers[1].draw();
	// for (int i = 0; i < handMarkers.size(); i++) {
	// 	handMarkers[i].draw();
	// }
}

//--------------------------------------------------------------
void ofApp::drawAnimations(){
	// Run through the `shouldRunAnimation` array, and run any of the animations that we should
	for (int i = 0; i < shouldRunAnimation.size(); i++) {
		if (shouldRunAnimation[i]) {
			// Increase the animation counter,
			// which is what we're using to run the animation for
			// a set amount of time
			animationCounter[i]++;
			// If we haven't hit the threshold for how long to
			// run the animation, fucking run it
			if (animationCounter[i] <= animationCounterMax[i]) {
				runAnimation(i + 1);
			} else { // Otherwise stop running the animation
				shouldRunAnimation[i] = false;
				animationCounter[i] = 0;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// Keys 1 through 9 are 49 through 57
	// so use this to determine the index of the animation we should run
	if (key >= 49 && key <= 57) {
		int keyIndex = key - 49;
		// Simulate increasing the threshold number,
		touchThreshold[keyIndex] = 50;
		// and once that reaches 50 we can run the animation
		if (touchThreshold[keyIndex] >= 50) {
			// Reset the threshold
			touchThreshold[keyIndex] = 0;
			checkShouldRunAnimations(keyIndex);
		}
	} else if (key == 32) {
		for (int i = 0; i < shouldRunAnimation.size(); i++) {
			shouldRunAnimation[i] = false;
			animationCounter[i] = 0;
		}
	}
}

//--------------------------------------------------------------
void ofApp::checkShouldRunAnimations(int index){
	// If it's the first button, we should run the animation
	if (index == 0) { // Right Millenium Falcon, building animations
		shouldRunAnimation[index] = true;
	} else if (index == 1) { // Left Millenium Falcon, companies animations
		shouldRunAnimation[index] = true;
	} else if (index == 2) { // Center of the companies, crowd of people animation
		if (shouldRunAnimation[1]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 3) { // Left of the companies, three buttons on colored diagram animation
		if (shouldRunAnimation[1]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 4) { // Bottom building, text animation
		if (shouldRunAnimation[0]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 5) { // Top building, text animation
		if (shouldRunAnimation[0]) {
			shouldRunAnimation[index] = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::runAnimation(int animationNum){
	switch(animationNum) {
		case 1:
			ofPushStyle();
			buildingTopMovement.update(animationCounter[0]);
			buildingTopMovement.draw();
			for (int i = 0; i < buildingTopDashedLines.size(); i++) {
				buildingTopDashedLines[i].update(animationCounter[0]);
				buildingTopDashedLines[i].draw();
			}
			handMarkers[4].draw();
			handMarkers[5].draw();
			ofPopStyle();
			break;
		case 2:
			ofPushStyle();
			companiesAnimation.update(animationCounter[1]);
			companiesAnimation.draw();
			handMarkers[2].draw();
			handMarkers[3].draw();
			ofPopStyle();
			break;
		case 3:
			ofPushStyle();
			ofPopStyle();
			break;
		case 4:
			break;
		case 5:
			break;
		case 6:
			break;
		case 7:
			break;
		case 8:
			break;
		case 9:
			break;
		default:
			cout << "WHAT FUCKING KEY IS THIS?? " << animationNum << endl;
			break;
	}
}
