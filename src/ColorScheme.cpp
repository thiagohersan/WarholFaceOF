#include "ColorScheme.h"

vector<ColorScheme> ColorScheme::theSchemes = vector<ColorScheme>();

ColorScheme const& ColorScheme::getScheme(int i){
	// first time this is called, fill it with stuff
	if(theSchemes.size() == 0){
		populateSchemes();
	}

	// proceed
	if(i == -1){
		return theSchemes.at((int)ofRandom(theSchemes.size()));
	}
	else{
		return theSchemes.at(i%theSchemes.size());
	}
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

	temp.background = ofColor(215,40,90);
	temp.print = ofColor(45,60,120);
	temp.hair = ofColor(210,150,130);
	temp.face = ofColor(215,55,15);
	temp.mouth = ofColor(45,60,80);
	temp.teeth = ofColor(215,155,0);
	temp.eyelid = ofColor(210,45,85);
	temp.eye = ofColor(190,115,115);
	theSchemes.push_back(temp);

	temp.background = ofColor(180,205,105);
	temp.print = ofColor(165,30,20);
	temp.hair = ofColor(245,195,40);
	temp.face = ofColor(125,170,65);
	temp.mouth = ofColor(210,40,60);
	temp.teeth = ofColor(205,65,110);
	temp.eyelid = ofColor(250,200,115);
	temp.eye = ofColor(200,165,90);
	theSchemes.push_back(temp);
}
