
#include "ofxParametricCurve.h"

void ofxParametricCurve::setTMin(float tMin_){
    if (tMin_ >= absTMin) {
        tMin = tMin_;
    }else{
        tMin = absTMin;
        ofLogWarning("ofxMathMesh") << "cannot set tMin smaller than setup tMin";
    }
}

void ofxParametricCurve::setTMax(float tMax_){
    if (tMax_ <= absTMax) {
        tMax = tMax_;
    }else{
        tMax = absTMax;
        ofLogWarning("ofxMathMesh") << "cannot set tMax bigger than setup tMax;";
    }
}

void ofxParametricCurve::setStep(float step_){
    float range = absTMax - absTMin;
    if (step_ > range) {
        ofLogWarning("ofxMathMesh") << "cannot set step greater than range";
        step = range;
        return;
    }
    int res = round(range/step_);
    step = range/(float)res;
}

void ofxParametricCurve::loadDomainPoints(){
    float epsilon = step/100.0;
    for (float t = absTMin; t <= absTMax + epsilon; t+=step) {
        domainPoints.push_back(t);
    }
}

void ofxParametricCurve::setTBounds(float tMin_, float tMax_){
    setTMin(tMin_);
    setTMax(tMax_);
}

void ofxParametricCurve::setup(float tMin_, float tMax_, float step_){
    if (step_ <= 0) {
        ofLogWarning("ofxMathMesh") << "step must be greater than zero";
        return;
    }
    isSetup = true;
    absTMin = tMin_;
    absTMax = tMax_;
    setStep(step_);
    setTBounds(tMin_, tMax_);
    loadDomainPoints();
    reload();
}

void ofxParametricCurve::reload(){
    if (!isSetup) {
        ofLogError("ofxMathMesh") << "cannot reload if the function is not setup";
        return;
    }
    clear();
    int tMinDomainPoint = round(ofMap(tMin, absTMin, absTMax, 0, domainPoints.size() - 1));
    int tMaxDomainPoint = round(ofMap(tMax, absTMin, absTMax, 0, domainPoints.size()-1));
    for (int t = tMinDomainPoint; t < tMaxDomainPoint; t++) {
        ParametricPosition one(domainPoints[t],valueForPoint(domainPoints[t]));
        ParametricPosition two(domainPoints[t+1],valueForPoint(domainPoints[t+1]));
        addLineSeg(one, two);
    }
    
}

void ofxParametricCurve::addLineSeg(const ofxParametricCurve::ParametricPosition &one, const ofxParametricCurve::ParametricPosition &two){
    vertices.push_back(one.value);
    vertices.push_back(two.value);
    ofFloatColor color1 = colorForPoint(one.t, one.value);
    ofFloatColor color2 = colorForPoint(two.t, one.value);
    colors.push_back(color1);
    colors.push_back(color2);
}


