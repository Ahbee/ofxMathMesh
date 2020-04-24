

#include "ofxParametricSurface.h"

void ofxParametricSurface::setUMin(float uMin_){
    if (uMin_ >= absUMin) {
        uMin = uMin_;
    }else{
        uMin = absUMin;
        ofLogWarning("ofxMathMesh") << "Cannot set uMin smaller than setup uMin";
    }
}

void ofxParametricSurface::setUMax(float uMax_){
    if (uMax_ <= absUMax) {
        uMax = uMax_;
    }else{
        uMax = absUMax;
        ofLogWarning("ofxMathMesh") << "Cannot set uMax bigger than setup uMax";
    }
}

void ofxParametricSurface::setVMin(float vMin_){
    if (vMin_ >= absVMin) {
        vMin = vMin_;
    }else{
        vMin = absVMin;
        ofLogWarning("ofxMathMesh") << "Cannot set vMin smaller than setup vMin";
    }
}

void ofxParametricSurface::setVMax(float vMax_){
    if (vMax_ <= absVMax) {
        vMax = vMax_;
    }else{
        vMax = absVMax;
        ofLogWarning("ofxMathMesh") << "Cannot set vMax bigger than setup vMax";

    }
}

void ofxParametricSurface::setVStep(float step_){
    float range = absVMax - absVMin;
    if (step_ > range) {
        ofLogWarning("ofxMathMesh") << "cannot set step greater than range";
        vStep = range;
        return;
    }
    int res = round(range/step_);
    vStep = range/(float)res;
}

void ofxParametricSurface::setUStep(float step_){
    float range = absUMax - absUMin;
    if (step_ > range) {
        ofLogWarning("ofxMathMesh") << "cannot set step greater than range";
        uStep = range;
        return;
    }
    int res = round(range/step_);
    uStep = range/(float)res;
}

void ofxParametricSurface::setUBounds(float uMin_,float uMax_){
    setUMin(uMin_);
    setUMax(uMax_);
}
void ofxParametricSurface::setVBounds(float vMin_,float vMax_){
    setVMin(vMin_);
    setVMax(vMax_);
}

void ofxParametricSurface::loadDomainPoints(){
    float epsilon = uStep/100.0;
    for (float u = absUMin; u <= absUMax + epsilon; u+=uStep) {
        uDomainPoints.push_back(u);
    }
    epsilon = vStep/100.0;
    for (float v = absVMin; v <= absVMax + epsilon; v+=vStep) {
        vDomainPoints.push_back(v);
    }
}

void ofxParametricSurface::setup(float uMin_, float uMax_, float vMin_, float vMax_, float uStep_ ,float vStep_){
    if (uStep_ <= 0 || vStep_ <= 0  ) {
        ofLogWarning("ofxMathMesh") << "step must be greater than zero";
        return;
    }
    isSetup = true;
    absUMin = uMin_;
    absUMax = uMax_;
    absVMin = vMin_;
    absVMax = vMax_;
    setUStep(uStep_);
    setVStep(vStep_);
    loadDomainPoints();
    setUBounds(uMin_, uMax_);
    setVBounds(vMin_, vMax_);
    reload();
}

glm::vec3 ofxParametricSurface::normalForPoint(float u, float v,ofPoint value){
    // derivative u to the right
    glm::vec3 duRight;
    float delta = uStep;
    glm::vec3 valuePlus = valueForPoint(u+delta, v);
    duRight.x = (valuePlus.x - value.x)/delta;
    duRight.y = (valuePlus.y - value.y)/delta;
    duRight.z = (valuePlus.z - value.z)/delta;
    
//    // derivative u to the left
//    glm::vec3 duLeft;
//    glm::vec3 valueMinus = valueForPoint(u-delta, v);
//    duLeft.x = (valueMinus.x - value.x)/delta;
//    duLeft.y = (valueMinus.y - value.y)/delta;
//    duLeft.z = (valueMinus.z - value.z)/delta;
//    
//    glm::vec3 du = (-duLeft + duRight);
    
    // derivative v to the right
    glm::vec3 dvRight;
    delta = vStep;
    valuePlus = valueForPoint(u, v+delta);
    dvRight.x = (valuePlus.x - value.x)/delta;
    dvRight.y = (valuePlus.y - value.y)/delta;
    dvRight.z = (valuePlus.z - value.z)/delta;
    
//    // deravative v to the left
//    glm::vec3 dvLeft;
//    valueMinus = valueForPoint(u, v-delta);
//    dvLeft.x = (valueMinus.x - value.x)/delta;
//    dvLeft.y = (valueMinus.y - value.y)/delta;
//    dvLeft.z = (valueMinus.z - value.z)/delta;
//    
//    glm::vec3 dv = (dvRight + -dvLeft);


    glm::vec3 normal = glm::normalize(glm::cross(duRight, dvRight));
    return -normal;
}

