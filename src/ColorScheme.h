#pragma once

#include "ofTypes.h"

class ColorScheme {
public:
	ofColor background, print, hair, face, mouth, teeth, eyelid, eye;
	static ColorScheme const& getScheme(int i=-1);
private:
	static vector<ColorScheme> theSchemes;
	static void populateSchemes();
};
