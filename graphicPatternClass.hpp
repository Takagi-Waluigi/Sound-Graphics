//  Created by 高木健太 on 2020/08/17.
//

#ifndef graphicPatternClass_hpp
#define graphicPatternClass_hpp

#include <stdio.h>
#include "ofMain.h"

class GraphicsPattern{
public:
    
    //B
    void init_shapeGrid(int beginPos);
    void draw_shapeGrid(int n, float speed, float size);
    
        float B_posZ = 0;
        float B_radius = 0;
    
    
    //C
    void bordreLine(float speed, float input_max);
    
    //D
    void init_Rings();
    void update_Rings();
    void draw_Rings(int num, float radius, float spped, float maxR, float ampInput);
    
        int rings_time;
    
    //E
    void init_imageDistortion();
    void draw_imageDistortion(float amp, float inputAmp, float threshold, float spread, float freq);
    void draw_imageDistortionB(float threshold, float amp);
    void update_imageDistortion();
    void flashLight(float radius, float npow);

    
        ofImage img;
        ofImage imgs[25];
        int imageNum;
        string fileName[25];
        ofVec3f img_amp;
        ofVec3f img_amp_plus = ofVec3f(0.05, 0.05, 0.1);
        ofVec3f pos;
        ofVec3f wave;
        float drop;
        ofVec3f slip;
        ofVec3f past_pos;
        bool imgPlusFlag = false;
        int img_time = 0;
    
    //F
    void draw_redCross(float inputAmp, float speed);
    
    //H
    void init_fallingDown();
    void draw_fallingDown(float fall_speed);
    void update_fallingDown();
    
        ofVec3f fall_pos;
        bool isDisplay;
    
    //I
    void init_breaking(float initX, float initY, float size);
    void draw_breaking();
    void update_breaking();
    
        ofVec3f debrisPos;
        ofVec3f debrisSp;
        int breaking_time = 0;
        int debrisSize;
        float beginAngle;
        bool isDebrisDisplay;
    
    //J
    void init_activeMap(float initZ, float initNum, int initInterval);
    void update_activeMap();
    void draw_activeMap(float amp, ofVec3f pos, float angle);
    
        ofImage mapImg;
        ofImage mapImgs[27];
        ofVec3f mapPos;
        ofVec3f globMapPos;
        float mapMoveZ;
        float mapSpeed = 10.0;
        int mapImgNum = 0;
        int mapImgInterval;
        bool isMapDisplay;
    
    //K
    void drawWavingPlane();
    void drawWavingSingle();
        ofVec3f wavePoint[1200];
    
    //L
    void init_face();
    void draw_face();
    ofImage faceImg;
    
    //side functions
    void shapeMaker(int n, ofVec3f pos, float radius);
    void draw_simpleWave(float amp, float speed, float width, float  slip, float y_offset);
    void draw_flash(float threshold, float amp);
    void draw_rects(float n, ofColor col);
    
    
    bool isLineDisplay = false;
    float lineAlpha = 255;
    float line_w = 0;
    float line_max;
    float line_beginPos;
    
    
    
    
};

#endif /* graphicPatternClass_hpp */
