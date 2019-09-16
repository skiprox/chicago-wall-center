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
	people.load("images/people-2.png");
}

//--------------------------------------------------------------
void ofApp::setupAnimations(){
	/**
	 * ALL THE LINE ANIMATIONS GO HERE
	 */
	vector<std::array<glm::vec2, 2>> pts;
	// Upper Building Dashed Lines
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1615, 1025), glm::vec2(1615, 430)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1670, 1069), glm::vec2(1670, 474)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1725, 1071), glm::vec2(1725, 476)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1780, 1000), glm::vec2(1780, 405)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1835, 945), glm::vec2(1835, 350)}});
	buildingTopDashedLines.push_back(DashedLine(pts, 2.0, red, 250, false));
	pts.clear();
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1890, 881), glm::vec2(1890, 286)}});
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
	// Aerial to upper building dashed lines
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(890, 375), glm::vec2(1350, 375)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1350, 375), glm::vec2(1350, 275)}});
	pts.push_back(array<glm::vec2, 2> {{glm::vec2(1350, 275), glm::vec2(1555, 275)}});
	aerialToBuildingLine = DashedLine(pts, 2.0, red, 250, true);
	pts.clear();
	/**
	 * ALL THE PRESS POINTS
	 */
	// 0, millenium right
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(1345, 815), glm::vec2(1500, 885)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	pressPts.clear();
	// 1, millenium left
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(1100, 885), glm::vec2(1336, 920)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 2, building bottom
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(1475, 480), glm::vec2(1550, 556)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 3, building top
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(1412, 465), glm::vec2(1530, 350)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 4. companies center
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(733, 715), glm::vec2(882, 805)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 5. companies right
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(205, 975), glm::vec2(420, 975)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 6, aerial left
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(90, 600), glm::vec2(235, 495)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 7, aerial center
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(-100, -100), glm::vec2(-100, -100)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 8, aerial right
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(1069, 420), glm::vec2(915, 475)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
	// 9, people
	pressPts.push_back(array<glm::vec2, 2> {{glm::vec2(800, 700), glm::vec2(970, 635)}});
	pressLines.push_back(DottedLine(pressPts, 5.0, red, 75, true));
	peoplePressLine = DottedLine(pressPts, 5.0, red, 75, true);
	pressPts.clear();
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
		glm::vec2(fixedWidth/2.0 + 244, fixedHeight/2.0 + 80),
		glm::vec2(430, 223),
		250
	);
	/**
	 * ALL THE IMAGE MOVEMENTS GO HERE
	 */
	buildingTopMovement = ImageMovement(
		buildingTop,
		glm::vec2(338, 268),
		glm::vec2(1745, 975),
		glm::vec2(1745, 350),
		250
	);
	/**
	 * ALL THE TEXT TYPING GO HERE
	 */
	layerZeroText = TextTyping(
		"Most call it “free market”, but it is based on generous regulations, subsidies, and the milking of urban commons that feed and reproduce\nit. We call it the urbanization of “global capitalism.” Cities laying out red carpets for extracting private profits from all things public, red\ncarpets that perpetuate the concentration of opportunities into the hands of a global elite, that feed from the corruption of democratic\npower, spatial segregation, digital disruption, patriarchal dominance, cultural manipulation, and environmental destruction. red carpets\nfor public-private partnerships with opaque governance, hidden in large urban infrastructures, ports, airports, tax-free and logistic\nzones, economic promotion agencies and fairgrounds for large mega-events.\n\nBarcelona remains a perfect example of a city with a robust, globally weaved red carpet network of for-profit organizations that abuse\npublic partnership. It is a world reference city, full of real estate opportunities and a myriad of tourist, cultural and event attractions; a\ncity threatened by predatory global investments that drive social injustices, displacements, gentrification, mass tourism, labor conflicts,\nand pollution. A rebel and fearless city, governed by a municipalist platform whose democratic radicalism constitutes a beacon for social\nmovements around the world. A submissive city, eaten away by a powerful and opaque mesh of public-private partnerships with a high\ncapacity to influence the management of strategic public goods.\n\nNevertheless, the grassroots movements of the rebel Barcelona do not seem to give up this urban common without fighting against the\ndecision of the municipal government and the lobbies that pressure it. More than 55 neighborhood associations, environmental groups\nand unions in defense of the right to housing and the right to the city have rallied around the new platform “La Fira o la Vida” —The\nFairground or Life— with a twofold aim. On the one hand, they oppose the privatization of the fairgrounds to move forward without the\nminimum guarantees of transparency and citizen participation. On the other, they complemented their protest with a concrete and viable\nproposal. Instead of an enclosed fairground dedicated exclusively to private mega-events, they propose an open and diverse neighbo\nhood. A neighborhood with retail shops, productive spaces, proximity facilities, and thousands of public housing units. A Mediterranean\nneighborhood, with narrow streets, human-scaled squares and green corridors. An affordable, productive, and walkable neighborhood\nthat contributes to combat the effects of gentrification, air pollution, or the climate crisis. A community that perfectly expresses the\ncommitment of the URBAN FRONT: in the face of the social and environmental abuses of neoliberal urbanism, we do need concrete\nalternatives that demonstrate that a fairer and more sustainable city is possible.",
		glm::vec2(450, 580),
		ofColor(255),
		10,
		250
	);
	milleniumRightText = TextTyping(
		"THE ANNOUNCEMENT OF THE “FIRA UNIV-\nERS” PROJECT\n\nGeneralitat de Catalunya as well as representa-\ntives of Barcelona's Chamber of Commerce and\nthe public-private entity Fira de Barcelona\nannounced a closed-door deal. They revealed\n“Fira Univers,” an institutional project to support\nthe investment of 380 million euros for the renova-\ntion of the Montjuïc fairgrounds and the expansion\nof the l'Hospitalet fairgrounds to be inaugurated in\n2029 for the 100th anniversary of the 1929 Interna-\ntional Exhibition. Even though the fairgrounds\nstand in municipal land, the details of the agree-\nment were not made public and until today are\nunknown. There was no public debate or consulta-\ntion before the signing of the deal. These actions\nhave recently put to question the new priorities of\nthe government of the City of Barcelona; a\nself-professed progressive government that won\nelections in 2015 with the public promise to\n“kick-out mafias and lobbies.” Without any public\ndebate, the Montjuïc fairgrounds and its manage-\nment company Fira de Barcelona continues to be\na highly profitable, opaque, and carefully guarded\npublic-private organization dedicated to promote\nmega-events for the global capitalist elites.",
		glm::vec2(1360, 340),
		ofColor(255),
		10,
		250
	);
	milleniumLeftText = TextTyping(
		"PUBLIC LAND FOR PRIVATE MEGA EVENTS\n\nThe Montjuïc fairground occupies 27 hectares of\nmunicipal land, in a very central location of Barce-\nlona. Its size is equivalent to 21 blocks of the\nfamous Cerdá Eixample or the full surface of La\nBarceloneta neighborhood where more than\n16,000 people live. The for-profit public-private\nFira de Barcelona company controls the site since\n1932. The current concession is due to expire on\nDecember 2025. Under this concession, Fira de\nBarcelona pays 0,25 euros per year for each\n140,000 square meter building in the site. This is\nthe equivalent of paying less than 2 euros a month\nfor a home of 80m² right in the heart of the city.\nThe concession also allows Fira de Barcelona the\nfor-profit use of all the public spaces inside the\nfairgrounds. They are even allowed to close these\npublic spaces for the benefit of the mega-events\nthey host. These concessions have demarcated\nthe urban fragmentation of its adjacent neighbor-\nhoods of Poble-sec and Font de la Guatlla. Most of\nthe buildings in the fairground have no historical\nvalue and are poorly maintained.",
		glm::vec2(320, 860),
		ofColor(255),
		10,
		250
	);
	buildingBottomText = TextTyping(
		"THE MEGA EVENTS OF FIRA DE BARCELONA\n\nEvery year, Fira de Barcelona hosts approximately\n150 events, and it is visited by over 2 million\npeople from across the globe. Rather than promot-\ning activities that benefit local initiatives and\ndevelopment, the vast majority of these events\nencourage the business of large multinational\ncorporations, such as mobile phone companies,\nautomobile industries, predatory real-estate\nexhibitions, agro-businesses, and mass tourism\nenterprises. Amongst its most recognized events\nis the International Real-Estate Fair, where notori-\nous vulture fund companies like Blackstone, TPG\nCapital, Lone Star, Capital Management as well as\nthe speculative banking sectors of Credit Suisse,\nMorgan Stanley, JP Morgan and Crédit Agricole fill\nthe halls of the Municipally owned buildings of its\ntwo fairgrounds, which have a legal mandate to\nserve its citizens and promote the public good.",
		glm::vec2(1450, 670),
		ofColor(255),
		10,
		250
	);
	buildingTopText = TextTyping(
		"AN OPEN, DIVERSE AND AFFORDABLE\nNEIGHBORHOOD IN THE CURRENT MONTJUÌC\nFAIRGROUND\n\nThe grassroots coalition #LaFiraOLaVida has been\nenvisioning a new type of urban development for\nthe future of the Montjuïc fairgrounds. Against the\ngated nature of the current fairground, they\nimagine an open landscape, weaved by continu\nous pedestrian streets that connect the adjacent\nneighborhoods of Poble-sec and Font de la\nGuatlla. They envision the site's main avenue as a\nlinear public park, full of endogenous vegetation\nand spaces for public gather that flow upwards\ninto the hill of Montjuïc and downwards into Plaça\nd'Espanya. They envision a compact neighbor\nhood, designed to house a density of thousands of\nlivelihoods and mixed uses. #LaFiraOLaVida\nprojects an entirely walkable community built on a\nsurplus of affordable living and working spaces\nthat will be permanently welcoming for all of\nBarcelona.",
		glm::vec2(1420, 350),
		ofColor(255),
		10,
		250
	);
	companiesCenterText = TextTyping(
		"FIRA DE BARCELONA AND THE CHAMBER OF\nCOMMERCE\n\nFira de Barcelona is a public-private consortium\nformed in 1932 by the City of Barcelona, the\nGeneralitat de Catalunya and the Chamber of\nCommerce. This last one is an independent public\ncorporation that promotes and offers services to\nbusiness entities. Fira de Barcelona occupies two\nseparate fairgrounds on municipal land. The oldest\nof the two offers 140,000m² of expo surface in the\ncenter of Barcelona, right next to the hill of Mon-\ntjuïc. The newest was built in 2007 in the adjacent\nmunicipality of l'Hospitalet but was financed by\nthe City of Barcelona. It offers 240,000m² of expo\nsurface. In 2017, the City of Barcelona gave to Fira\nde Barcelona in limited concession the Internation-\nal Convention Center of Barcelona, which provides\nanother 75,000m² of surface located in the water-\nfront. Fira de Barcelona has an annual revenue of\n210 million euros, while the complete housing\nyearly budget of the City of Barcelona is 165\nmillion euros.",
		glm::vec2(320, 860),
		ofColor(255),
		10,
		250
	);
	companiesLeftText = TextTyping(
		"THE LAUNCH OF THE CITIZEN PLATFORM\n#LAFIRAOLAVIDA\n\nOn May 30, 2019, a grassroots coalition of over 60\nsocial movements publicly announced its opposi-\ntion to the City of Barcelona's intention to extend\nthe concession of the Montjuïc fairground to Fira\nde Barcelona. The coalition took the name \n#LaFiraOLaVida (the fair or life). Its main objectives\nare: first, to denounce the lack of transparency\nand anti-democratic character of the closed-door\ndeal; and second, to propose alternatives for\nconverting the fairground into an urban common.\n#LaFiraOLaVida presented a public manifesto\ndemanding the halt of all plans to extend the \nconcession and the renovation of the fairground\nand that the city opens a public consultation\ninstead. #LaFiraOLaVida imagines the future of \nthe fairground as an inclusive, open, and diverse\nneighborhood full of affordable housing, retail\nshops, proximity facilities, walkable streets, and\ngreen public spaces of human scale.",
		glm::vec2(260, 900),
		ofColor(255),
		10,
		250
	);
	peopleText = TextTyping(
		"THE GRASSROOTS MOVEMENTS ENDORS\nING #LAFIRAOLAVIDA\n\nSince its inception, the grassroots coalition\n#LaFiraOLaVida keeps growing and it already\nincludes over 60 social movements, neighborhood\norganizations and civic platforms. From a national\nand city-scale it includes big entities like Sindicat\nde Llogaters, PAH Barcelona or Associació\n500x20 (housing), ATTAC Catalunya, Assemblea\nde Barris per un Turisme Sostenible or Marea\nPensionista (economy), Ecologistes en Acció,\nPlataforma per la Qualitat de l'Aire or Associació\nSalut i Agroecologia (ecology) and Procés Constit-\nuent (democracy). It also includes multiple neigh-\nborhood organizations like Sindicat de Barri del \nPoble-sec, Fem Sant Antoni, Assemblea del Barri\nde Sants, Espai Comunitari Autogestionat de Can\nBatlló, Espai Feminista de Sants, Associació\nCultural El Lokal del Raval, Plataforma en Defensa\nde la Barceloneta, Oficina d'Habitatge Popular de\nGràica, Associació de Veïns i Veïnes de la Sagrada\nFamília.",
		glm::vec2(860, 770),
		ofColor(255),
		10,
		250
	);
	aerialLeftText = TextTyping(
		"THE NEW NEIGHBORHOOD AND ECOLOGY\n\nIn contrast, the proposed new neighborhood\nwould replace the fairground with a socially\nsustainable and environmentally resilient urban\nmodel. It prioritizes the design and implementa\ntion of environmental justice principles, which are\nat the core of the Urban Front ethos. The design\nprerogative for density and compactness provides\nthe spatial layout for a neighborhood built on\npedestrian, bicycle, and public infrastructure. A\ndiverse local workforce composed of construction\ncooperatives would erect new buildings that\nutilize renewable resources and new technologies\nin thermal efficiency and recycling. The creation\nof collectivized facilities, community-controlled\nutilities, and spaces to pursue food sovereignty\nwould be elemental.",
		glm::vec2(200, 415),
		ofColor(255),
		10,
		250
	);
	aerialCenterText = TextTyping(
		"THE NEW NEIGHBORHOOD AND ECONOMY\n\nAn essential part of # LaFiraOLaVida's proposal for\nthe fairground is the inclusion of local social \ninitiatives and solidarity networks. The project puts\nforward a neighborhood that is co-produced by\nautonomous collective worker groups, small\nenterprises, non-profit cooperatives, cultural\ninitiatives, as well as socially productive sites that\npursue fair trade, environmental justice, and the\ncommon good.",
		glm::vec2(590, 590),
		ofColor(255),
		10,
		250
	);
	aerialRightText = TextTyping(
		"THE NEW NEIGHBORHOOD AND HOUSING\n\nThe proposed new neighborhood would replace\nthe derelict buildings in the Montjuïc fairground \nfor thousands of new affordable housing units\n—between 5,000 and 10,000, depending on the\ndensity— that would help to improve Barcelona's\nhousing crisis significantly and to contra-rest the\nintensifying gentrification of adjacent neighbor-\nhoods as Poble-sec and Sant Antoni. Far from\nbeing a monofunctional neighborhood or a social\nghetto-like so many peripheral dormitory cities,\nit would be a community-controlled central neigh-\nborhood that would embrace a mixture of func\ntions and populations in a dense fabric built by\nlocal construction cooperatives and organiza-\ntions.",
		glm::vec2(1010, 450),
		ofColor(255),
		10,
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
	buildingBottom.draw(1579, 528, 333, 260);
	if (!shouldRunAnimation[0]) {
		layerZeroText.update(500);
		layerZeroText.draw();
	}
	ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::drawHandMarkers(){
	if (!shouldRunAnimation[0]) {
		handMarkers[0].draw();
		ofPushStyle();
		ofSetColor(red);
		ofFill();
		ofDrawCircle(fixedWidth - 200, fixedHeight - 115, 80);
		ofPopStyle();
		pressLines[0].update(500);
		pressLines[0].draw();
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
		if (shouldRunAnimation[2]) {
			// Text animation
			shouldRunOne = true;
		}
	} else if (index == 4) { // Companies center
		if (shouldRunAnimation[1]) {
			// People animation
			shouldRunOne = true;
		}
	} else if (index == 5) { // Companies left
		if (shouldRunAnimation[9]) {
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
				pressLines[2].update(animationCounter[0]);
				pressLines[2].draw();
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
				pressLines[4].update(animationCounter[1]);
				pressLines[4].draw();
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
				handMarkers[3].draw();
				pressLines[3].update(animationCounter[2]);
				pressLines[3].draw();
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
				pressLines[1].update(animationCounter[3]);
				pressLines[1].draw();
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
				pressLines[9].update(animationCounter[4]);
				pressLines[9].draw();
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
				pressLines[6].update(animationCounter[5]);
				pressLines[6].draw();
			}
			if (!shouldRunAnimation[7]) {
				//handMarkers[7].draw();
			}
			if (!shouldRunAnimation[8]) {
				handMarkers[8].draw();
				pressLines[8].update(animationCounter[5]);
				pressLines[8].draw();
			}
			ofPopStyle();
			break;
		// PRESS AERIAL LEFT
		// ANIMATE TEXT
		case 6:
			ofPushStyle();
			aerialLeftText.update(animationCounter[6]);
			aerialLeftText.draw();
			aerialCenterText.update(animationCounter[6]);
			aerialCenterText.draw();
			ofPopStyle();
			break;
		// PRESS AERIAL CENTER
		// ANIMATE TEXT
		case 7:
			ofPushStyle();
			// aerialCenterText.update(animationCounter[7]);
			// aerialCenterText.draw();
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
				pressLines[5].update(animationCounter[9]);
				pressLines[5].draw();
			}
			ofPopStyle();
			break;
		default:
			cout << "WHAT FUCKING KEY IS THIS?? " << animationNum << endl;
			break;
	}
}
