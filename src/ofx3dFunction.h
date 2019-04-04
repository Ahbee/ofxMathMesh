#pragma once

#include "ofxMathSurface.h"

// creates a mesh for equations of the form y = f(x,z)
class ofx3dFunction:public ofxMathSurface{
public:
    virtual ~ofx3dFunction(){}
    ofx3dFunction(){bUseYBounds = false;isSetup = false;flatColors = false;}
    
    // override these functions to configure your function
    // valueForPoint is mandatory, the rest are optional
    // override backColor and/or backTexCoord if you are using the 2 sided draw function;
    // x and z make up the point in your domain, while y is the corresponding range value
    virtual float valueForPoint(float x,float z) = 0;
    virtual ofFloatColor colorForPoint(float x,float z, float y) {return ofFloatColor::white;}
    virtual glm::vec2 texCoordForPoint(float x,float z ,float y){ return glm::vec2();}
    virtual ofFloatColor backColorForPoint(float x, float z,float y){return ofFloatColor::white;}
    virtual glm::vec2 backTexCoordForPoint(float x,float z, float y){return glm::vec2();}
    
    // retrives normal at point see http://mathworld.wolfram.com/NormalVector.html
    // really no need to override this
    virtual glm::vec3 normalForPoint(float x,float z,float y);
    
    // setups the function. You must call this before anything else.
    // You want to set xMin,xMax,zMin,and zMax to the largest possible ranges (Max-Min)
    // that will be displayed on screen. Step defines the resoultion of your function.
    // Smaller Values give better Resolution.
    void setup(float xMin,float xMax,float zMin,float zMax, float xStep ,float zStep);
    void setup(float xMin,float xMax,float zMin,float zMax,float yMin, float yMax, float xStep, float zStep);
    
    // reloads data. Whenever you make a change to the function you want to call reload
    void reload();
    
    void setXBounds(float xMin,float xMax);
    void setZBounds(float zMin,float zMax);
    void setYBounds(float yMin,float yMax);
    void setUseYBounds(bool useYBounds){bUseYBounds = useYBounds;}
    
    void setXMin(float xMin);
    void setXMax(float xMax);
    void setZMin(float zMin);
    void setZMax(float zMax);
    void setYMin(float yMin){this->yMin = yMin;}
    void setYMax(float yMax){this->yMax = yMax;}
    
    float getXMin()const{return xMin;}
    float getXMax()const{return xMax;}
    float getZMin()const{return zMin;}
    float getZMax()const{return zMax;}
    float getYMin()const{return yMin;}
    float getYMax()const{return yMax;}
    float getAbsXMin()const{return absXMin;}
    float getAbsZMin()const{return absZMin;}
    float getAbsXMax()const{return absXMax;}
    float getAbsZMax()const{return absZMax;}
    
    float getXStep()const{return xStep;}
    float getZStep()const{return zStep;}
    
    bool getUseYBounds()const{return bUseYBounds;}
    
    
private:
    void addQuad(const glm::vec3 &one,const glm::vec3 &two,const glm::vec3 &three,const glm::vec3 &four);
    void addTriangle(const glm::vec3 &one,const glm::vec3 &two,const glm::vec3 &three);
    void addTriangleWithClip(const glm::vec3 &a,const glm::vec3 &b,const glm::vec3 &c);
    void setXStep(float step_);
    void setZStep(float step_);
    void loadDomainPoints();
    
    vector<float> xDomainPoints;
    vector<float> zDomainPoints;
    bool  bUseYBounds;
    bool isSetup;
    float xMin;
    float xMax;
    float zMin;
    float zMax;
    float yMin;
    float yMax;
    float xStep;
    float zStep;
    float absXMin;
    float absXMax;
    float absZMin;
    float absZMax;
};
