#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setPosition(0, 0, 7);
    cam.setNearClip(.001);
    cam.setAutoDistance(false);
    ofSetLineWidth(3.0);
    butterfly.setup(0, 6 * M_PI, .01);
    
    tMax.setCurve(EASE_OUT);
    tMax.setRepeatType(LOOP);
    tMax.animateFromTo(0, 6 * M_PI);
    tMax.setDuration(20.0);
}

//--------------------------------------------------------------
void ofApp::update(){
    float dt = 1/ofGetFrameRate();
    tMax.update(dt);
    butterfly.setTMax(tMax);
    butterfly.reload();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    cam.begin();
    ofSetColor(255, 0, 0);
    butterfly.draw(false);
    cam.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
