#pragma once

#include "ofxMathSurface.h"

// create meshs of the form r = r(u,v)
class ofxParametricSurface:public ofxMathSurface{
public:
    virtual ~ofxParametricSurface(){}
    ofxParametricSurface(){isSetup = false;flatColors = false;}
    
    // override these functions to configure your mesh
    // valueForPoint is mandatory, the rest are optional
    // valueForPoint should return a 3dpoint for a given u,v coordinate
    // override backColor and/or backTexCoord if you are using the 2 sided draw function;
    virtual ofPoint valueForPoint(float u,float v) = 0;
    virtual ofFloatColor colorForPoint(float u, float v, ofPoint value){return ofFloatColor::white;}
    virtual ofVec2f texCoordForPoint(float u,float v,ofPoint value){return ofVec2f::zero();}
    virtual ofFloatColor backColorForPoint(float u, float v, ofPoint value){return ofFloatColor::white;}
    virtual ofVec2f backTexCoordForPoint(float u,float v, ofPoint value){return ofVec2f::zero();}
    
    // retrives normal at point see http://mathworld.wolfram.com/NormalVector.html
    // really no need to override this
    virtual ofVec3f normalForPoint(float u,float v,ofPoint value);
    
    void setup(float uMin,float uMax,float vMin,float vMax,float uStep,float vStep);
    
    void reload();
    
    void setUBounds(float uMin,float uMax);
    void setVBounds(float vMin,float vMax);
    
    void setUMin(float uMin);
    void setUMax(float uMax);
    void setVMin(float vMin);
    void setVMax(float vMax);
    
    float getUMin()const{return uMin;}
    float getUMax()const{return uMax;}
    float getVMin()const{return vMin;}
    float getVMax()const{return vMax;}
    float getAbsUMin()const{return absUMin;}
    float getAbsVMin()const{return absVMin;}
    float getAbsUMax()const{return absUMax;}
    float getAbsVMax()const{return absVMax;}
    
    float getUStep()const{return uStep;}
    float getVStep()const{return vStep;}
    
    
    
private:
    bool isSetup;
    float absUMin;
    float absUMax;
    float absVMin;
    float absVMax;
    float uMin;
    float uMax;
    float vMin;
    float vMax;
    float uStep;
    float vStep;
    vector<float> uDomainPoints;
    vector<float> vDomainPoints;

private:
    struct ParametricPosition{
        ParametricPosition(const float &u,const float &v, const ofPoint &value){
            this->u = u;
            this->v = v;
            this->value = value;
        }
        float u;
        float v;
        ofPoint value;
    };
    void addQuad(const ParametricPosition &one,const ParametricPosition &two,const ParametricPosition &three,const ParametricPosition &four);
    void loadDomainPoints();
    void setUStep(float step_);
    void setVStep(float step_);
};

