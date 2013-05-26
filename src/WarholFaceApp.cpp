#include "WarholFaceApp.h"

using namespace ofxCv;

//--------------------------------------------------------------
void WarholFaceApp::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	cam.initGrabber(640, 480);

	cImg.allocate(cam.width,cam.height);
	grayDiff.allocate(cam.width,cam.height);
	for(int i=0; i<2; ++i){
		previousFrames[i].allocate(cam.width, cam.height);
	}
	previousFramesIndex = 0;

	hairLayer.allocate(cam.width,cam.height, OF_IMAGE_COLOR_ALPHA);
	printLayer.allocate(cam.width,cam.height, OF_IMAGE_COLOR_ALPHA);
	thresholdValue = 40;

	lastFaceTime = ofGetElapsedTimeMillis();
	currentColorScheme = ColorScheme::getScheme(0);

	tracker.setup();
	tracker.setRescale(.5);
}

//--------------------------------------------------------------
void WarholFaceApp::update(){
	cam.update();
	if(cam.isFrameNew()) {
		// face detection
		tracker.update(toCv(cam));
		// get a copy for the print layer
		thresholdCam(cam,printLayer);

		// keep previous camera color image as a grayscale cv image
		previousFrames[previousFramesIndex] = cImg;
		previousFrames[previousFramesIndex].blur(23);
		previousFramesIndex = (previousFramesIndex+1)%2;

		// get new camera image
		cImg.setFromPixels(cam.getPixelsRef());
		grayDiff = cImg;
		//grayDiff.blur();

		// get diff between frames with blur to get person outline
		previousFrames[previousFramesIndex].absDiff(grayDiff);
		previousFrames[previousFramesIndex].blur(17);
		previousFrames[previousFramesIndex].threshold(4,false);
		// makes a layer where outline is white and everything else is transparent
		makeBlackTransparent(previousFrames[previousFramesIndex],hairLayer);
	}
}

