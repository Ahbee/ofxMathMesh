#pragma once

#include "ofMain.h"
#include "ofxMathMesh.h"
#include "ofxAnimatableFloat.h"

class Butterfly:public ofxParametricCurve {
    ofPoint valueForPoint(float t){
        float p1 = pow((float)M_E, cos(t));
        float p2 = 2 * cos(4*t);
        float p3 = pow(sin(t/12.0), 5);
        float x = sin(t) * (p1 - p2 -p3);
        float y = cos(t) * (p1 - p2 - p3);
        return ofPoint(x,y,0);
    }
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    ofEasyCam cam;
    Butterfly butterfly;
    ofxAnimatableFloat tMax;
		
};
