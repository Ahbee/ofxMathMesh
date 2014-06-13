#pragma once
#include "ofxMathCurve.h"

class ofxParametricCurve:public ofxMathCurve {
public:
    virtual ~ofxParametricCurve(){}
    ofxParametricCurve(){isSetup= false;}
    // override these functions to configure your function
    // valueForPoint is mandatory, the rest are optional
    // valueForPoint should return a 3dPoint for a given t value
    virtual ofPoint valueForPoint(float t) = 0;
    virtual ofFloatColor colorForPoint(float t, ofPoint value){return ofFloatColor::white;}
    
    // setups the function. You must call this before anything else.
    // You want to set tMin and tMax to the largest possible range (tMax-tMin)
    // that will be displayed on screen. Step defines the resoultion of your function.
    // Smaller Values give better Resolution.
    void setup(float tMin,float tMax,float step);
    
     // reloads data. Whenever you make a change to the equation or bounds call reload.
    void reload();
    void setTBounds(float tMin,float tMax);
    void setTMin(float tMin);
    void setTMax(float tMax);
    
    float getTMin()const{return tMin;}
    float getTMax()const{return tMax;}
    
    float getStep()const {return step;}
    
private:
    bool isSetup;
    float absTMin;
    float absTMax;
    float tMin;
    float tMax;
    float step;
    vector<float> domainPoints;
    
private:
    struct ParametricPosition{
        ParametricPosition(const float &t, const ofPoint &value){
            this->t = t;
            this->value = value;
        }
        float t;
        ofPoint value;
    };
    void addLineSeg(const ParametricPosition &one, const ParametricPosition &two);
    void loadDomainPoints();
    void setStep(float step_);
};