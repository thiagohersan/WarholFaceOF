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
	
	ColorScheme temp;

	temp.background = ofColor(90,130,110);
	temp.print = ofColor(0,0,0);
	temp.hair = ofColor(230,190,65);
	temp.face = ofColor(235,170,155);
	temp.mouth = ofColor(220,50,30);
	temp.teeth = ofColor(250,245,225);
	temp.eyelid = ofColor(175,180,185);
	temp.eye = ofColor(95,145,125);
	theColors.push_back(temp);

	temp.background = ofColor(205,75,100);
	temp.print = ofColor(130,40,85);
	temp.hair = ofColor(145,175,75);
	temp.face = ofColor(240,210,55);
	temp.mouth = ofColor(110,45,70);
	temp.teeth = ofColor(145,175,75);
	temp.eyelid = ofColor(210,70,90);
	temp.eye = ofColor(155,160,55);
	theColors.push_back(temp);

	temp.background = ofColor(215,100,70);
	temp.print = ofColor(20,15,20);
	temp.hair = ofColor(220,185,55);
	temp.face = ofColor(195,155,165);
	temp.mouth = ofColor(90,25,50);
	temp.teeth = ofColor(205,200,205);
	temp.eyelid = ofColor(50,95,65);
	temp.eye = ofColor(95,115,105);
	theColors.push_back(temp);

	temp.background = ofColor(0,180,215);
	temp.print = ofColor(80,80,85);
	temp.hair = ofColor(245,130,35);
	temp.face = ofColor(250,185,215);
	temp.mouth = ofColor(80,85,90);
	temp.teeth = ofColor(185,210,235);
	temp.eyelid = ofColor(0,180,220);
	temp.eye = ofColor(250,160,50);
	theColors.push_back(temp);

	temp.background = ofColor(65,130,175);
	temp.print = ofColor(125,100,110);
	temp.hair = ofColor(220,70,5);
	temp.face = ofColor(215,130,145);
	temp.mouth = ofColor(90,100,125);
	temp.teeth = ofColor(95,180,210);
	temp.eyelid = ofColor(80,130,150);
	temp.eye = ofColor(165,70,85);
	theColors.push_back(temp);

	temp.background = ofColor(215,140,160);
	temp.print = ofColor(65,95,25);
	temp.hair = ofColor(230,130,35);
	temp.face = ofColor(130,180,205);
	temp.mouth = ofColor(125,65,65);
	temp.teeth = ofColor(220,60,120);
	temp.eyelid = ofColor(220,155,150);
	temp.eye = ofColor(75,80,45);
	theColors.push_back(temp);

	temp.background = ofColor(215,40,90);
	temp.print = ofColor(45,60,120);
	temp.hair = ofColor(210,150,130);
	temp.face = ofColor(215,55,15);
	temp.mouth = ofColor(45,60,80);
	temp.teeth = ofColor(215,155,0);
	temp.eyelid = ofColor(210,45,85);
	temp.eye = ofColor(190,115,115);
	theColors.push_back(temp);

	temp.background = ofColor(180,205,105);
	temp.print = ofColor(165,30,20);
	temp.hair = ofColor(245,195,40);
	temp.face = ofColor(125,170,65);
	temp.mouth = ofColor(210,40,60);
	temp.teeth = ofColor(205,65,110);
	temp.eyelid = ofColor(250,200,115);
	temp.eye = ofColor(200,165,90);
	theColors.push_back(temp);

	// http://www.webexhibits.org/colorart/marilyns.html
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
		ofSetColor(theColors[0].background);
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
			ofSetColor(theColors[0].hair);
			ofPushMatrix();
			ofTranslate(face.getBoundingBox().x-face.getBoundingBox().width/2,0);
            ofBeginShape();
			ofVertices(contourFinder.blobs[0].pts);
            ofEndShape(true);
			ofPopMatrix();
		}
				
		// FACE
		ofSetColor(theColors[0].face);
		ofBeginShape();
		ofVertices(face.getVertices());
		ofEndShape();
		
		// FOREHEAD
		ofSetColor(theColors[0].face);
		ofVec2f faceTrans = tracker.getImageFeature(ofxFaceTracker::NOSE_BASE).getCentroid2D()-inMouth.getCentroid2D();
		faceTrans.normalize();
		ofPushMatrix();
		ofTranslate(faceTrans*face.getBoundingBox().height/3);
		ofBeginShape();
		ofVertices(face.getVertices());
		ofEndShape();
		ofPopMatrix();

		// MOUTH
		ofSetColor(theColors[0].mouth);
		ofBeginShape();
		ofVertices(outMouth.getVertices());
		ofEndShape();
		ofSetColor(theColors[0].teeth);
		ofBeginShape();
		ofVertices(inMouth.getVertices());
		ofEndShape();

		// RIGHT EYE LID
		ofPushMatrix();
		ofSetColor(theColors[0].eyelid);
		ofTranslate(-rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(rightEye.getVertices());
		ofEndShape();
		ofSetColor(theColors[0].face);
		ofTranslate(rightEyeTrans*0.3);
		ofBeginShape();
		ofVertices(rightEye.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		// LEFT EYE LID
		ofPushMatrix();
		ofSetColor(theColors[0].eyelid);
		ofTranslate(-leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(leftEye.getVertices());
		ofEndShape();
		ofSetColor(theColors[0].face);
		ofTranslate(leftEyeTrans*0.3);
		ofBeginShape();
		ofVertices(leftEye.getVertices());
		ofEndShape();
		ofPopMatrix();
		
		// TODO:
		// RIGHT EYE
		// LEFT EYE
		
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