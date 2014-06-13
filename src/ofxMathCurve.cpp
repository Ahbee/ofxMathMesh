#include "ofxMathCurve.h"


ofMesh ofxMathCurve::getMesh()const{
    ofMesh mesh;
    mesh.addVertices(vertices);
    mesh.addColors(colors);
    return mesh;
}

ofVbo ofxMathCurve::getVbo()const{
    ofVbo mesh;
    mesh.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    mesh.setColorData(colors.data(), colors.size(), GL_DYNAMIC_DRAW);
    return mesh;
}


void ofxMathCurve::draw(bool colors_){
    vbo.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    if (colors_) {
        vbo.enableColors();
        vbo.setColorData(colors.data(), colors.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableColors();
    }
    vbo.draw(GL_LINES, 0, vertices.size());
}

void ofxMathCurve::drawPoints(bool colors_){
    vbo.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    if (colors_) {
        vbo.enableColors();
        vbo.setColorData(colors.data(), colors.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableColors();
    }
    vbo.draw(GL_POINTS, 0, vertices.size());
}


void ofxMathCurve::clear(){
    vertices.clear();
    colors.clear();
}
