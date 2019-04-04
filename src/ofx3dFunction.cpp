#include "ofx3dFunction.h"

void ofx3dFunction::setXStep(float step_){
    float range = absXMax - absXMin;
    if (step_ > range) {
        ofLogWarning("ofxMathMesh") << "cannot set step greater than range";
        xStep = range;
        return;
    }
    int res = round(range/step_);
    xStep = range/(float)res;
}

void ofx3dFunction::setZStep(float step_){
    float range = absZMax - absZMin;
    if (step_ > range) {
        ofLogWarning("ofxMathMesh") << "cannot set step greater than range";
        zStep = range;
        return;
    }
    int res = round(range/step_);
    zStep = range/(float)res;
}

void ofx3dFunction::setXMin(float xMin_){
    if (xMin_ >= absXMin) {
        xMin = xMin_;
    }else{
        xMin = absXMin;
        ofLogWarning("ofxMathMesh") << "cannot set xMin smaller than setup xMin";
    }
}

void ofx3dFunction::setXMax(float xMax_){
    if (xMax_ <= absXMax) {
        xMax = xMax_;
    }else{
        xMax = absXMax;
        ofLogWarning("ofxMathMesh") << "cannot set xMax bigger than setup xMax";

    }
}

void ofx3dFunction::setZMin(float zMin_){
    if (zMin_ >= absZMin) {
        zMin = zMin_;
    }else{
        zMin = absZMin;
        ofLogWarning("ofxMathMesh") << "cannot set zMin smaller than setup zMin";
    }
}

void ofx3dFunction::setZMax(float zMax_){
    if (zMax_ <= absZMax) {
        zMax = zMax_;
    }else{
        zMax = absZMax;
        ofLogWarning("ofxMathMesh") << "cannot set zMax bigger than setup zMax";
    }
}

void ofx3dFunction::loadDomainPoints(){
    float epsilon = xStep/100.0;
    for (float x = absXMin; x <= absXMax + epsilon; x+=xStep) {
        xDomainPoints.push_back(x);
    }
    epsilon = zStep/100.0;
    for (float z = absZMin; z <= absZMax + epsilon; z+=zStep) {
        zDomainPoints.push_back(z);
    }
}

void ofx3dFunction::setXBounds(float xMin_, float xMax_){
    setXMin(xMin_);
    setXMax(xMax_);
}

void ofx3dFunction::setZBounds(float zMin_, float zMax_){
    setZMin(zMin_);
    setZMax(zMax_);
}

void ofx3dFunction::setYBounds(float yMin, float yMax){
    setUseYBounds(true);
    this->yMin = yMin;
    this->yMax = yMax;
}

void ofx3dFunction::setup(float xMin_, float xMax_, float zMin_, float zMax_,float xStep_,float zStep_){
    if (xStep_ <= 0 || zStep_ <= 0  ) {
        ofLogWarning("ofxMathMesh") << "step must be greater than zero";
        return;
    }
    isSetup = true;
    absXMin = xMin_;
    absXMax = xMax_;
    absZMin = zMin_;
    absZMax = zMax_;
    setXStep(xStep_);
    setZStep(zStep_);
    loadDomainPoints();
    setXBounds(xMin_, xMax_);
    setZBounds(zMin_, zMax_);
    reload();
}

void ofx3dFunction::setup(float xMin_, float xMax_, float zMin_, float zMax_, float yMin_, float yMax_,float xStep_, float zStep_){
    setYBounds(yMin_, yMax_);
    setup(xMin_,xMax_,zMin_,zMax_,xStep_,zStep_);
}

void ofx3dFunction::reload(){
    if (!isSetup) {
        ofLogError("ofxMathMesh") << "cannot reload if function is not setup";
        return;
    }
    clear();
    int xMinDomainPoint = round(ofMap(xMin, absXMin, absXMax, 0, xDomainPoints.size() - 1));
    int xMaxDomainPoint = round(ofMap(xMax, absXMin, absXMax, 0, xDomainPoints.size()-1));
    int zMinDomainPoint = round(ofMap(zMin, absZMin, absZMax, 0, zDomainPoints.size() - 1));
    int zMaxDomainPoint = round(ofMap(zMax, absZMin, absZMax, 0, zDomainPoints.size()-1));
    for (int x = xMinDomainPoint; x < xMaxDomainPoint; x++) {
        for (int z = zMinDomainPoint; z < zMaxDomainPoint; z++) {
            float y1 = valueForPoint(xDomainPoints[x], zDomainPoints[z]);
            float y2 = valueForPoint(xDomainPoints[x], zDomainPoints[z+1]);
            float y3 = valueForPoint(xDomainPoints[x+1], zDomainPoints[z+1]);
            float y4 = valueForPoint(xDomainPoints[x+1], zDomainPoints[z]);
            
            glm::vec3 one = glm::vec3(xDomainPoints[x],y1,zDomainPoints[z]);
            glm::vec3 two = glm::vec3(xDomainPoints[x],y2,zDomainPoints[z+1]);
            glm::vec3 three = glm::vec3(xDomainPoints[x+1],y3,zDomainPoints[z+1]);
            glm::vec3 four = glm::vec3(xDomainPoints[x+1],y4,zDomainPoints[z]);
            if (bUseYBounds) {
                addTriangleWithClip(one, two, four);
                addTriangleWithClip(two, three, four);
            }else{
                addQuad(one,two,three,four);
            }
        }
    }
}

