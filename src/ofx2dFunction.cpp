
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
        glm::vec2 one = glm::vec2(domainPoints[i],valueForPoint(domainPoints[i]));
        glm::vec2 two = glm::vec2(domainPoints[i+1],valueForPoint(domainPoints[i+1]));
        if (bUseYBounds) {
            addLineSegClip(one,two);
        }else{
            addLineSeg(one, two);
        }
    }
}

void ofx2dFunction::addLineSeg(const glm::vec2 &one, const glm::vec2 &two){
    glm::vec3 one2D = glm::vec3(one.x,one.y,0.0f);
    glm::vec3 two2D = glm::vec3(two.x,two.y,0.0f);
    vertices.push_back(one2D);
    vertices.push_back(two2D);
    
    ofFloatColor color1 = colorForPoint(one.x, one.y);
    ofFloatColor color2 = colorForPoint(two.x, two.y);
    
    colors.push_back(color1);
    colors.push_back(color2);
}


void ofx2dFunction::addLineSegClip(const glm::vec2 &one, const glm::vec2 &two){
    ofMesh tempMesh;
    glm::vec2 yMaxLine[2];
    glm::vec2 intersection;
    yMaxLine[0] = glm::vec2(xMin-10,yMax);
    yMaxLine[1] = glm::vec2(xMax+10, yMax);
    
    if (one.y > yMax && two.y < yMax) {
        ofLineSegmentIntersection(one, two, yMaxLine[0], yMaxLine[1], intersection);
        tempMesh.addVertex(glm::vec3(intersection.x, intersection.y, 0.0f));
        glm::vec3 two2D = glm::vec3(two.x,two.y,0.0f);
        tempMesh.addVertex(two2D);
    }else if (one.y < yMax && two.y > yMax ){
        ofLineSegmentIntersection(one, two, yMaxLine[0], yMaxLine[1], intersection);
        glm::vec3 one2D = glm::vec3(one.x,one.y,0.0f);
        tempMesh.addVertex(one2D);
        tempMesh.addVertex(glm::vec3(intersection.x, intersection.y, 0.0f));
    }else if (one.y <= yMax && two.y <=yMax){
        tempMesh.addVertex(glm::vec3(one.x, one.y, 0.0f));
        tempMesh.addVertex(glm::vec3(two.x, two.y, 0.0f));
    }else if (one.y > yMax && two.y > yMax){
        
    }
    glm::vec2 yMinLine[2];
    yMinLine[0] = glm::vec2(xMin-10,yMin);
    yMinLine[1] = glm::vec2(xMax+10,yMin);
    for (int i = 0 ; i < tempMesh.getVertices().size(); i+=2) {
        glm::vec2 a = glm::vec2(tempMesh.getVertices()[i]);
        glm::vec2 b = glm::vec2(tempMesh.getVertices()[i+1]);
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