void ofxParametricSurface::reload(){
    if (!isSetup) {
        ofLogError("ofxMathMesh") << "cannot reload if the surface is not setup";
        return;
    }
    clear();
    int uMinDomainPoint = round(ofMap(uMin, absUMin, absUMax, 0, uDomainPoints.size()-1));
    int uMaxDomainPoint = round(ofMap(uMax, absUMin, absUMax, 0, uDomainPoints.size()-1));
    int vMinDomainPoint = round(ofMap(vMin, absVMin, absVMax, 0, vDomainPoints.size()-1));
    int vMaxDomianPoint = round(ofMap(vMax, absVMin, absVMax, 0, vDomainPoints.size()-1));
    
    for (int u = uMinDomainPoint; u < uMaxDomainPoint; u++) {
        for (int v = vMinDomainPoint; v < vMaxDomianPoint; v++) {
            ofPoint value1 = valueForPoint(uDomainPoints[u], vDomainPoints[v]);
            ofPoint value2 = valueForPoint(uDomainPoints[u], vDomainPoints[v+1]);
            ofPoint value3 = valueForPoint(uDomainPoints[u+1], vDomainPoints[v+1]);
            ofPoint value4 = valueForPoint(uDomainPoints[u+1], vDomainPoints[v]);
            ParametricPosition one(uDomainPoints[u],vDomainPoints[v],value1);
            ParametricPosition two(uDomainPoints[u],vDomainPoints[v+1],value2);
            ParametricPosition three(uDomainPoints[u+1],vDomainPoints[v+1],value3);
            ParametricPosition four(uDomainPoints[u+1],vDomainPoints[v],value4);
            addQuad(one, two, three, four);
        }
    }
    
}

void ofxParametricSurface::addQuad(const ofxParametricSurface::ParametricPosition &one, const ofxParametricSurface::ParametricPosition &two, const ofxParametricSurface::ParametricPosition &three, const ofxParametricSurface::ParametricPosition &four){
    vertices.push_back(one.value);
    vertices.push_back(two.value);
    vertices.push_back(four.value);
    vertices.push_back(two.value);
    vertices.push_back(three.value);
    vertices.push_back(four.value);
    
    // load normals
    glm::vec3 normal1 = normalForPoint(one.u, one.v,one.value);
    glm::vec3 normal2 = normalForPoint(two.u,two.v,two.value);
    glm::vec3 normal3 = normalForPoint(three.u,three.v,three.value);
    glm::vec3 normal4 = normalForPoint(four.u,four.v,four.value);
    
    frontFaceNormals.push_back(normal1);
    frontFaceNormals.push_back(normal2);
    frontFaceNormals.push_back(normal4);
    frontFaceNormals.push_back(normal2);
    frontFaceNormals.push_back(normal3);
    frontFaceNormals.push_back(normal4);
    
    backFaceNormals.push_back(-normal1);
    backFaceNormals.push_back(-normal2);
    backFaceNormals.push_back(-normal4);
    backFaceNormals.push_back(-normal2);
    backFaceNormals.push_back(-normal3);
    backFaceNormals.push_back(-normal4);
    
    // load colors
    ofFloatColor color1 = colorForPoint(one.u,one.v,one.value);
    ofFloatColor color2 = colorForPoint(two.u,two.v,two.value);
    ofFloatColor color3 = colorForPoint(three.u,three.v,three.value);
    ofFloatColor color4 = colorForPoint(four.u,four.v,four.value);
    if (flatColors) {
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color1);
    }else{
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color2);
        frontFaceColors.push_back(color4);
        frontFaceColors.push_back(color2);
        frontFaceColors.push_back(color3);
        frontFaceColors.push_back(color4);
    }
    
    color1 = backColorForPoint(one.u,one.v,one.value);
    color2 = backColorForPoint(two.u,two.v,two.value);
    color3 = backColorForPoint(three.u,three.v,three.value);
    color4 = backColorForPoint(four.u,four.v,four.value);
    
    if (flatColors) {
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color1);
    }else{
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color2);
        backFaceColors.push_back(color4);
        backFaceColors.push_back(color2);
        backFaceColors.push_back(color3);
        backFaceColors.push_back(color4);
    }
    
   //load texCoords
    glm::vec2 tex1 = texCoordForPoint(one.u, one.v,one.value);
    glm::vec2 tex2 = texCoordForPoint(two.u,two.v,two.value);
    glm::vec2 tex3 = texCoordForPoint(three.u,three.v,three.value);
    glm::vec2 tex4 = texCoordForPoint(four.u,four.v,four.value);
    
    frontFaceTexCoords.push_back(tex1);
    frontFaceTexCoords.push_back(tex2);
    frontFaceTexCoords.push_back(tex4);
    frontFaceTexCoords.push_back(tex2);
    frontFaceTexCoords.push_back(tex3);
    frontFaceTexCoords.push_back(tex4);
    
    tex1 = backTexCoordForPoint(one.u, one.v,one.value);
    tex2 = backTexCoordForPoint(two.u,two.v,two.value);
    tex3 = backTexCoordForPoint(three.u,three.v,three.value);
    tex4 = backTexCoordForPoint(four.u,four.v,four.value);
    
    backFaceTexCoords.push_back(tex1);
    backFaceTexCoords.push_back(tex2);
    backFaceTexCoords.push_back(tex4);
    backFaceTexCoords.push_back(tex2);
    backFaceTexCoords.push_back(tex3);
    backFaceTexCoords.push_back(tex4);
}