//--------------------------------------------------------------
void WarholFaceApp::draw(){
	ofBackground(0);
	ofSetColor(255);

	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, cam.height+20);

	if(tracker.getFound()) {
		// if this is a new face, get a new color
		if(ofGetElapsedTimeMillis()-lastFaceTime > 1000){
			currentColorScheme = ColorScheme::getScheme();
		}

		// get poly lines
		ofPolyline face = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
		ofPolyline outMouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
		ofPolyline inMouth = tracker.getImageFeature(ofxFaceTracker::INNER_MOUTH);
		ofPolyline rightEyebrow = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYEBROW);
		ofPolyline leftEyebrow = tracker.getImageFeature(ofxFaceTracker::LEFT_EYEBROW);
		ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
		ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);

		// for eye lids
		blowUpPolyline(rightEye);
		blowUpPolyline(leftEye);
		ofVec2f rightEyeTrans = rightEye.getBoundingBox().getCenter()-rightEyebrow.getBoundingBox().getCenter();
		ofVec2f leftEyeTrans = leftEye.getBoundingBox().getCenter()-leftEyebrow.getBoundingBox().getCenter();
		
		// bounding box
		float x0 = face.getBoundingBox().x-face.getBoundingBox().width/1.666;
		float y0 = (face.getBoundingBox().y+face.getBoundingBox().height*1.1)-face.getBoundingBox().width*2.2;
		if(y0 < 0) y0 = 0;
		ofTranslate(-x0,-y0);

		// draw shapes and color

		// BACKGROUND
		ofSetColor(currentColorScheme.background);
		ofRect(0,0, cam.width,cam.height);

		// HAIR
		ofSetColor(currentColorScheme.hair);
		hairLayer.draw(0,0);

		// FACE
		ofSetColor(currentColorScheme.face);
		ofBeginShape();
		ofVertices(face.getVertices());
		ofEndShape();
		
		// FOREHEAD
		ofSetColor(currentColorScheme.face);
		ofVec2f faceTrans = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE).getCentroid2D()-inMouth.getCentroid2D();
		faceTrans.normalize();
		ofPushMatrix();
		ofTranslate(0.29*faceTrans*face.getBoundingBox().height);
		ofBeginShape();
		ofVertices(face.getVertices());
		ofEndShape();
		ofPopMatrix();

		// MOUTH
		ofSetColor(currentColorScheme.mouth);
		ofBeginShape();
		ofVertices(outMouth.getVertices());
		ofEndShape();
		ofSetColor(currentColorScheme.teeth);
		ofBeginShape();
		ofVertices(inMouth.getVertices());
		ofEndShape();

		// RIGHT EYE LID
		ofPushMatrix();
		ofSetColor(currentColorScheme.eyelid);
		ofTranslate(-rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(rightEye.getVertices());
		ofEndShape();
		ofSetColor(currentColorScheme.face);
		ofTranslate(rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(rightEye.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		// LEFT EYE LID
		ofPushMatrix();
		ofSetColor(currentColorScheme.eyelid);
		ofTranslate(-leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(leftEye.getVertices());
		ofEndShape();
		ofSetColor(currentColorScheme.face);
		ofTranslate(leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(leftEye.getVertices());
		ofEndShape();
		ofPopMatrix();

		// RIGHT EYE
		ofSetColor(currentColorScheme.eye);
		ofCircle(rightEye.getCentroid2D(), 0.5*min(rightEye.getBoundingBox().getWidth(),rightEye.getBoundingBox().getHeight()));
		
		// LEFT EYE
		ofSetColor(currentColorScheme.eye);
		ofCircle(leftEye.getCentroid2D(), 0.5*min(leftEye.getBoundingBox().getWidth(),leftEye.getBoundingBox().getHeight()));
		
		// PRINT
		printLayer.draw(0,0);
		
		// update timer
		lastFaceTime = ofGetElapsedTimeMillis();
	}
}

void WarholFaceApp::blowUpPolyline(ofPolyline &pl){
	ofPoint center = pl.getBoundingBox().getCenter();
	for(int i=0; i<pl.size(); ++i){
		pl[i] += pl[i]-center;
	}
}

// destructive: changes incoming img
void WarholFaceApp::thresholdCam(ofVideoGrabber &in, ofImage &out){
	unsigned char *op = out.getPixels();
	unsigned char *ip = in.getPixels();
	for(int i=0; i<in.height*in.width; ++i){
		float gray = 0.21*float(ip[i*3+0]) + 0.71*float(ip[i*3+1]) + 0.07*float(ip[i*3+2]);
		op[i*4+0] = op[i*4+1] = op[i*4+2] = (gray < thresholdValue)?0:255;
		op[i*4+3] = (gray < thresholdValue)?255:0;
	}
	out.update();
}

// makes black pixels transparent
void WarholFaceApp::makeBlackTransparent(ofxCvGrayscaleImage &in, ofImage &out){
	unsigned char *ip = in.getPixels();
	unsigned char *op = out.getPixels();
	for(int i=0; i<in.height*in.width; ++i){
		op[i*4+0] = op[i*4+1] = op[i*4+2] = (ip[i] < 10)?0:255;
		op[i*4+3] = (ip[i] < 10)?0:255;
	}
	out.update();
}



//--------------------------------------------------------------
void WarholFaceApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void WarholFaceApp::keyReleased(int key){
	tracker.reset();
	if(key == '-' || key == '_'){
		thresholdValue = (thresholdValue>0)?(thresholdValue-1):thresholdValue;
	}
	if(key == '+' || key == '='){
		thresholdValue = (thresholdValue<255)?(thresholdValue+1):thresholdValue;
	}
}

//--------------------------------------------------------------
void WarholFaceApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void WarholFaceApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void WarholFaceApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void WarholFaceApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void WarholFaceApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void WarholFaceApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void WarholFaceApp::dragEvent(ofDragInfo dragInfo){
	
}