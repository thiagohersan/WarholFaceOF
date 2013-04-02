#include "testApp.h"

using namespace ofxCv;

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	cam.initGrabber(640, 480);
	
	thresholded.allocate(cam.width,cam.height, OF_IMAGE_COLOR_ALPHA);
	thresholdValue = 40;
	
	grayDiff.allocate(thresholded.width,thresholded.height);
	cImg.allocate(thresholded.width,thresholded.height);
	
	tracker.setup();
	tracker.setRescale(.5);
}

//--------------------------------------------------------------
void testApp::update(){
	cam.update();
	if(cam.isFrameNew()) {
		// face detection
		tracker.update(toCv(cam));
		position = tracker.getPosition();
		scale = tracker.getScale();
		orientation = tracker.getOrientation();
		rotationMatrix = tracker.getRotationMatrix();
		
		thresholdCam(cam,thresholded);
		
		cImg.setFromPixels(cam.getPixelsRef());
		grayDiff.setROI(0,0, cam.width,cam.height);
		grayDiff = cImg;
		grayDiff.threshold(80,true);
	}
}

//--------------------------------------------------------------
void testApp::draw(){
	// draw regular cam
	ofBackground(0);
	ofSetColor(255);
	//cam.draw(0, 0);
	//grayDiff.draw(0,0);

	ofSetColor(255,255,0);
	ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, cam.height+20);

	if(tracker.getFound()) {
		//ofSetupScreenOrtho(640, 480, OF_ORIENTATION_UNKNOWN, true, -1000, 1000);

		// draw warhol test
		ofPushMatrix();
		ofTranslate(cam.width/2,0);
		ofSetColor(70,190,190);
		ofRect(0,0, thresholded.width,thresholded.height);
		ofPolyline face = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE);
		ofPolyline outMouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH);
		ofPolyline inMouth = tracker.getImageFeature(ofxFaceTracker::INNER_MOUTH);
		ofPolyline rightEyebrow = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYEBROW);
		ofPolyline leftEyebrow = tracker.getImageFeature(ofxFaceTracker::LEFT_EYEBROW);
		ofPolyline rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE);
		ofPolyline leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE);
		
		blowUpPolyline(rightEye);
		blowUpPolyline(leftEye);
		
		ofVec2f rightEyeTrans = rightEye.getBoundingBox().getCenter()-rightEyebrow.getBoundingBox().getCenter();
		ofVec2f leftEyeTrans = leftEye.getBoundingBox().getCenter()-leftEyebrow.getBoundingBox().getCenter();
		
		// find head outline
		grayDiff.setROI(face.getBoundingBox().x-face.getBoundingBox().width/2,0,
						face.getBoundingBox().width*2,face.getBoundingBox().y+face.getBoundingBox().height);
		contourFinder.findContours(grayDiff, 500, cam.width*cam.height/2, 1, true);
		
		// HAIR
		if(contourFinder.nBlobs){
			ofSetColor(255,255,0);
			ofPushMatrix();
			ofTranslate(face.getBoundingBox().x-face.getBoundingBox().width/2,0);
            ofBeginShape();
			ofVertices(contourFinder.blobs[0].pts);
            ofEndShape(true);
			ofPopMatrix();
		}
				
		// FACE
		ofSetColor(255,200,200);
		ofBeginShape();
		ofVertices(face.getVertices());
		ofEndShape();
		
		// FOREHEAD
		ofSetColor(255,200,200);
		ofVec2f faceTrans = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE).getCentroid2D()-inMouth.getCentroid2D();
		faceTrans.normalize();
		ofPushMatrix();
		ofTranslate(faceTrans*face.getBoundingBox().height/3);
		ofBeginShape();
		ofVertices(face.getVertices());
		ofEndShape();
		ofPopMatrix();

		// MOUTH
		ofSetColor(255,10,10);
		ofBeginShape();
		ofVertices(outMouth.getVertices());
		ofEndShape();
		ofSetColor(255,255,255);
		ofBeginShape();
		ofVertices(inMouth.getVertices());
		ofEndShape();

		// RIGHT EYE LID
		ofPushMatrix();
		ofSetColor(150,128,150);
		ofTranslate(-rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(rightEye.getVertices());
		ofEndShape();
		ofSetColor(255,200,200);
		ofTranslate(rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(rightEye.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		// LEFT EYE LID
		ofPushMatrix();
		ofSetColor(150,128,150);
		ofTranslate(-leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(leftEye.getVertices());
		ofEndShape();
		ofSetColor(255,200,200);
		ofTranslate(leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(leftEye.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		thresholded.draw(0,0);
		ofPopMatrix();

		//ofMesh faceMesh = tracker.getObjectMesh();
		//ofTranslate(position.x,position.y-240);
		//applyMatrix(rotationMatrix);
		//ofScale(scale,scale,scale);
		//faceMesh.draw();
		//ofSetColor(0,255,0);
		//faceMesh.drawWireframe();
		
		/*
		 addMessage("/gesture/mouth/width", tracker.getGesture(ofxFaceTracker::MOUTH_WIDTH));
		 addMessage("/gesture/mouth/height", tracker.getGesture(ofxFaceTracker::MOUTH_HEIGHT));
		 addMessage("/gesture/eyebrow/left", tracker.getGesture(ofxFaceTracker::LEFT_EYEBROW_HEIGHT));
		 addMessage("/gesture/eyebrow/right", tracker.getGesture(ofxFaceTracker::RIGHT_EYEBROW_HEIGHT));
		 addMessage("/gesture/eye/left", tracker.getGesture(ofxFaceTracker::LEFT_EYE_OPENNESS));
		 addMessage("/gesture/eye/right", tracker.getGesture(ofxFaceTracker::RIGHT_EYE_OPENNESS));
		 addMessage("/gesture/jaw", tracker.getGesture(ofxFaceTracker::JAW_OPENNESS));
		 addMessage("/gesture/nostrils", tracker.getGesture(ofxFaceTracker::NOSTRIL_FLARE));
		 */
	}
}

void testApp::blowUpPolyline(ofPolyline &pl){
	ofPoint center = pl.getBoundingBox().getCenter();
	for(int i=0; i<pl.size(); ++i){
		pl[i] += pl[i]-center;
	}
}

// destructive: changes incoming img
void testApp::thresholdCam(ofVideoGrabber &in, ofImage &out){
	unsigned char *op = out.getPixels();
	unsigned char *ip = in.getPixels();
	for(int i=0; i<in.height*in.width; ++i){
		float gray = 0.2989*float(ip[i*3+0]) + 0.5870*float(ip[i*3+1]) + 0.1140*float(ip[i*3+2]);
		op[i*4+0] = op[i*4+1] = op[i*4+2] = (gray < thresholdValue)?0:255;
		op[i*4+3] = (gray < thresholdValue)?255:0;
	}
	out.update();
}



//--------------------------------------------------------------
void testApp::keyPressed(int key){
	
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
	tracker.reset();
	if(key == '-' || key == '_'){
		thresholdValue = (thresholdValue>0)?(thresholdValue-1):thresholdValue;
	}
	if(key == '+' || key == '='){
		thresholdValue = (thresholdValue<255)?(thresholdValue+1):thresholdValue;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
	
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
	
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
	
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){
	
}