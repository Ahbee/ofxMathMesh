#pragma once
#include "ofMain.h"

// This class is an abstract base for ofx3dFunction and ofxParametricCurve
class ofxMathSurface{
public:
    
     // 'colors' and 'texcoords' arguments specify if you want to use the colorForPoint and texCoordForPoint functions
     //  you defined. drawBackFaces will use functions 'backColorForPoint' and 'backTexCoordForPoint', if you pass 'true' to 'colors' and 'texCoords'
    void draw(bool colors,bool texCoords);
    void drawFrontFaces(bool colors, bool texCoords);
    void drawBackFaces(bool colors,bool texCoords);
    void drawWireFrame(bool colors);
    void drawPoints(bool colors);
    void drawNormals(float length) const;
    void drawFaceNormals(float length) const;
    
    // flatColors will set the same color for each vertex in a quad;
    void enableFlatColors(){flatColors = true;}
    void disableFlatColors(){flatColors = false;}
    
    // returns a vbo which with vertices,frontNormals,frontColors,and frontTexCoords
    ofVbo getVbo() const;
    
    // returns a vbo which with vertices,backNormals,backColors,and backTexCoords
    ofVbo getBackVbo() const;
    
    // returns a mesh which with vertices,frontNormals,frontColors,and frontTexCoords
    ofMesh getMesh() const;
    
    // returns a mesh which with vertices,backNormals,backColors,and backTexCoords
    ofMesh getBackMesh() const;

private:
    ofVbo vbo;

protected:
    vector<ofFloatColor> backFaceColors;
    vector<glm::vec2> backFaceTexCoords;
    vector<ofFloatColor> frontFaceColors;
    vector<glm::vec2> frontFaceTexCoords;
    vector<glm::vec3> frontFaceNormals;
    vector<glm::vec3> backFaceNormals;
    vector<glm::vec3> vertices;
    bool flatColors;

protected:
    void clear();
    static float distFromPlane(const glm::vec3 &point,const glm::vec3 &planeNormal,const float &planeD);
    static bool getSegmentPlaneIntersection(const glm::vec3 &a,const glm::vec3 &b,glm::vec3 &intersectionPoint,const glm::vec3 &planeNormal,const float &planeD);
    static void addTriangleToMesh(ofMesh &mesh,const glm::vec3 &one,const glm::vec3 &two,const glm::vec3 &three);
    static void addQuadToMesh(ofMesh &mesh,const glm::vec3 &one,const glm::vec3 &two,const glm::vec3 &three,const glm::vec3 &four);
    static glm::vec3 getCenter(const ofMeshFace& face);
};
