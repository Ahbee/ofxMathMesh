#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setAutoDistance(false);
    cam.setDistance(12);
    cam.setNearClip(0.01);
    cam.setTarget(ofVec3f::zero());
    cam.lookAt(ofVec3f::zero());
    ofSetLineWidth(5.0);
    ofBackground(0);
    
    gui.setup();
    gui.add(drawChildren.setup("DrawChildren",true));
    sineWave.setup(SINEMIN,SINEMAX,.1);

}

//--------------------------------------------------------------
void ofApp::update(){
    static int ID = 0;
    for (int i = 0; i < children.size(); i++) {
        children[i]->reload();
    }
    sineWave.reload();
    shared_ptr<SineWaveChild> child(new SineWaveChild);
    child->setup(SINEMIN, SINEMAX, .1);
    child->set(ID, sineWave.amplitude,sineWave.orangeStart);
    ofAddListener(child->endE, this, &ofApp::childDied);
    children.push_back(child);
    ID++;
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    sineWave.draw(true);
    if (drawChildren) {
        for (int i = 0; i < children.size(); i++) {
            children[i]->draw(true);
        }
    }
    cam.end();
    gui.draw();
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

void ofApp::childDied(int &ID){
    int indexToRemove;
    for (int i = 0; i < children.size(); i++) {
        if (children[i]->ID == ID) {
            indexToRemove = i;
        }
    }
    children.erase(children.begin() + indexToRemove);
}
