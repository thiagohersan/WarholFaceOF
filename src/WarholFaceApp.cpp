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

	for(int i=0; i<12; ++i){
		faceFbos[i].allocate(cam.width, cam.height);
	}
	//mFaceFeatures.cropArea.set(0,0,cam.width, cam.height);

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
		previousFrames[previousFramesIndex].blur(27);
		previousFrames[previousFramesIndex].threshold(4,false);
		// makes a layer where outline is white and everything else is transparent
		makeBlackTransparent(previousFrames[previousFramesIndex],hairLayer);
	}
	
	////
	if(tracker.getFound()) {
		// if this is a new face, get a new color
		if(ofGetElapsedTimeMillis()-lastFaceTime > 1000){
			currentColorScheme = ColorScheme::getScheme();
		}

		// get poly lines
		mFaceFeatures.face = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
		mFaceFeatures.outMouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
		mFaceFeatures.inMouth = tracker.getImageFeature(ofxFaceTracker::INNER_MOUTH);
		mFaceFeatures.rightEyebrow = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYEBROW);
		mFaceFeatures.leftEyebrow = tracker.getImageFeature(ofxFaceTracker::LEFT_EYEBROW);
		mFaceFeatures.rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
		mFaceFeatures.leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);

		// for forehead
		mFaceFeatures.faceTrans = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE).getCentroid2D()-mFaceFeatures.inMouth.getCentroid2D();
		mFaceFeatures.faceTrans.normalize();

		// for eye lids
		FaceFeatures::blowUpPolyline(mFaceFeatures.rightEye);
		FaceFeatures::blowUpPolyline(mFaceFeatures.leftEye);
		mFaceFeatures.rightEyeTrans = mFaceFeatures.rightEye.getBoundingBox().getCenter()-mFaceFeatures.rightEyebrow.getBoundingBox().getCenter();
		mFaceFeatures.leftEyeTrans = mFaceFeatures.leftEye.getBoundingBox().getCenter()-mFaceFeatures.leftEyebrow.getBoundingBox().getCenter();

		// bounding box
		float x0 = mFaceFeatures.face.getBoundingBox().x-mFaceFeatures.face.getBoundingBox().width/1.666;
		float y0 = (mFaceFeatures.face.getBoundingBox().y+mFaceFeatures.face.getBoundingBox().height*1.1)-mFaceFeatures.face.getBoundingBox().width*2.2;
		if(y0 < 0) y0 = 0;
		mFaceFeatures.cropArea.set(x0,y0,mFaceFeatures.face.getBoundingBox().width*2.2,mFaceFeatures.face.getBoundingBox().width*2.2);
		
		// update timer
		lastFaceTime = ofGetElapsedTimeMillis();
	}
}

void WarholFaceApp::drawFace(){
	if(tracker.getFound()) {
		ofPushMatrix();
		ofTranslate(-mFaceFeatures.cropArea.x,-mFaceFeatures.cropArea.y);

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
		ofVertices(mFaceFeatures.face.getVertices());
		ofEndShape();
		
		// FOREHEAD
		ofSetColor(currentColorScheme.face);
		ofPushMatrix();
		ofTranslate(0.29*mFaceFeatures.faceTrans*mFaceFeatures.face.getBoundingBox().height);
		ofBeginShape();
		ofVertices(mFaceFeatures.face.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		// MOUTH
		ofSetColor(currentColorScheme.mouth);
		ofBeginShape();
		ofVertices(mFaceFeatures.outMouth.getVertices());
		ofEndShape();
		ofSetColor(currentColorScheme.teeth);
		ofBeginShape();
		ofVertices(mFaceFeatures.inMouth.getVertices());
		ofEndShape();
		
		// RIGHT EYE LID
		ofPushMatrix();
		ofSetColor(currentColorScheme.eyelid);
		ofTranslate(-mFaceFeatures.rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(mFaceFeatures.rightEye.getVertices());
		ofEndShape();
		ofSetColor(currentColorScheme.face);
		ofTranslate(mFaceFeatures.rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(mFaceFeatures.rightEye.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		// LEFT EYE LID
		ofPushMatrix();
		ofSetColor(currentColorScheme.eyelid);
		ofTranslate(-mFaceFeatures.leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(mFaceFeatures.leftEye.getVertices());
		ofEndShape();
		ofSetColor(currentColorScheme.face);
		ofTranslate(mFaceFeatures.leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(mFaceFeatures.leftEye.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		// RIGHT EYE
		ofSetColor(currentColorScheme.eye);
		ofCircle(mFaceFeatures.rightEye.getCentroid2D(), 0.5*min(mFaceFeatures.rightEye.getBoundingBox().getWidth(),mFaceFeatures.rightEye.getBoundingBox().getHeight()));
		
		// LEFT EYE
		ofSetColor(currentColorScheme.eye);
		ofCircle(mFaceFeatures.leftEye.getCentroid2D(), 0.5*min(mFaceFeatures.leftEye.getBoundingBox().getWidth(),mFaceFeatures.leftEye.getBoundingBox().getHeight()));
		
		// PRINT
		ofSetColor(currentColorScheme.print);
		printLayer.draw(0,0);
		ofPopMatrix();
	}
	else{
		ofClear(0);
	}
}

//--------------------------------------------------------------
void WarholFaceApp::draw(){
	ofBackground(0);
	ofSetColor(255);

	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, cam.height+20);

	// create 12
	for(int i=0; i<12; ++i){
		currentColorScheme = ColorScheme::getScheme(i);
		faceFbos[i].begin();
		drawFace();
		faceFbos[i].end();
	}

	// can draw more than 12
	for(int i=0;i<12;++i){
		ofPushMatrix();
		ofTranslate((i%6)*mFaceFeatures.cropArea.width,(i/6)*mFaceFeatures.cropArea.height);
		ofSetColor(255);
		faceFbos[i].getTextureReference().drawSubsection(0, 0, mFaceFeatures.cropArea.width, mFaceFeatures.cropArea.height, 1,1);
		ofPopMatrix();
	}
}

void FaceFeatures::blowUpPolyline(ofPolyline &pl){
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
		op[i*4+0] = op[i*4+1] = op[i*4+2] = (gray < thresholdValue)?255:0;
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