glm::vec3 ofx3dFunction::normalForPoint(float x, float z,float y){
    float yValue = y;
    float delta = xStep;
    
    // get derivative x in positive direction
    float derivativeXPos = (valueForPoint(x + delta, z) - yValue)/delta;
    // get derivative x in negative direction
    float derivativeXNeg = (valueForPoint(x-delta,z) - yValue)/(-delta);
    // average the two to get derivative x
    float derivativeX = (derivativeXPos + derivativeXNeg)/2.0 ;
    
    delta = zStep;
    // repaeat same procedure for z
    float derivativeZPos = (valueForPoint(x, z+delta) - yValue)/delta;
    float derivativeZNeg = (valueForPoint(x,z-delta) - yValue)/(-delta);
    float derivativeZ = (derivativeZPos + derivativeZNeg)/2.0;
    
    // the mathWorld formula gives back faced normals so we have to reverse the normal;
    glm::vec3 normal = -glm::normalize(glm::vec3(derivativeX,-1.0f,derivativeZ));
    
    return normal;
}

void ofx3dFunction::addQuad(const glm::vec3 &one, const glm::vec3 &two, const glm::vec3 &three, const glm::vec3 &four){
    addTriangle(one, two, four);
    addTriangle(two, three, four);
}

void ofx3dFunction::addTriangle(const glm::vec3 &one, const glm::vec3 &two, const glm::vec3 &three){
    vertices.push_back(one);
    vertices.push_back(two);
    vertices.push_back(three);
    
    // load normals
    glm::vec3 normal1 = normalForPoint(one.x, one.z,one.y);
    glm::vec3 normal2 = normalForPoint(two.x, two.z,two.y);
    glm::vec3 normal3 = normalForPoint(three.x,three.z,three.y);
    
    frontFaceNormals.push_back(normal1);
    frontFaceNormals.push_back(normal2);
    frontFaceNormals.push_back(normal3);
    
    backFaceNormals.push_back(-normal1);
    backFaceNormals.push_back(-normal2);
    backFaceNormals.push_back(-normal3);
    
    
    // load colors
    ofFloatColor color1 = colorForPoint(one.x,one.z,one.y);
    ofFloatColor color2 = colorForPoint(two.x,two.z,two.y);
    ofFloatColor color3 = colorForPoint(three.x,three.z,three.y);
    
    if (flatColors) {
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color1);
    }else{
        frontFaceColors.push_back(color1);
        frontFaceColors.push_back(color2);
        frontFaceColors.push_back(color3);
    }
    
    color1 = backColorForPoint(one.x,one.z,one.y);
    color2 = backColorForPoint(two.x,two.z,two.y);
    color3 = backColorForPoint(three.x,three.z,three.y);
    
    
    if (flatColors) {
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color1);
    }else{
        backFaceColors.push_back(color1);
        backFaceColors.push_back(color2);
        backFaceColors.push_back(color3);
    }
    
    //load texCoords
    glm::vec2 tex1 = texCoordForPoint(one.x, one.z,one.y);
    glm::vec2 tex2 = texCoordForPoint(two.x,two.z,two.y);
    glm::vec2 tex3 = texCoordForPoint(three.x,three.z,three.y);
    
    frontFaceTexCoords.push_back(tex1);
    frontFaceTexCoords.push_back(tex2);
    frontFaceTexCoords.push_back(tex3);
    
    tex1 = backTexCoordForPoint(one.x, one.z,one.y);
    tex2 = backTexCoordForPoint(two.x,two.z,two.y);
    tex3 = backTexCoordForPoint(three.x,three.z,three.y);
    
    backFaceTexCoords.push_back(tex1);
    backFaceTexCoords.push_back(tex2);
    backFaceTexCoords.push_back(tex3);
    
}

