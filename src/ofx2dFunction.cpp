
#include "ofx2dFunction.h"


void ofx2dFunction::setXMax(float xMax_){
    if (xMax_ <= absXMax) {
        xMax = xMax_;
    }else{
        xMax = absXMax;
        ofLogWarning("ofxMathMesh") << "cannot set xMax greater than setup xMax";
    }
}

void ofx2dFunction::setXMin(float xMin_){
    if (xMin_ >= absXMin) {
        xMin = xMin_;
    }else{
        xMin = absXMin;
        ofLogWarning("ofxMathMesh") << "cannot set xMin smaller than setup xMin";
    }
}

void ofx2dFunction::setXBounds(float xMin_, float xMax_){
    setXMin(xMin_);
    setXMax(xMax_);
}

void ofx2dFunction::setYBounds(float yMin, float yMax){
    setUseYBounds(true);
    this->yMin = yMin;
    this->yMax = yMax;
}

void ofx2dFunction::loadDomainPoints(){
    float epsilon = step/100.0;
    for (float x = absXMin; x <= absXMax + epsilon; x+=step) {
        domainPoints.push_back(x);
    }
}

void ofx2dFunction::setStep(float step_){
    float range = absXMax - absXMin;
    if (step_ > range) {
        ofLogWarning("ofxMathMesh") << "cannot set step greater than range";
        step = range;
        return;
    }
    int res = round(range/step_);
    step = range/(float)res;
}

void ofx2dFunction::setup(float xMin_, float xMax_, float step_){
    if (step_ <= 0) {
        ofLogWarning("ofxMathMesh") << "step must be greater than zero";
        return;
    }
    isSetup = true;
    absXMax = xMax_;
    absXMin = xMin_;
    setStep(step_);
    setXBounds(xMin_, xMax_);
    loadDomainPoints();
    reload();
}

void ofx2dFunction::setup(float xMin_, float xMax_, float yMin_, float yMax_, float step_){
    setYBounds(yMin_, yMax_);
    setup(xMin_, xMax_, step_);
}

void ofx2dFunction::reload(){
    if (!isSetup) {
        ofLogError("ofxMathMesh") << "cannot reload if the function is not setup";
        return;
    }
    clear();
    int xMinDomainPoint = round(ofMap(xMin, absXMin, absXMax, 0, domainPoints.size() - 1));
    int xMaxDomainPoint = round(ofMap(xMax, absXMin, absXMax, 0, domainPoints.size()-1));
    for (int i = xMinDomainPoint; i < xMaxDomainPoint; i++){
        ofVec2f one = ofVec2f(domainPoints[i],valueForPoint(domainPoints[i]));
        ofVec2f two = ofVec2f(domainPoints[i+1],valueForPoint(domainPoints[i+1]));
        if (bUseYBounds) {
            addLineSegClip(one,two);
        }else{
            addLineSeg(one, two);
        }
    }
}

void ofx2dFunction::addLineSeg(const ofVec2f &one, const ofVec2f &two){
    vertices.push_back(one);
    vertices.push_back(two);
    
    ofFloatColor color1 = colorForPoint(one.x, one.y);
    ofFloatColor color2 = colorForPoint(two.x, two.y);
    
    colors.push_back(color1);
    colors.push_back(color2);
}


void ofx2dFunction::addLineSegClip(const ofVec2f &one, const ofVec2f &two){
    ofMesh tempMesh;
    ofPoint yMaxLine[2];
    ofPoint intersection;
    yMaxLine[0] = ofVec2f(xMin-10,yMax);
    yMaxLine[1] = ofVec2f(xMax+10, yMax);
    
    if (one.y > yMax && two.y < yMax) {
        ofLineSegmentIntersection(one, two, yMaxLine[0], yMaxLine[1], intersection);
        tempMesh.addVertex(intersection);
        tempMesh.addVertex(two);
    }else if (one.y < yMax && two.y > yMax ){
        ofLineSegmentIntersection(one, two, yMaxLine[0], yMaxLine[1], intersection);
        tempMesh.addVertex(one);
        tempMesh.addVertex(intersection);
    }else if (one.y <= yMax && two.y <=yMax){
        tempMesh.addVertex(one);
        tempMesh.addVertex(two);
    }else if (one.y > yMax && two.y > yMax){
        
    }
    ofPoint yMinLine[2];
    yMinLine[0] = ofVec2f(xMin-10,yMin);
    yMinLine[1] = ofVec2f(xMax+10,yMin);
    for (int i = 0 ; i < tempMesh.getVertices().size(); i+=2) {
        ofVec2f a = tempMesh.getVertices()[i];
        ofVec2f b = tempMesh.getVertices()[i+1];
        if (a.y < yMin && b.y > yMin) {
            ofLineSegmentIntersection(a, b, yMinLine[0], yMinLine[1], intersection);
            addLineSeg(intersection, b);
        }else if (a.y > yMin && b.y < yMin){
            ofLineSegmentIntersection(a, b, yMinLine[0], yMinLine[1], intersection);
            addLineSeg(a, intersection);
        }else if (a.y >= yMin && b.y >=yMin){
            addLineSeg(a, b);
        }else if (a.y < yMin && b.y < yMin){
            
        }
    }
}


