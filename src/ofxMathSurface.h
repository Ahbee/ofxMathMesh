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
    vector<ofVec2f> backFaceTexCoords;
    vector<ofFloatColor> frontFaceColors;
    vector<ofVec2f> frontFaceTexCoords;
    vector<ofVec3f> frontFaceNormals;
    vector<ofVec3f> backFaceNormals;
    vector<ofVec3f> vertices;
    bool flatColors;

protected:
    void clear();
    static float distFromPlane(const ofVec3f &point,const ofVec3f &planeNormal,const float &planeD);
    static bool getSegmentPlaneIntersection(const ofVec3f &a,const ofVec3f &b,ofVec3f &intersectionPoint,const ofVec3f &planeNormal,const float &planeD);
    static void addTriangleToMesh(ofMesh &mesh,const ofVec3f &one,const ofVec3f &two,const ofVec3f &three);
    static void addQuadToMesh(ofMesh &mesh,const ofVec3f &one,const ofVec3f &two,const ofVec3f &three,const ofVec3f &four);
    static ofVec3f getCenter(const ofMeshFace& face);
};