void ofx3dFunction::addTriangleWithClip(const glm::vec3 &a,const glm::vec3 &b,const glm::vec3 &c){
    ofMesh mesh;
    glm::vec3 planeNormal = glm::vec3(0,1,0);
    float planeD = -yMin;
    glm::vec3 lineSeg[2];
    
    if (a.y < yMin && b.y > yMin && c.y > yMin) {
        getSegmentPlaneIntersection(a, b, lineSeg[0], planeNormal, planeD);
        getSegmentPlaneIntersection(a, c, lineSeg[1], planeNormal, planeD);
        addQuadToMesh(mesh, lineSeg[0], b, c, lineSeg[1]);
    }else if (a.y > yMin && b.y < yMin && c.y > yMin){
        getSegmentPlaneIntersection(b, a, lineSeg[0], planeNormal, planeD);
        getSegmentPlaneIntersection(b, c, lineSeg[1], planeNormal, planeD);
        addQuadToMesh(mesh,a, lineSeg[0], lineSeg[1], c);
    }else if (a.y > yMin && b.y > yMin && c.y < yMin){
        getSegmentPlaneIntersection(c, b, lineSeg[0], planeNormal, planeD);
        getSegmentPlaneIntersection(c, a, lineSeg[1], planeNormal, planeD);
        addQuadToMesh(mesh, a, b, lineSeg[0], lineSeg[1]);
    }else if (a.y <= yMin && b.y <= yMin && c.y > yMin){
        getSegmentPlaneIntersection(c, a, lineSeg[0], planeNormal, planeD);
        getSegmentPlaneIntersection(c, b, lineSeg[1], planeNormal, planeD);
        addTriangleToMesh(mesh, lineSeg[0], lineSeg[1],c);
    }else if (a.y > yMin && b.y <= yMin && c.y <= yMin){
        getSegmentPlaneIntersection(a, b, lineSeg[0], planeNormal, planeD);
        getSegmentPlaneIntersection(a, c, lineSeg[1], planeNormal, planeD);
        addTriangleToMesh(mesh,a,lineSeg[0], lineSeg[1]);
    }else if (a.y <= yMin && b.y > yMin && c.y <= yMin){
        getSegmentPlaneIntersection(b, a, lineSeg[0], planeNormal, planeD);
        getSegmentPlaneIntersection(b, c, lineSeg[1], planeNormal, planeD);
        addTriangleToMesh(mesh, lineSeg[0], b, lineSeg[1]);
    }else if (a.y >= yMin && b.y >= yMin && c.y >= yMin){
        addTriangleToMesh(mesh, a, b, c);
    }else if (a.y < yMin && b.y < yMin && c.y < yMin){
        
    }
    
    planeD = -yMax;
    for (int i = 0; i < mesh.getVertices().size();i+=3) {
        glm::vec3 a = mesh.getVertices()[i];
        glm::vec3 b = mesh.getVertices()[i+1];
        glm::vec3 c = mesh.getVertices()[i+2];
        if (a.y > yMax && b.y < yMax && c.y < yMax) {
            getSegmentPlaneIntersection(a, b, lineSeg[0], planeNormal, planeD);
            getSegmentPlaneIntersection(a, c, lineSeg[1], planeNormal, planeD);
            addQuad(lineSeg[0], b, c, lineSeg[1]);
        }else if (a.y < yMax && b.y > yMax && c.y < yMax){
            getSegmentPlaneIntersection(b, a, lineSeg[0], planeNormal, planeD);
            getSegmentPlaneIntersection(b, c, lineSeg[1], planeNormal, planeD);
            addQuad(a, lineSeg[0], lineSeg[1], c);
        }else if (a.y < yMax && b.y < yMax && c.y > yMax){
            getSegmentPlaneIntersection(c, b, lineSeg[0], planeNormal, planeD);
            getSegmentPlaneIntersection(c, a, lineSeg[1], planeNormal, planeD);
            addQuad(a, b, lineSeg[0], lineSeg[1]);
        }else if (a.y < yMax && b.y >= yMax && c.y >=yMax){
            getSegmentPlaneIntersection(a, b, lineSeg[0], planeNormal, planeD);
            getSegmentPlaneIntersection(a, c, lineSeg[1], planeNormal, planeD);
            addTriangle(a, lineSeg[0], lineSeg[1]);
        }else if (a.y >= yMax && b.y < yMax && c.y >=yMax){
            getSegmentPlaneIntersection(b, a, lineSeg[0], planeNormal, planeD);
            getSegmentPlaneIntersection(b, c, lineSeg[1], planeNormal, planeD);
            addTriangle(lineSeg[0], b, lineSeg[1]);
        }else if (a.y >=yMax && b.y >= yMax && c.y < yMax){
            getSegmentPlaneIntersection(c, a, lineSeg[0], planeNormal, planeD);
            getSegmentPlaneIntersection(c, b, lineSeg[1], planeNormal, planeD);
            addTriangle(lineSeg[0], lineSeg[1], c);
        }else if (a.y <= yMax && b.y <= yMax && c.y <= yMax){
            addTriangle(a, b, c);
        }else if (a.y > yMax && b.y > yMax && c.y > yMax){
            
        }
    }
}
