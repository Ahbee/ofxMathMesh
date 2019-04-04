
#include "ofxMathSurface.h"

void ofxMathSurface::draw(bool colors,bool texCoords){
    vbo.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    if (colors) {
        vbo.enableColors();
        vbo.setColorData(frontFaceColors.data(), frontFaceColors.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableColors();
    }
    
    if (texCoords) {
        vbo.enableTexCoords();
        vbo.setTexCoordData(frontFaceTexCoords.data(), frontFaceTexCoords.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableTexCoords();
    }
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    vbo.setNormalData(frontFaceNormals.data(), frontFaceNormals.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_TRIANGLES, 0, vertices.size());
    glCullFace(GL_FRONT);
    vbo.setNormalData(backFaceNormals.data(), backFaceNormals.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_TRIANGLES, 0, vertices.size());
    glDisable(GL_CULL_FACE);
}

void ofxMathSurface::drawFrontFaces(bool colors, bool texCoords){
    vbo.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    vbo.setNormalData(frontFaceNormals.data(), frontFaceNormals.size(), GL_DYNAMIC_DRAW);
    if (colors) {
        vbo.enableColors();
        vbo.setColorData(frontFaceColors.data(), frontFaceColors.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableColors();
    }
    if (texCoords) {
        vbo.enableTexCoords();
        vbo.setTexCoordData(frontFaceTexCoords.data(), frontFaceTexCoords.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableTexCoords();
    }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    vbo.draw(GL_TRIANGLES, 0, vertices.size());
    glDisable(GL_CULL_FACE);
}

void ofxMathSurface::drawBackFaces(bool colors,bool texCoords){
    vbo.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    vbo.setNormalData(backFaceNormals.data(), backFaceNormals.size(), GL_DYNAMIC_DRAW);
    if (colors) {
        vbo.enableColors();
        vbo.setColorData(backFaceColors.data(), backFaceColors.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableColors();
    }
    if (texCoords) {
        vbo.enableTexCoords();
        vbo.setTexCoordData(backFaceTexCoords.data(), backFaceTexCoords.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableTexCoords();
    }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    vbo.draw(GL_TRIANGLES, 0, vertices.size());
    glDisable(GL_CULL_FACE);
}

void ofxMathSurface::drawWireFrame(bool colors){
    if (colors) {
        vbo.enableColors();
        vbo.setColorData(frontFaceColors.data(), frontFaceColors.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableColors();
    }
    vbo.setNormalData(frontFaceNormals.data(), frontFaceNormals.size(), GL_DYNAMIC_DRAW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    vbo.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_TRIANGLES, 0, vertices.size());
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ofxMathSurface::drawPoints(bool colors){
    vbo.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    if (colors) {
        vbo.enableColors();
        vbo.setColorData(frontFaceColors.data(), frontFaceColors.size(), GL_DYNAMIC_DRAW);
    }else{
        vbo.disableColors();
    }
    vbo.setNormalData(frontFaceNormals.data(), frontFaceNormals.size(), GL_DYNAMIC_DRAW);
    vbo.draw(GL_POINTS, 0, vertices.size());
}

void ofxMathSurface::drawNormals(float length) const{
    ofMesh normalsMesh;
    normalsMesh.setMode( OF_PRIMITIVE_LINES );
    normalsMesh.getVertices().resize( frontFaceNormals.size() * 2);
    const vector<glm::vec3>& normals = frontFaceNormals;
    glm::vec3 normal;
    glm::vec3 vert;
    
    for(int i = 0; i < (int)frontFaceNormals.size(); i++) {
        vert = vertices[i];
        normal = normals[i];
        normal = glm::normalize(normal);
        normalsMesh.setVertex( i*2, vert);
        normal *= length;
        normalsMesh.setVertex(i*2+1, normal+vert);
    }
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    normalsMesh.draw();
    
    normalsMesh.getVertices().resize( backFaceNormals.size() * 2);
    const vector<glm::vec3>& backNormals = backFaceNormals;
    for(int i = 0; i < (int)backNormals.size(); i++) {
        vert = vertices[i];
        normal = backNormals[i];
        normal = glm::normalize(normal);
        normalsMesh.setVertex( i*2, vert);
        normal *= length;
        normalsMesh.setVertex(i*2+1, normal+vert);
    }
    glCullFace(GL_BACK);
    normalsMesh.draw();
    glDisable(GL_CULL_FACE);
}

glm::vec3 ofxMathSurface::getCenter(const ofMeshFace& meshface){
    const glm::vec3 &v1 = meshface.getVertex(0);
    const glm::vec3 &v2 = meshface.getVertex(1);
    const glm::vec3 &v3 = meshface.getVertex(2);
    return (v1 + v2 + v3)/3.0;
}

void ofxMathSurface::drawFaceNormals(float length) const {
    ofMesh nMesh;
    nMesh.setMode(OF_PRIMITIVE_LINES);
    const vector<glm::vec3> &verts = vertices;
    for (int i = 0; i < verts.size(); i+=3) {
        glm::vec3 v1 = verts[i];
        glm::vec3 v2 = verts[i+1];
        glm::vec3 v3 = verts[i+2];
        ofMeshFace meshFace;
        meshFace.setVertex(0, v1);
        meshFace.setVertex(1, v2);
        meshFace.setVertex(2, v3);
        glm::vec3 norm = meshFace.getFaceNormal() * length;
        glm::vec3 center = getCenter(meshFace);
        nMesh.addVertex(center);
        nMesh.addVertex(center + norm);
    }
    nMesh.draw();
}

ofVbo ofxMathSurface::getVbo()const{
    ofVbo mesh;
    mesh.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    mesh.setNormalData(frontFaceNormals.data(), frontFaceNormals.size(), GL_DYNAMIC_DRAW);
    mesh.setColorData(frontFaceColors.data(), frontFaceColors.size(), GL_DYNAMIC_DRAW);
    mesh.setTexCoordData(frontFaceTexCoords.data(), frontFaceTexCoords.size(), GL_DYNAMIC_DRAW);
    return mesh;
}

ofVbo ofxMathSurface::getBackVbo()const{
    ofVbo mesh;
    mesh.setVertexData(vertices.data(), vertices.size(), GL_DYNAMIC_DRAW);
    mesh.setNormalData(backFaceNormals.data(), backFaceNormals.size(), GL_DYNAMIC_DRAW);
    mesh.setColorData(backFaceColors.data(), backFaceColors.size(), GL_DYNAMIC_DRAW);
    mesh.setTexCoordData(backFaceTexCoords.data(), backFaceTexCoords.size(), GL_DYNAMIC_DRAW);
    return mesh;
}

ofMesh ofxMathSurface::getMesh()const{
    ofMesh mesh;
    mesh.addVertices(vertices);
    mesh.addNormals(frontFaceNormals);
    mesh.addColors(frontFaceColors);
    mesh.addTexCoords(frontFaceTexCoords);
    return mesh;
}

ofMesh ofxMathSurface::getBackMesh()const{
    ofMesh mesh;
    mesh.addVertices(vertices);
    mesh.addNormals(backFaceNormals);
    mesh.addColors(backFaceColors);
    mesh.addTexCoords(backFaceTexCoords);
    return mesh;
}

void ofxMathSurface::clear(){
    vertices.resize(0);
    frontFaceNormals.resize(0);
    backFaceNormals.resize(0);
    frontFaceColors.resize(0);
    backFaceColors.resize(0);
    frontFaceTexCoords.resize(0);
    backFaceTexCoords.resize(0);
}

float ofxMathSurface::distFromPlane(const glm::vec3 &point, const glm::vec3 &planeNormal, const float &planeD){
    return glm::dot(planeNormal, point) + planeD;
}

bool ofxMathSurface::getSegmentPlaneIntersection(const glm::vec3 &a, const glm::vec3 &b, glm::vec3 &intersectionPoint, const glm::vec3 &planeNormal, const float &planeD){
    float d1 = distFromPlane(a, planeNormal, planeD);
    float d2 = distFromPlane(b, planeNormal, planeD);
    if (d1*d2 > 0) {
        return false;
    }
    float t = d1/(d1-d2);
    intersectionPoint = a + t * (b-a);
    return true;
}

void ofxMathSurface::addTriangleToMesh(ofMesh &mesh, const glm::vec3 &one, const glm::vec3 &two, const glm::vec3 &three){
    mesh.addVertex(one);
    mesh.addVertex(two);
    mesh.addVertex(three);
}

void ofxMathSurface::addQuadToMesh(ofMesh &mesh, const glm::vec3 &one, const glm::vec3 &two, const glm::vec3 &three, const glm::vec3 &four){
    mesh.addVertex(one);
    mesh.addVertex(two);
    mesh.addVertex(four);
    mesh.addVertex(two);
    mesh.addVertex(three);
    mesh.addVertex(four);
}




