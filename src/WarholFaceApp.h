#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxFaceTracker.h"
#include "ColorScheme.h"

class WarholFaceApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void thresholdCam(ofVideoGrabber &in, ofImage &out);
	void makeBlackTransparent(ofxCvGrayscaleImage &in, ofImage &out);
	void blowUpPolyline(ofPolyline &pl);

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	
	ofVideoGrabber cam;

	ofxCvGrayscaleImage grayDiff;
	ofxCvColorImage cImg;
	
	ofxCvGrayscaleImage previousFrames[2];
	unsigned int previousFramesIndex;

	ofImage printLayer, hairLayer;
	unsigned char thresholdValue;

	long long int lastFaceTime;
	ColorScheme currentColorScheme;

	ofxFaceTracker tracker;
};
