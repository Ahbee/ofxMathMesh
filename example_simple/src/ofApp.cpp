#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    cam.setAutoDistance(false);
    cam.setNearClip(.001);
    cam.setTarget(ofVec3f::zero());
    cam.setPosition(0, 0, 10);
    ofDisableArbTex();
    ofSetSmoothLighting(true);
    glPointSize(3.0);
    
    gui.setup();
    gui.add(drawWireFrame.setup("draw wireframe",false));
    gui.add(drawPoints.setup("draw points",false));
    gui.add(drawNoramls.setup("draw normals",false));
    gui.add(lightingOn.setup("Lighting On",false));
    
    currentScene = 0;
    
    info.loadFont("verdana.ttf", 20);
    
    light.setPosition(0, 3, 0);
    light.setScale(.01);
    light.setAttenuation(1.0,.2);
    
    
    torus.enableFlatColors();
    mobius.enableFlatColors();
    
    parabala.setup(-2, 2, -2, 2, .1, .1);
    torus.setup(0, 2*M_PI, 0,2*M_PI, .1, .1);
    mobius.setup(0, 2*M_PI, 0, 1, .1, .1);
    tube.setup(-2, 2, -2, 2,0,2,.1, .1);
    spriral.setup(0, 2*M_PI, .01);
    
    flower.loadImage("flower.jpg");
}

//--------------------------------------------------------------
void ofApp::update(){
    if (currentScene == 0) {
        parabala.reload();
    }
    if (currentScene == 1) {
        torus.reload();
    }
    if (currentScene == 3) {
        tube.reload();
    }
    if (currentScene == 4) {
        spriral.reload();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    ofEnableDepthTest();
    
    cam.begin();
    if (lightingOn) {
        ofEnableLighting();
        light.enable();
    }
    if (currentScene == 0) {
        if (drawNoramls) {
            ofSetColor(ofColor::turquoise);
            parabala.drawNormals(.2);
        }
        if (drawPoints) {
            parabala.drawPoints(true);
        }else{
            parabala.draw(true, false);
            if (drawWireFrame) {
                ofSetColor(ofColor::black);
                glEnable(GL_POLYGON_OFFSET_LINE);
                glPolygonOffset(-1, -1);
                parabala.drawWireFrame(false);
                glDisable(GL_POLYGON_OFFSET_LINE);
            }
        }
    }else if(currentScene == 1){
        if (drawNoramls) {
            ofSetColor(ofColor::turquoise);
            torus.drawNormals(.2);
        }
        if (drawPoints) {
            torus.drawPoints(true);
        }else{
            ofSetColor(255, 255, 255);
            flower.bind();
            torus.drawFrontFaces(false, true);
            flower.unbind();
            torus.drawBackFaces(true, false);
            if (drawWireFrame) {
                ofSetColor(ofColor::black);
                glEnable(GL_POLYGON_OFFSET_LINE);
                glPolygonOffset(-.01, -1);
                torus.drawWireFrame(false);
                glDisable(GL_POLYGON_OFFSET_LINE);
            }
        }
    }else if (currentScene == 2){
        if (drawNoramls) {
            ofSetColor(ofColor::turquoise);
            mobius.drawNormals(.2);
        }
        if (drawPoints) {
            mobius.drawPoints(true);
        }else{
            mobius.draw(true, false);
            if (drawWireFrame) {
                ofSetColor(ofColor::black);
                glEnable(GL_POLYGON_OFFSET_LINE);
                glPolygonOffset(-.01, -1);
                mobius.drawWireFrame(false);
                glDisable(GL_POLYGON_OFFSET_LINE);
            }
        }
    }else if (currentScene == 3){
        if (drawNoramls) {
            ofSetColor(ofColor::turquoise);
            tube.drawNormals(.2);
        }
        if (drawPoints) {
            ofSetColor(ofColor::fireBrick);
            tube.drawPoints(false);
        }else{
            ofSetColor(ofColor::fireBrick);
            tube.draw(false, false);
            if (drawWireFrame) {
                ofSetColor(ofColor::black);
                glEnable(GL_POLYGON_OFFSET_LINE);
                glPolygonOffset(-.01, -1);
                tube.drawWireFrame(false);
                glDisable(GL_POLYGON_OFFSET_LINE);
            }
        }
    }else if (currentScene == 4){
        ofPushMatrix();
        ofTranslate(-1, -4);
        ofSetLineWidth(3.0);
        if (drawPoints) {
            spriral.drawPoints(true);
        }else{
            spriral.draw(true);
        }
        ofSetLineWidth(1.0);
        ofPopMatrix();
    }
    
   if (lightingOn) {
        light.draw();
        light.disable();
        ofDisableLighting();
    }
    
    cam.end();
    
    ofDisableDepthTest();
    gui.draw();
    ofSetColor(ofColor::white);
    info.drawString("press right/left arrows move to next scene",240, 700);
    if (currentScene == 0) {
        info.drawString("Parabala with custom colors", 250, 50);
    }else if (currentScene == 1){
        info.drawString("Torus with texture on front side\nand custom colors on back side", 250, 50);
    }else if (currentScene == 2){
        info.drawString("Mobius Strip with custom colors", 250, 50);
    }else if (currentScene == 3){
        info.drawString("Tube with no custom colors, varying yBounds", 250, 50);
    }else if (currentScene == 4){
        info.drawString("Spiral with custom colors", 250, 50);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == OF_KEY_RIGHT) {
        currentScene++;
        if (currentScene == 5) {
            currentScene = 0;
        }
    }else if(key == OF_KEY_LEFT){
        currentScene--;
        if (currentScene == -1) {
            currentScene = 4;
        }
    }
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


