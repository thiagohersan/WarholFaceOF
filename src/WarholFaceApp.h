#pragma once

#include "ofMain.h"
#include "ofxCv.h"
#include "ofxOpenCv.h"
#include "ofxFaceTracker.h"
#include "ColorScheme.h"

class FaceFeatures{
public:
	ofPolyline face, outMouth, inMouth;
	ofPolyline rightEyebrow, leftEyebrow;
	ofPolyline rightEye, leftEye;
	ofVec2f rightEyeTrans, leftEyeTrans;
	ofVec2f faceTrans;
	ofRectangle cropArea;
	static void blowUpPolyline(ofPolyline &pl);
};

class WarholFaceApp : public ofBaseApp{
	
public:
	void setup();
	void update();
	void draw();
	
	void thresholdCam(ofVideoGrabber &in, ofImage &out);
	void makeBlackTransparent(ofxCvGrayscaleImage &in, ofImage &out);

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

	ofxCvColorImage cImg;
	ofxCvGrayscaleImage grayDiff;
	ofxCvGrayscaleImage previousFrames[2];
	unsigned int previousFramesIndex;

	ofImage printLayer, hairLayer;
	unsigned char thresholdValue;

	long long int lastFaceTime;
	ColorScheme currentColorScheme;

	ofFbo faceFbos[12];
	FaceFeatures mFaceFeatures;
	unsigned int scaleFactor;

	ofxFaceTracker tracker;

private:
	void drawFace();
};
