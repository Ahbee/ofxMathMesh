#pragma once

#include "ofMain.h"

// This class is an abstract base for ofx2dFunction and ofxParametricCurve

class ofxMathCurve {
public:
    void draw(bool colors);
    void drawPoints(bool colors);
    ofMesh getMesh() const;
    ofVbo getVbo() const;
private:
    ofVbo vbo;
protected:
    vector<glm::vec3> vertices;
    vector<ofFloatColor> colors;
protected:
    void clear();
};
