#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofHideCursor();
	ofBackground(0);
	ofSetCircleResolution(100);
	width = ofGetWidth();
	height = ofGetHeight();
	mySerial.listDevices();
	vector <ofSerialDeviceInfo> deviceList = mySerial.getDeviceList();
	mySerial.setup(0, baud); //open the first device
	mySerial.startContinuousRead(false);
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
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1615, 1025), glm::vec2(1615, 400)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1670, 1069), glm::vec2(1670, 444)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1725, 1071), glm::vec2(1725, 446)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1780, 1000), glm::vec2(1780, 375)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1835, 945), glm::vec2(1835, 320)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1890, 881), glm::vec2(1890, 256)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	// The dashed line around the companies
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(fixedWidth/2.0, 840), glm::vec2(505, 840)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(505, 840), glm::vec2(505, 1060)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(505, 1060), glm::vec2(1370, 1060)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1370, 1060), glm::vec2(1370, 840)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1370, 840), glm::vec2(fixedWidth/2.0, 840)}});
	companiesLine = DashedLine(pts, 5.0, red, 250, false);
	pts.clear();
	// Press points for the people
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(800, 700), glm::vec2(970, 635)}});
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// Aerial to upper building dashed lines
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(780, 375), glm::vec2(1350, 375)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1350, 375), glm::vec2(1350, 275)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1350, 275), glm::vec2(1555, 275)}});
	aerialToBuildingLine = DashedLine(pts, 2.0, red, 250, true);
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
		glm::vec2(fixedWidth/3.0 - 60, 450),
		glm::vec2(600, 359),
		250
	);
	peopleAnimation = ImageAnimation(
		people,
		glm::vec2(fixedWidth/2.0 + 254, fixedHeight/2.0 + 55),
		glm::vec2(430, 223),
		250
	);
	/**
	 * ALL THE IMAGE MOVEMENTS GO HERE
	 */
	buildingTopMovement = ImageMovement(
		buildingTop,
		glm::vec2(338, 268),
		glm::vec2(1737, 945),
		glm::vec2(1737, 320),
		250
	);
	/**
	 * ALL THE TEXT TYPING GO HERE
	 */
	milleniumRightText = TextTyping(
		"Generalitat de Catalunya as well as representatives of\nBarcelona's Chamber of Commerce and the public-private\nentity Fira de Barcelona announced a closed-door deal.\nThey revealed ”Fira Univers,” an institutional project to\nsupport the investment of 380 million euros for the renovation\nof the Montjuïc fairgrounds and the expansion of the\nl’Hospitalet fairgrounds to be inaugurated in 2029 for the\n100th anniversary of the 1929 International Exhibition. Even\nthough the fairgrounds stand in municipal land, the details\nof the agreement were not made public and until today are\nunknown. There was no public debate or consultation before\nthe signing of the deal. These actions have recently put to\nquestion the new priorities of the government of the City of\nBarcelona; a self-professed progressive government that\nwon elections in 2015 with the public promise to “kick-out\nmafias and lobbies.” Without any public debate, the Montjuïc\nfairgrounds and its management company Fira de Barcelona\ncontinues to be a highly profitable, opaque, and carefully\nguarded public-private organization dedicated to promote\nmega-events for the global capitalist elites.",
		glm::vec2(1500, 900),
		ofColor(255),
		8,
		250
	);
	milleniumLeftText = TextTyping(
		"The Montjuïc fairground occupies 27 hectares of\nmunicipal land, in a very central location of Barcelona.\nIts size is equivalent to 21 blocks of the famous Cerdá\nEixample or the full surface of La Barceloneta neigh-\nborhood where more than 16,000 people live. The for-\nprofit public-private Fira de Barcelona company controls\nthe site since 1932. The current concession is due to\nexpire on December 2025. Under this concession, Fira\nde Barcelona pays 0,25 euros per year for each 140,000\nsquare meter building in the site. This is the equivalent\nof paying less than 2 euros a month for a home of 80m²\nright in the heart of the city. The concession also allows\nFira de Barcelona the for-profit use of all the public\nspaces inside the fairgrounds. They are even allowed to\nclose these public spaces for the benefit of the mega-\nevents they host. These concessions have demarcated\nthe urban fragmentation of its adjacent neighborhoods\nof Poble-sec and Font de la Guatlla. Most of the buildings\nin the fairground have no historical value and are poorly\nmaintained.",
		glm::vec2(1500, 900),
		ofColor(255),
		8,
		250
	);
	buildingBottomText = TextTyping(
		"Every year, Fira de Barcelona hosts approximately 150 events, and it is\nvisited by over 2 million people from across the globe. Rather than\npromoting activities that benefit local initiatives and development, the vast\nmajority of these events encourage the business of large multinational\ncorporations, such as mobile phone companies, automobile industries,\npredatory real-estate exhibitions, agro-businesses, and mass tourism\nenterprises. Amongst its most recognized events is the International Real\nEstate Fair, where notorious vulture fund companies like Blackstone, TPG\nCapital, Lone Star, Capital Management as well as the speculative banking\nsectors of Credit Suisse, Morgan Stanley, JP Morgan and Crédit Agricole\nfill the halls of the Municipally owned buildings of its two fairgrounds, which\nhave a legal mandate to serve its citizens and promote the public good.",
		glm::vec2(1450, 670),
		ofColor(255),
		8,
		250
	);
	buildingTopText = TextTyping(
		"The grassroots coalition #LaFiraOLaVida has been envisioning a new type\nof urban development for the future of the Montjuïc fairgrounds. Against the\ngated nature of the current fairground, they imagine an open landscape,\nweaved by continuous pedestrian streets that connect the adjacent\nneighborhoods of Poble-sec and Font de la Guatlla. They envision the\nsite's main avenue as a linear public park, full of endogenous vegetation\nand spaces for public gather that flow upwards into the hill of Montjuïc and\ndownwards into Plaça d'Espanya. They envision a compact neighborhood,\ndesigned to house a density of thousands of livelihoods and mixed uses.\n#LaFiraOLaVida projects an entirely walkable community built on a surplus\nof affordable living and working spaces that will be permanently welcoming\nfor all of Barcelona.",
		glm::vec2(1350, 350),
		ofColor(255),
		8,
		250
	);
	companiesCenterText = TextTyping(
		"Fira de Barcelona is a public-private consortium formed in 1932 by the City of Barcelona, the Generalitat de Catalunya and the Chamber of Commerce.\nThis last one is an independent public corporation that promotes and offers services to business entities. Fira de Barcelona occupies two separate\nfairgrounds on municipal land. The oldest of the two offers 140,000m² of expo surface in the center of Barcelona, right next to the hill of Montjuïc. The\nnewest was built in 2007 in the adjacent municipality of l'Hospitalet but was financed by the City of Barcelona. It offers 240,000m² of expo surface. In\n2017, the City of Barcelona gave to Fira de Barcelona in limited concession the International Convention Center of Barcelona, which provides another\n75,000m² of surface located in the waterfront. Fira de Barcelona has an annual revenue of 210 million euros, while the complete housing yearly budget of\nthe City of Barcelona is 165 million euros.",
		glm::vec2(900, 790),
		ofColor(255),
		8,
		250
	);
	companiesLeftText = TextTyping(
		"On May 30, 2019, a grassroots coalition of over 60 social movements publicly announced\nits opposition to the City of Barcelona's intention to extend the concession of the\nMontjuïc fairground to Fira de Barcelona. The coalition took the name #LaFiraOLaVida (the\nfair or life). Its main objectives are: first, to denounce the lack of transparency and anti-\ndemocratic character of the closed-door deal; and second, to propose alternatives for\nconverting the fairground into an urban common. #LaFiraOLaVida presented a public\nmanifesto demanding the halt of all plans to extend the concession and the renovation\nof the fairground and that the city opens a public consultation instead. #LaFiraOLaVida\nimagines the future of the fairground as an inclusive, open, and diverse neighborhood full\nof affordable housing, retail shops, proximity facilities, walkable streets, and green public\nspaces of human scale.",
		glm::vec2(260, 940),
		ofColor(255),
		8,
		250
	);
	peopleText = TextTyping(
		"Since its inception, the grassroots coalition #LaFiraOLaVida keeps growing and it already includes over 60 social movements,\nneighborhood organizations and civic platforms. From a national and city-scale it includes big entities like Sindicat de Llogaters,\nPAH Barcelona or Associació 500x20 (housing), ATTAC Catalunya, Assemblea de Barris per un Turisme Sostenible or Marea\nPensionista (economy), Ecologistes en Acció, Plataforma per la Qualitat de l'Aire or Associació Salut i Agroecologia (ecology)\nand Procés Constituent (democracy). It also includes multiple neighborhood organizations like Sindicat de Barri del Poble-sec,\nFem Sant Antoni, Assemblea del Barri de Sants, Espai Comunitari Autogestionat de Can Batlló, Espai Feminista de Sants,\nAssociació Cultural El Lokal del Raval, Plataforma en Defensa de la Barceloneta, Oficina d'Habitatge Popular de Gràica,\nAssociació de Veïns i Veïnes de la Sagrada Família.",
		glm::vec2(1160, 770),
		ofColor(255),
		8,
		250
	);
	aerialLeftText = TextTyping(
		"Despite its greenwashing propaganda, the megaevents that occur in the\nMontjuïc fairground promote the consumer fantasy that unlimited growth is\npossible in a finite and fragile planet. The Fira de Barcelona dedicates\nmany of its itinerant congresses to the automobile and motorcycle\nindustries, mass tourism, predatory real-estate development, mass\nconsumption, and corporate agro-business, all of those responsible for\nemitting high levels of pollution and the heavy dependence on destructive\nextractivism. The negative environmental footprint of the 2 million congress\ntourists that arrive each year is substantial. The business model of La Fira\nde Barcelona heavily depends on the use of airplanes, cruise ships, taxis,\nhotels, luxury properties, as well as intense energy and water consumption.\n\nIn contrast, the proposed new neighborhood would replace the fairground\nwith a socially sustainable and environmentally resilient urban model. It\nprioritizes the design and implementation of environmental justice\nprinciples, which are at the core of the Urban Front ethos. The design\nprerogative for density and compactness provides the spatial layout for a\nneighborhood built on pedestrian, bicycle, and public infrastructure. A\ndiverse local workforce composed of construction cooperatives would erect\nnew buildings that utilize renewable resources and new technologies in\nthermal efficiency and recycling. The creation of collectivized facilities,\ncommunity-controlled utilities, and spaces to pursue food sovereignty\nwould be elemental.",
		glm::vec2(200, 415),
		ofColor(255),
		8,
		250
	);
	aerialCenterText = TextTyping(
		"The mega-events of Fira de Barcelona amplify the voice of\nmultinational corporations and their capitalist desires to\nderegulate, extract public wealth, financialize, exploit, pollute\nand dislocate local processes for their benefit. The companies\nthat Fira de Barcelona promotes have a substantial impact on\nthe concentration of wealth and economic opportunities in the\ncity. They are involved in the promotion of mass tourism and its\ndire socio-environmental ills; in the precarization of workers; and\nin the speculative real-estate industries that have made\nBarcelona an unaffordable city.\n\nAn essential part of # LaFiraOLaVida's proposal for the\nfairground is the inclusion of local social initiatives and solidarity\nnetworks. The project puts forward a neighborhood that is co-\nproduced by autonomous collective worker groups, small\nenterprises, non-profit cooperatives, cultural initiatives, as well\nas socially productive sites that pursue fair trade, environmental\njustice, and the common good.",
		glm::vec2(590, 590),
		ofColor(255),
		8,
		250
	);
	aerialRightText = TextTyping(
		"The proposed new neighborhood would replace the derelict buildings in the\nMontjuïc fairground for thousands of new affordable housing units —between\n5,000 and 10,000, depending on the density— that would help to improve\nBarcelona's housing crisis significantly and to contra-rest the intensifying\ngentrification of adjacent neighborhoods as Poble-sec and Sant Antoni. Far\nfrom being a monofunctional neighborhood or a social ghetto-like so many\nperipheral dormitory cities, it would be a community-controlled central\nneighborhood that would embrace a mixture of functions and populations in a\ndense fabric built by local construction cooperatives and organizations.",
		glm::vec2(970, 300),
		ofColor(255),
		8,
		250
	);
	/**
	 * ALL THE HAND MARKERS GO HERE
	 */
	// MILLENIUM FALCON RIGHT
	handMarkers[0] = HandMarker(glm::vec2(fixedWidth - 200, fixedHeight - 115), red, true);
	// MILLENIUM FALCON LEFT
	handMarkers[1] = HandMarker(glm::vec2(fixedWidth - 380, fixedHeight - 100), red, true);
	// BOTTOM BUILDING
	handMarkers[2] = HandMarker(glm::vec2(fixedWidth - 180, fixedHeight/2.0 + 20), red, true);
	// TOP BUILDING
	handMarkers[3] = HandMarker(glm::vec2(1702, 420), white, true);
	// CENTER OF THE COMPANIES
	handMarkers[4] = HandMarker(glm::vec2(fixedWidth/2.0 - 60, fixedHeight - 110), red, false);
	// LEFT OF THE COMPANIES
	handMarkers[5] = HandMarker(glm::vec2(fixedWidth/3.0 - 100, fixedHeight - 200), red, false);
	// AERIAL LEFT
	handMarkers[6] = HandMarker(glm::vec2(fixedWidth/3.0 - 300, 470), red, true);
	// AERIAL CENTER
	handMarkers[7] = HandMarker(glm::vec2(fixedWidth/3.0 - 60, 470), red, true);
	// AERIAL RIGHT
	handMarkers[8] = HandMarker(glm::vec2(fixedWidth/3.0 + 190, 470), red, true);
	// PEOPLE
	handMarkers[9] = HandMarker(glm::vec2(fixedWidth/2.0 + 216, fixedHeight/2.0 + 30), red, true);
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
		// Check if it's our reset button first:
		if (i == serialInput.size() - 1) {
			// Reset stuff if we have our button press
			if (serialInput[i] == 1) {
				for (int i = 0; i < shouldRunAnimation.size(); i++) {
					shouldRunAnimation[i] = false;
					animationCounter[i] = 0;
				}
			}
		} else {
			// If we have something going on with the serial input...
			if (serialInput[i] == 1) {
				// Increase the threshold number,
				touchThreshold[i]++;
				// and once that reaches 2 we can run the animation
				if (touchThreshold[i] >= 2) {
					// Reset the threshold
					touchThreshold[i] = 0;
					checkShouldRunAnimations(i);
				}
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
	ofPushStyle();
	ofBackground(0);
	background.draw(0, 0, fixedWidth, fixedHeight);
	buildingBottom.draw(1752 - 183, 652 - 90, 333, 260);
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	if (!shouldRunAnimation[0]) {
		handMarkers[0].draw();
	}
}

//--------------------------------------------------------------
void ofApp::drawAnimations(){
	bool runningAnyAnimations = false;
	// Run through the `shouldRunAnimation` array, and run any of the animations that we should
	for (int i = 0; i < shouldRunAnimation.size(); i++) {
		if (shouldRunAnimation[i]) {
			// We're running an animation! so this is true
			runningAnyAnimations = true;
			// Increase the animation counter,
			// which is what we're using to run the animation for
			// a set amount of time
			animationCounter[i]++;
			// Run the fucking animation
			runAnimation(i);
		}
	}
	// If we are running any animations, increment
	// the universal animation counter
	// and check if we should stop running all animations
	if (runningAnyAnimations) {
		universalAnimationCounter++;
		if (universalAnimationCounter >= universalAnimationCounterMax) {
			universalAnimationCounter = 0;
			for (int i = 0; i < shouldRunAnimation.size(); i++) {
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
	bool shouldRunOne = false;
	// If it's the first button, we should run the animation
	if (index == 0) { // Right Millenium Falcon
		// building animations
		shouldRunOne = true;
	} else if (index == 1) { // Left Millenium Falcon
		if (shouldRunAnimation[3]) {
			// companies animations
			shouldRunOne = true;
		}
	} else if (index == 2) { // Bottom building
		if (shouldRunAnimation[0]) {
			// Text animation
			shouldRunOne = true;
		}
	} else if (index == 3) { // Top building
		if (shouldRunAnimation[0]) {
			// Text animation
			shouldRunOne = true;
		}
	} else if (index == 4) { // Companies center
		if (shouldRunAnimation[1]) {
			// People animation
			shouldRunOne = true;
		}
	} else if (index == 5) { // Companies left
		if (shouldRunAnimation[1]) {
			// Aerial animation
			shouldRunOne = true;
		}
	} else if (index == 6) { // Aerial left
		if (shouldRunAnimation[5]) {
			// Aerial text left
			shouldRunOne = true;
		}
	} else if (index == 7) { // Aerial center
		if (shouldRunAnimation[5]) {
			// Aerial text center
			shouldRunOne = true;
		}
	} else if (index == 8) { // Aerial right
		if (shouldRunAnimation[5]) {
			// Aerial text right
			shouldRunOne = true;
		}
	} else if (index == 9) { // People
		if (shouldRunAnimation[4]) {
			// People text
			shouldRunOne = true;
		}
	}
	if (shouldRunOne) {
		shouldRunAnimation[index] = true;
		universalAnimationCounter = 0;
	}
}

//--------------------------------------------------------------
void ofApp::runAnimation(int animationNum){
	switch(animationNum) {
		// PRESS RIGHT MILLENIUM FALCON
		// ANIMATE BUILDINGS & MILLENIUM RIGHT TEXT
		case 0:
			ofPushStyle();
			buildingTopMovement.update(animationCounter[0]);
			buildingTopMovement.draw();
			for (int i = 0; i < buildingTopDashedLines.size(); i++) {
				buildingTopDashedLines[i].update(animationCounter[0]);
				buildingTopDashedLines[i].draw();
			}
			if (!shouldRunAnimation[2]) {
				milleniumRightText.update(animationCounter[0]);
				milleniumRightText.draw();
			}
			if (!shouldRunAnimation[2]) {
				handMarkers[2].draw();
			}
			if (!shouldRunAnimation[3] &&
				shouldRunAnimation[2]) {
				handMarkers[3].draw();
			}
			ofPopStyle();
			break;
		// PRESS LEFT MILLENIUM FALCON
		// ANIMATE COMPANIES AND MILLENIUM LEFT TEXT
		case 1:
			ofPushStyle();
			// companies animation
			companiesAnimation.update(animationCounter[1]);
			companiesAnimation.draw();
			// companies center hand marker
			if (!shouldRunAnimation[4]) {
				handMarkers[4].draw();
			}
			// comapanies dashed line
			companiesLine.update(animationCounter[1]);
			companiesLine.draw();
			// millenium left text
			if (!shouldRunAnimation[4]) {
				milleniumLeftText.update(animationCounter[1]);
				milleniumLeftText.draw();
			}
			ofPopStyle();
			break;
		// PRESS BOTTOM BUILDING
		// ANIMATE TEXT
		case 2:
			ofPushStyle();
			if (!shouldRunAnimation[3]) {
				buildingBottomText.update(animationCounter[2]);
				buildingBottomText.draw();
			}
			ofPopStyle();
			break;
		// PRESS TOP BUILDING
		// ANIMATE TEXT
		case 3:
			ofPushStyle();
			if (!shouldRunAnimation[1]) {
				buildingTopText.update(animationCounter[3]);
				buildingTopText.draw();
				handMarkers[1].draw();
			}
			ofPopStyle();
			break;
		// PRESS COMPANIES CENTER
		// ANIMATE PEOPLE
		case 4:
			ofPushStyle();
			peopleAnimation.update(animationCounter[4]);
			peopleAnimation.draw();
			// If we aren't running the people text animation
			if (!shouldRunAnimation[9]) {
				companiesCenterText.update(animationCounter[4]);
				companiesCenterText.draw();
				peoplePressLine.update(animationCounter[4]);
				peoplePressLine.draw();
				handMarkers[9].draw();
			}
			ofPopStyle();
			break;
		// PRESS COMPANIES LEFT
		// ANIMATE AERIAL
		case 5:
			ofPushStyle();
			aerialAnimation.update(animationCounter[5]);
			aerialAnimation.draw();
			if (!shouldRunAnimation[6] && !shouldRunAnimation[7] && !shouldRunAnimation[8]) {
				companiesLeftText.update(animationCounter[5]);
				companiesLeftText.draw();
			}
			aerialToBuildingLine.update(animationCounter[5]);
			aerialToBuildingLine.draw();
			if (!shouldRunAnimation[6]) {
				handMarkers[6].draw();
			}
			if (!shouldRunAnimation[7]) {
				handMarkers[7].draw();
			}
			if (!shouldRunAnimation[8]) {
				handMarkers[8].draw();
			}
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
			// companies left hand marker
			if (!shouldRunAnimation[5]) {
				peopleText.update(animationCounter[9]);
				peopleText.draw();
				handMarkers[5].draw();
			}
			ofPopStyle();
			break;
		default:
			cout << "WHAT FUCKING KEY IS THIS?? " << animationNum << endl;
			break;
	}
}
