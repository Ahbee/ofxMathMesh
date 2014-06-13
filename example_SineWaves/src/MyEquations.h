#pragma once

#include "ofxMathMesh.h"
#include "ofxAnimatableFloat.h"

#define SINEMIN -4*M_PI
#define SINEMAX 4*M_PI

class SineWaveParent:public ofx2dFunction{
public:
    SineWaveParent():ofx2dFunction(){
        orangeStart.setCurve(LINEAR);
        orangeStart.setRepeatType(LOOP);
        orangeStart.animateFromTo(SINEMIN, SINEMAX);
        orangeStart.setDuration(3.0);
        
        amplitude.setCurve(LINEAR);
        amplitude.setRepeatType(LOOP_BACK_AND_FORTH);
        amplitude.animateFromTo(-3.0, 3.0);
        amplitude.setDuration(3.0);
        
        ofAddListener(ofEvents().update, this, &SineWaveParent::update);
    }
    
    float valueForPoint(float x){
        return amplitude.getCurrentValue() * sin(x);
    }
    
    ofFloatColor colorForPoint(float x,float y){
        float colorRange = (SINEMAX - SINEMIN)/3.0;
        float fullRange = SINEMAX - SINEMIN;
        float orangeBegin = orangeStart.getCurrentValue();
        float purpleBegin = orangeBegin + colorRange;
        float yellowBegin = purpleBegin + colorRange;
        float xWrapped = ofWrap(x, orangeBegin, orangeBegin + fullRange);
        if (xWrapped >= orangeBegin && xWrapped < purpleBegin) {
            return ofFloatColor::orange;
        }else if (xWrapped >=purpleBegin && xWrapped < yellowBegin){
            return ofFloatColor::purple;
        }else{
            return ofFloatColor::yellow;
        }
        
        
    }
    
    void update(ofEventArgs &args){
        float dt = 1.0f/ofGetFrameRate();
        amplitude.update(dt);
        orangeStart.update(dt);
    }
    
public:
    ofxAnimatableFloat orangeStart;
    ofxAnimatableFloat amplitude;
};

class SineWaveChild:public ofx2dFunction {
public:
    
    ~SineWaveChild(){
        ofRemoveListener(ofEvents().update, this ,&SineWaveChild::update);
    }
    
    void set(int ID,float amplitude,float orangeStart){
        this->ID = ID;
        this->amplitude = amplitude;
        this->orangeStart = orangeStart;
        alpha.setCurve(LINEAR);
        alpha.animateFromTo(1.0, 0.0);
        alpha.setDuration(1.0);
        ofAddListener(ofEvents().update, this, &SineWaveChild::update);
    }
    
    float valueForPoint(float x){
        return amplitude * sin(x);
        
    }
    
    ofFloatColor colorForPoint(float x,float y){
        float colorRange = (SINEMAX - SINEMIN)/3.0;
        float fullRange = SINEMAX - SINEMIN;
        float orangeBegin = orangeStart;
        float purpleBegin = orangeBegin + colorRange;
        float yellowBegin = purpleBegin + colorRange;
        float xWrapped = ofWrap(x, orangeBegin, orangeBegin + fullRange);
        ofFloatColor returnColor;
        
        if (xWrapped >= orangeBegin && xWrapped < purpleBegin) {
            returnColor = ofFloatColor::orange;
        }else if (xWrapped >=purpleBegin && xWrapped < yellowBegin){
            returnColor = ofFloatColor::purple;
        }else{
            returnColor = ofFloatColor::yellow;
        }
        returnColor.a = alpha;
        return returnColor;
    }
    
    void update(ofEventArgs &args){
        float dt = 1.0f/ofGetFrameRate();
        alpha.update(dt);
        if (!alpha.isAnimating()) {
            ofNotifyEvent(endE, ID, this);
        }
    }
    
public:
    int ID;
    ofEvent<int> endE;
    float percent;
    float amplitude;
    float orangeStart;
    ofxAnimatableFloat alpha;
};
