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
	background.load("images/background.jpg");
	companies.load("images/companies.png");
	buildingBottom.load("images/building-bottom.png");
	buildingTop.load("images/building-top.png");
	aerial.load("images/aerial.jpg");
	people.load("images/people.png");
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
		glm::vec2(800, 212),
		250
	);
	aerialAnimation = ImageAnimation(
		aerial,
		glm::vec2(fixedWidth/3.0, 200),
		glm::vec2(400, 239),
		250
	);
	peopleAnimation = ImageAnimation(
		people,
		glm::vec2(fixedWidth/2.0 + 198, fixedHeight/2.0 + 15),
		glm::vec2(430, 223),
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
	 * ALL THE TEXT TYPING GO HERE
	 */
	buildingBottomText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(1500, 800),
		ofColor(255),
		7,
		150
	);
	buildingTopText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(1460, 260),
		ofColor(255),
		7,
		150
	);
	companiesCenterText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(900, 790),
		ofColor(255),
		7,
		150
	);
	companiesLeftText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(330, 920),
		ofColor(255),
		7,
		150
	);
	peopleText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(790, 550),
		ofColor(255),
		7,
		150
	);
	aerialLeftText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(275, 200),
		ofColor(255),
		7,
		150
	);
	aerialCenterText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(450, 435),
		ofColor(255),
		7,
		150
	);
	aerialRightText = TextTyping(
		"Lorem ipsum dolor sit amet, consectetur adipisicing\nelit, sed do eiusmod tempor incididunt ut labore et\ndolore magna aliqua. Ut enim ad minim veniam, quis\nnostrud exercitation ullamco laboris nisi ut aliquip\nex ea commodo consequat. Duis aute irure dolor in\nreprehenderit in voluptate velit esse cillum dolore\neu fugiat nulla pariatur. Excepteur sint occaecat\ncupidatat non proident, sunt in culpa qui officia\ndeserunt mollit anim id est laborum. Lorem ipsum dolor\nsit amet, consectetur adipisicing elit, sed do eiusmod\ntempor incididunt ut labore et dolore magna aliqua.\nUt enim ad minim veniam, quis nostrud exercitation\nullamco laboris nisi ut aliquip ex ea commodo consequat.\nDuis aute irure dolor in reprehenderit in voluptate\nvelit esse cillum dolore eu fugiat nulla pariatur.\nExcepteur sint occaecat cupidatat non proident, sunt in\nculpa qui officia deserunt mollit anim id est laborum.",
		glm::vec2(980, 200),
		ofColor(255),
		7,
		150
	);
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// MILLENIUM FALCON RIGHT
	handMarkers[0] = HandMarker(glm::vec2(fixedWidth - 100, fixedHeight - 100), red, true);
	// MILLENIUM FALCON LEFT
	handMarkers[1] = HandMarker(glm::vec2(fixedWidth - 200, fixedHeight - 40), red, true);
	// BOTTOM BUILDING
	handMarkers[2] = HandMarker(glm::vec2(1752, 652), red, true);
	// TOP BUILDING
	handMarkers[3] = HandMarker(glm::vec2(1752, 230), red, true);
	// CENTER OF THE COMPANIES
	handMarkers[4] = HandMarker(glm::vec2(fixedWidth/2.0 - 50, fixedHeight - 120), red, false);
	// LEFT OF THE COMPANIES
	handMarkers[5] = HandMarker(glm::vec2(fixedWidth/3.0 - 150, fixedHeight - 160), red, false);
	// AERIAL LEFT
	handMarkers[6] = HandMarker(glm::vec2(fixedWidth/3.0 - 150, 200), red, true);
	// AERIAL CENTER
	handMarkers[7] = HandMarker(glm::vec2(fixedWidth/3.0, 200), red, true);
	// AERIAL RIGHT
	handMarkers[8] = HandMarker(glm::vec2(fixedWidth/3.0 + 150, 200), red, true);
	// PEOPLE
	handMarkers[9] = HandMarker(glm::vec2(fixedWidth/2.0 + 160, fixedHeight/2.0 + 50), red, true);
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
				runAnimation(i);
			} else { // Otherwise stop running the animation
				shouldRunAnimation[i] = false;
				animationCounter[i] = 0;
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// Keys 0 through 9 are 48 through 57
	// so use this to determine the index of the animation we should run
	if (key >= 48 && key <= 57) {
		int keyIndex = key - 48;
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
	if (index == 0) { // Right Millenium Falcon
		// building animations
		shouldRunAnimation[0] = true;
	} else if (index == 1) { // Left Millenium Falcon
		// companies animations
		shouldRunAnimation[1] = true;
	} else if (index == 2) { // Bottom building
		if (shouldRunAnimation[0]) {
			// Text animation
			shouldRunAnimation[2] = true;
		}
	} else if (index == 3) { // Top building
		if (shouldRunAnimation[0]) {
			// Text animation
			shouldRunAnimation[3] = true;
		}
	} else if (index == 4) { // Companies center
		if (shouldRunAnimation[1]) {
			// People animation
			shouldRunAnimation[4] = true;
		}
	} else if (index == 5) { // Companies left
		if (shouldRunAnimation[1]) {
			// Aerial animation
			shouldRunAnimation[5] = true;
		}
	} else if (index == 6) { // Aerial left
		if (shouldRunAnimation[5]) {
			// Aerial text left
			shouldRunAnimation[6] = true;
		}
	} else if (index == 7) { // Aerial center
		if (shouldRunAnimation[5]) {
			// Aerial text center
			shouldRunAnimation[7] = true;
		}
	} else if (index == 8) { // Aerial right
		if (shouldRunAnimation[5]) {
			// Aerial text right
			shouldRunAnimation[8] = true;
		}
	} else if (index == 9) { // People
		if (shouldRunAnimation[4]) {
			// People text
			shouldRunAnimation[9] = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::runAnimation(int animationNum){
	switch(animationNum) {
		// PRESS RIGHT MILLENIUM FALCON
		// ANIMATE BUILDINGS
		case 0:
			ofPushStyle();
			buildingTopMovement.update(animationCounter[0]);
			buildingTopMovement.draw();
			for (int i = 0; i < buildingTopDashedLines.size(); i++) {
				buildingTopDashedLines[i].update(animationCounter[0]);
				buildingTopDashedLines[i].draw();
			}
			handMarkers[2].draw();
			handMarkers[3].draw();
			ofPopStyle();
			break;
		// PRESS LEFT MILLENIUM FALCON
		// ANIMATE COMPANIES
		case 1:
			ofPushStyle();
			companiesAnimation.update(animationCounter[1]);
			companiesAnimation.draw();
			handMarkers[4].draw();
			handMarkers[5].draw();
			ofPopStyle();
			break;
		// PRESS BOTTOM BUILDING
		// ANIMATE TEXT
		case 2:
			ofPushStyle();
			buildingBottomText.update(animationCounter[2]);
			buildingBottomText.draw();
			ofPopStyle();
			break;
		// PRESS TOP BUILDING
		// ANIMATE TEXT
		case 3:
			ofPushStyle();
			buildingTopText.update(animationCounter[3]);
			buildingTopText.draw();
			ofPopStyle();
			break;
		// PRESS COMPANIES CENTER
		// ANIMATE PEOPLE
		case 4:
			ofPushStyle();
			peopleAnimation.update(animationCounter[4]);
			peopleAnimation.draw();
			companiesCenterText.update(animationCounter[4]);
			companiesCenterText.draw();
			handMarkers[9].draw();
			ofPopStyle();
			break;
		// PRESS COMPANIES LEFT
		// ANIMATE AERIAL
		case 5:
			ofPushStyle();
			aerialAnimation.update(animationCounter[5]);
			aerialAnimation.draw();
			companiesLeftText.update(animationCounter[5]);
			companiesLeftText.draw();
			handMarkers[6].draw();
			handMarkers[7].draw();
			handMarkers[8].draw();
			ofPopStyle();
			break;
		// PRESS AERIAL LEFT
		// ANIMATE TEXT
		case 6:
			ofPushStyle();
			aerialLeftText.update(animationCounter[6]);
			aerialLeftText.draw();
			ofPopStyle();
			break;
		// PRESS AERIAL CENTER
		// ANIMATE TEXT
		case 7:
			ofPushStyle();
			aerialCenterText.update(animationCounter[7]);
			aerialCenterText.draw();
			ofPopStyle();
			break;
		// PRESS AERIAL RIGHT
		// ANIMATE TEXT
		case 8:
			ofPushStyle();
			aerialRightText.update(animationCounter[8]);
			aerialRightText.draw();
			ofPopStyle();
			break;
		// PRESS PEOPLE
		// ANIMATE TEXT
		case 9:
			ofPushStyle();
			peopleText.update(animationCounter[9]);
			peopleText.draw();
			ofPopStyle();
			break;
		default:
			cout << "WHAT FUCKING KEY IS THIS?? " << animationNum << endl;
			break;
	}
}
