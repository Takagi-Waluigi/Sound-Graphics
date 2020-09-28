#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxEasyFft.h"
#include "ofxOsc.h"
#include "ofTrueTypeFont.h"

#include "graphicPatternClass.hpp"

#define PORT 8000

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
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void plot(vector<float>& buffer, float scale);
    void dumpOSC(ofxOscMessage msg);
    void modeActivator(int num);
    void drawFlower();
		
    ofShader shader;
    ofxEasyFft fft;
    
    ofEasyCam cam;
    
    ofxOscReceiver receiver;
    
    ofPoint pos;
    
    int nChan, nVal, nPitch;
    
    bool mode[8];
    
    float rotX, rotY, rotZ;
    
    float speed;
    float size;
    
    float ampAvg;
    
    ofVec3f flowerPos;
    ofVec3f flowerRot;
    float rotSpeed = 0;
    float rot;
    float ampMax;
    ofFbo fbo;
    
    float imgAmp;
    float imgSpread;
    
    GraphicsPattern graphics;
    GraphicsPattern shapeGrid[3];
    
    GraphicsPattern rains[2048];
    float fallSpeed;
    float threshold;
    float rainAmount = 1024;
    
    ofTrueTypeFont hanamin;
    
    float flashAmount;
    float nPow;
    float flashRadius;
    
    bool debugMode;
    
    float beltY;
    float crossZ = -500;
    float crossSpeed;
    float crossRotY;
    float crossRotX;
    
    GraphicsPattern glass[42000];
    int debrisSize = 12;
    int glassW = 350;
    int glassH = 120;
    
    ofVec3f cityPos;
    float cityAngle;
    
    ofSoundStream stream;
    
    GraphicsPattern maps[4];
    int mapNum = 4;
    
    float freq;
};
