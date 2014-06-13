#pragma once

#include "ofxMathMesh.h"
#include "ofxAnimatableFloat.h"
#include "MyEquations.h"

class Parabala:public ofx3dFunction {
public:
    Parabala():ofx3dFunction(){
        a.setCurve(EASE_IN_EASE_OUT);
        a.setRepeatType(LOOP_BACK_AND_FORTH);
        a.setDuration(3.0);
        a.animateFromTo(-.4, .4);
        ofAddListener(ofEvents().update, this, &Parabala::update);
    }
    
    float valueForPoint(float x,float z){
        return a.getCurrentValue() * pow(x, 2) + a.getCurrentValue() * pow(z, 2);
    }
    
    ofFloatColor colorForPoint(float x, float z,float y){
        float maxY = valueForPoint(getXMax(), getZMax());
        ofFloatColor color;
        float hueValue = ofMap(y, 0, maxY, 0, ofFloatColor::limit());
        color.setHsb(hueValue, 1.0, 1.0);
        return color;
    }
    
    void update(ofEventArgs &args){
        float dt = 1/ofGetFrameRate();
        a.update(dt);
    }

public:
    ofxAnimatableFloat a;
};


class Torus:public ofxParametricSurface{
public:
    Torus():ofxParametricSurface(){
        minU.setCurve(EASE_OUT);
        minU.setRepeatType(LOOP_BACK_AND_FORTH);
        minU.setDuration(4.0);
        minU.animateFromTo(2*M_PI,0);
        
        minV.setCurve(LINEAR);
        minV.setRepeatType(LOOP_BACK_AND_FORTH);
        minV.setDuration(4.0);
        minV.animateFromTo(2*M_PI,0);
        ofAddListener(ofEvents().update, this, &Torus::update);
    }
    ofPoint valueForPoint(float u,float v){
        float c = 2;
        float a = 1;
        float x = (c+ a* cos(v)) * cos(u);
        float y = a* sin(v);
        float z = (c+ a*cos(v)) *sin(u);
        return ofPoint(x,y,z);
    }
    
    ofVec2f texCoordForPoint(float u,float v,ofPoint value){
        float s = u/(2*M_PI);
        float t = v/(2*M_PI);
        return ofVec2f(s,t);
    }
    
    ofFloatColor backColorForPoint(float u,float v,ofPoint value){
        if (u <= M_PI/2) {
            return ofFloatColor::green;
        }else if(u <= M_PI ){
            return ofFloatColor::orange;
        }else if (u <= 3*M_PI/2){
            return ofFloatColor::pink;
        }else{
            return ofFloatColor::violet;
        }
    }
    
    void update(ofEventArgs &args){
        float dt = 1/ofGetFrameRate();
        minU.update(dt);
        minV.update(dt);
        setUMin(minU);
        setVMin(minV);
    }
    
public:
    ofxAnimatableFloat minU;
    ofxAnimatableFloat minV;
};


class Mobius:public ofxParametricSurface {

public:
    ofPoint valueForPoint(float u,float v){
        float a = 2.0;
        float x = (a + (v/2.0) * cos(u/2.0)) * cos(u);
        float y = (a + (v/2.0) * cos(u/2.0)) * sin(u);
        float z = (v/2.0) * sin(u/2.0);
        return ofPoint(x,y,z);
    }
    
    ofFloatColor colorForPoint(float u, float v, ofPoint value){
        if (u <= M_PI/2) {
            return ofFloatColor::red;
        }else if(u <= M_PI ){
            return ofFloatColor::green;
        }else if (u <= 3*M_PI/2){
            return ofFloatColor::white;
        }else{
            return ofFloatColor::yellow;
        }
    }
};

class Tube:public ofx3dFunction{
public:
    Tube():ofx3dFunction(){
        height.setCurve(LINEAR);
        height.setRepeatType(LOOP_BACK_AND_FORTH);
        height.setDuration(4.0);
        height.animateFromTo(2,4);
        ofAddListener(ofEvents().update, this, &Tube::update);
    }
    
    float valueForPoint(float x,float z){
        return 1.0/(5 * (pow(x, 2.0f) + pow(z, 2.0f)));
    }
    
    void update(ofEventArgs &args){
        float dt = 1/ofGetFrameRate();
        height.update(dt);
        setYMax(height);
    }
public:
    ofxAnimatableFloat height;
    
};

class Spiral:public ofxParametricCurve {
    
public:
    Spiral():ofxParametricCurve(){
        greenStart.setCurve(LINEAR);
        greenStart.setRepeatType(LOOP);
        greenStart.setDuration(40.0);
        greenStart.animateFromTo(0,2*M_PI);
        ofAddListener(ofEvents().update, this, &Spiral::update);
    }
    
    ofPoint valueForPoint(float t){
        float a = 1;
        float r = 1;
        float x = t*cos(6*t);
        float z = t*sin(6*t);
        float y = t;
        return ofPoint(x,y,z);
    }
    
    ofFloatColor colorForPoint(float t, ofPoint value){
        float colorRange = .1;
        float redStart = greenStart.getCurrentValue() + colorRange/2.0;
        float tWrapped = ofWrap(t, greenStart, (float)greenStart + colorRange);
        if (tWrapped >= (float)greenStart && tWrapped < redStart) {
            return ofFloatColor::green;
        }else{
            return ofFloatColor::red;
        }
        
    }
    void update(ofEventArgs &args){
        float dt = 1/ofGetFrameRate();
        greenStart.update(dt);
    }
public:
    ofxAnimatableFloat greenStart;
};








