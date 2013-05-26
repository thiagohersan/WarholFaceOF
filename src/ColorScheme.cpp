#include "ColorScheme.h"

vector<ColorScheme> ColorScheme::theSchemes = vector<ColorScheme>();
unsigned int ColorScheme::currentSchemeIndex = 0;

ColorScheme const& ColorScheme::getScheme(int i){
	// first time this is called, fill it with stuff
	if(theSchemes.size() == 0){
		populateSchemes();
	}

	// proceed
	currentSchemeIndex = (i < 0)?(int)ofRandom(theSchemes.size()):((currentSchemeIndex+i)%theSchemes.size());
	return theSchemes.at(currentSchemeIndex);
}

void ColorScheme::populateSchemes(){
	// inpsiration:
	//     http://www.webexhibits.org/colorart/marilyns.html

	ColorScheme temp;

	temp.background = ofColor(90,130,110);
	temp.print = ofColor(0,0,0);
	temp.hair = ofColor(230,190,65);
	temp.face = ofColor(235,170,155);
	temp.mouth = ofColor(220,50,30);
	temp.teeth = ofColor(250,245,225);
	temp.eyelid = ofColor(175,180,185);
	temp.eye = ofColor(95,145,125);
	theSchemes.push_back(temp);

	temp.background = ofColor(205,75,100);
	temp.print = ofColor(130,40,85);
	temp.hair = ofColor(145,175,75);
	temp.face = ofColor(240,210,55);
	temp.mouth = ofColor(110,45,70);
	temp.teeth = ofColor(145,175,75);
	temp.eyelid = ofColor(210,70,90);
	temp.eye = ofColor(155,160,55);
	theSchemes.push_back(temp);

	temp.background = ofColor(215,100,70);
	temp.print = ofColor(20,15,20);
	temp.hair = ofColor(220,185,55);
	temp.face = ofColor(195,155,165);
	temp.mouth = ofColor(90,25,50);
	temp.teeth = ofColor(205,200,205);
	temp.eyelid = ofColor(50,95,65);
	temp.eye = ofColor(95,115,105);
	theSchemes.push_back(temp);

	temp.background = ofColor(0,180,215);
	temp.print = ofColor(80,80,85);
	temp.hair = ofColor(245,130,35);
	temp.face = ofColor(250,185,215);
	temp.mouth = ofColor(80,85,90);
	temp.teeth = ofColor(185,210,235);
	temp.eyelid = ofColor(0,180,220);
	temp.eye = ofColor(250,160,50);
	theSchemes.push_back(temp);

	temp.background = ofColor(65,130,175);
	temp.print = ofColor(125,100,110);
	temp.hair = ofColor(220,70,5);
	temp.face = ofColor(215,130,145);
	temp.mouth = ofColor(90,100,125);
	temp.teeth = ofColor(95,180,210);
	temp.eyelid = ofColor(80,130,150);
	temp.eye = ofColor(165,70,85);
	theSchemes.push_back(temp);

	temp.background = ofColor(215,140,160);
	temp.print = ofColor(65,95,25);
	temp.hair = ofColor(230,130,35);
	temp.face = ofColor(130,180,205);
	temp.mouth = ofColor(125,65,65);
	temp.teeth = ofColor(220,60,120);
	temp.eyelid = ofColor(220,155,150);
	temp.eye = ofColor(75,80,45);
	theSchemes.push_back(temp);

	temp.background = ofColor(200,17,0);
	temp.print = ofColor(0,0,0);
	temp.hair = ofColor(255,245,0);
	temp.face = ofColor(254,231,152);
	temp.mouth = ofColor(194,16,0);
	temp.teeth = ofColor(250,245,230);
	temp.eyelid = ofColor(200,17,0);
	temp.eye = ofColor(244,234,0);
	theSchemes.push_back(temp);

	temp.background = ofColor(225,14,135);
	temp.print = ofColor(0,5,25);
	temp.hair = ofColor(187,26,0);
	temp.face = ofColor(178,162,0);
	temp.mouth = ofColor(194,0,166);
	temp.teeth = ofColor(250,220,250);
	temp.eyelid = ofColor(225,14,135);
	temp.eye = ofColor(187,26,0);
	theSchemes.push_back(temp);

	temp.background = ofColor(36,81,90);
	temp.print = ofColor(35,45,80);
	temp.hair = ofColor(54,211,0);
	temp.face = ofColor(112,95,171);
	temp.mouth = ofColor(150,137,0);
	temp.teeth = ofColor(150,190,150);
	temp.eyelid = ofColor(36,81,90);
	temp.eye = ofColor(54,211,0);
	theSchemes.push_back(temp);

	temp.background = ofColor(250,98,140);
	temp.print = ofColor(39,62,52);
	temp.hair = ofColor(236,225,0);
	temp.face = ofColor(130,160,140);
	temp.mouth = ofColor(233,83,165);
	temp.teeth = ofColor(233,240,250);
	temp.eyelid = ofColor(250,98,140);
	temp.eye = ofColor(236,225,0);
	theSchemes.push_back(temp);

	temp.background = ofColor(76,140,0);
	temp.print = ofColor(0,0,0);
	temp.hair = ofColor(150,74,252);
	temp.face = ofColor(180,180,117);
	temp.mouth = ofColor(36,50,2);
	temp.teeth = ofColor(76,140,0);
	temp.eyelid = ofColor(36,50,2);
	temp.eye = ofColor(150,74,252);
	theSchemes.push_back(temp);

	temp.background = ofColor(40,83,118);
	temp.print = ofColor(56,47,71);
	temp.hair = ofColor(196,90,225);
	temp.face = ofColor(37,102,78);
	temp.mouth = ofColor(196,90,225);
	temp.teeth = ofColor(138,152,0);
	temp.eyelid = ofColor(40,83,118);
	temp.eye = ofColor(196,90,225);
	theSchemes.push_back(temp);
}
