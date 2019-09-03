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
	building.load("images/building.png");
}

//--------------------------------------------------------------
void ofApp::setupAnimations(){
	/**
	 * ALL THE LINE ANIMATIONS GO HERE
	 */
	vector<std::array<glm::vec2, 2>> pts;
	// Lower Building Dashed Line
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1615, 1086), glm::vec2(1615, 706)}});
	buildingDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1670, 1152), glm::vec2(1670, 772)}});
	buildingDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1725, 1092), glm::vec2(1725, 712)}});
	buildingDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1780, 1062), glm::vec2(1780, 682)}});
	buildingDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1835, 1017), glm::vec2(1835, 637)}});
	buildingDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1890, 982), glm::vec2(1890, 610)}});
	buildingDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
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
	buildingMovement = ImageMovement(
		building,
		glm::vec2(333, 260),
		glm::vec2(1752, 982),
		glm::vec2(1752, 652),
		250
	);
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// MILLENIUM FALCON
	handMarkers[0] = HandMarker(glm::vec2(fixedWidth - 100, fixedHeight - 100), red, true);
	// CENTER OF THE COMPANIES
	handMarkers[1] = HandMarker(glm::vec2(fixedWidth/2.0 - 50, fixedHeight - 120), red, false);
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
		ofDrawRectangle(width/3.0 * (i + 1) - 1, 0, 2, height);
		float divConst = 5.0;
		if (i == 0) {
			ofDrawRectangle(0, height/divConst, width, 2);
		} else {
			ofDrawRectangle(0, height - height/divConst, width, 2);
		}
	}
	background.draw(0, 0, width, height);
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	handMarkers[0].draw();
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
	if (index == 0) {
		shouldRunAnimation[index] = true;
	} else if (index == 1) { // The bad companies sensor
		if (shouldRunAnimation[0]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 2) { // The lower building
		if (shouldRunAnimation[1]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 3) { // The right building text
		if (shouldRunAnimation[1]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 4) { // The plane text
		if (shouldRunAnimation[1]) {
			shouldRunAnimation[index] = true;
		}
	} else if (index == 5) { // The ship text
		if (shouldRunAnimation[1]) {
			shouldRunAnimation[index] = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::runAnimation(int animationNum){
	switch(animationNum) {
		case 1:
			ofPushStyle();
			handMarkers[1].draw();
			companiesAnimation.update(animationCounter[0]);
			companiesAnimation.draw();
			ofPopStyle();
			break;
		case 2:
			ofPushStyle();
			buildingMovement.update(animationCounter[1]);
			buildingMovement.draw();
			for (int i = 0; i < buildingDashedLines.size(); i++) {
				buildingDashedLines[i].update(animationCounter[1]);
				buildingDashedLines[i].draw();
			}
			ofPopStyle();
			break;
		case 3:
			ofDrawRectangle(width - width/3.0, 0, width/3.0, height/5.0);
			break;
		case 4:
			ofDrawRectangle(0, height/5.0, width/3.0, height - 2.0 * (height/5.0));
			break;
		case 5:
			ofDrawRectangle(width/3.0, height/5.0, width/3.0, height - 2.0 * (height/5.0));
			break;
		case 6:
			ofDrawRectangle(width - width/3.0, height/5.0, width/3.0, height - 2.0 * (height/5.0));
			break;
		case 7:
			ofDrawRectangle(0, height - height/5.0, width/3.0, height/3.0);
			break;
		case 8:
			ofDrawRectangle(width/3.0, height - height/5.0, width/3.0, height/3.0);
			break;
		case 9:
			ofDrawRectangle(width/3.0 * 2, height - height/5.0, width/3.0, height/3.0);
			break;
		default:
			cout << "WHAT FUCKING KEY IS THIS?? " << animationNum << endl;
			break;
	}
}
