#pragma once

#include "ofMain.h"
#include "ofxMathCurve.h"

// creates meshes of the form y = f(x)
class ofx2dFunction:public ofxMathCurve {
public:
    virtual ~ofx2dFunction(){}
    ofx2dFunction(){bUseYBounds = false; isSetup= false;}
    
    // override these functions to configure your function
    // valueForPoint is mandatory, the rest are optional
    // x is the domain and y is the range
    virtual float valueForPoint(float x) = 0;
    virtual ofFloatColor colorForPoint(float x,float y) {return ofFloatColor::white;}
    
    // setups the function. You must call this before anything else.
    // You want to set xMin and xMax to the largest possible range (xMax-xMin)
    // that will be displayed on screen. Step defines the resoultion of your function.
    // Smaller Values give better Resolution.
    void setup(float xMin,float xMax,float step);
    void setup(float xMin,float xMax,float yMin,float yMax,float step);
    
    // reloads data. Whenever you make a change to the equation or bounds call reload.
    void reload();
    
    void setXBounds(float xMin,float xMax);
    void setYBounds(float yMin,float yMax);
    void setXMin(float xMin);
    void setXMax(float xMax);
    void setYMin(float yMin){this->yMin = yMin;}
    void setYMax(float yMax){this->yMax = yMax;}
    
    void setUseYBounds(bool useYBounds){bUseYBounds = useYBounds;}
    
    
    float getXmin()const{return xMin;}
    float getXMax()const{return xMax;}
    float getYMin()const{return yMin;}
    float getYMax()const{return yMax;}
    
    float getStep()const{return step;}
    
private:
    void addLineSeg(const ofVec2f &one, const ofVec2f &two);
    void addLineSegClip(const ofVec2f &one, const ofVec2f &two);
    void setStep(float step);
    void loadDomainPoints();
    
    vector<float> domainPoints;
    bool isSetup;
    float step;
    float xMin;
    float xMax;
    float yMin;
    float yMax;
    float absXMin;
    float absXMax;
    float bUseYBounds;
};

