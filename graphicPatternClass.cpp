//  Created by 高木健太 on 2020/08/17.
//

#include "graphicPatternClass.hpp"

//--------------------------------------------------------------
void GraphicsPattern::init_shapeGrid(int beginPos){
    B_posZ = beginPos;
}

void GraphicsPattern::draw_shapeGrid(int n, float speed, float size){
    float radius = ofMap(B_posZ, 0, 1000, 0, 50) * size;
    float alpha = ofMap(B_posZ, 0, 2000, 0, 255);
    ofSetColor(200, 0, 0, alpha);
    
    for(int x = -ofGetWidth()/2; x < ofGetWidth()/2; x += 40){
        for(int y = -ofGetHeight()/2; y < ofGetHeight()/2; y += 40){
            shapeMaker(n, ofVec3f(x, y, B_posZ), radius);
        }
    }
    
    B_posZ += speed;
    
    if (B_posZ > 1000) {
        B_posZ = 0;
    }
    
}

//--------------------------------------------------------------
void GraphicsPattern::bordreLine(float speed, float input_max){
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    lineAlpha = ofMap(line_w, 0, 500, 255, 0);
    ofSetColor(lineAlpha);
    
    if(isLineDisplay){
        ofDrawRectangle(line_beginPos, 0, line_w, ofGetHeight());
        line_w += speed;
    }
    
    if(line_w > 500){
        line_w = 0;
        isLineDisplay = false;
    }
    
    if(isLineDisplay == false){
        line_beginPos = ofRandom(-ofGetWidth()/2, ofGetWidth()/2);
        isLineDisplay = true;
    }
}

//--------------------------------------------------------------
void GraphicsPattern::init_Rings(){
    rings_time = 0;
}

void GraphicsPattern::update_Rings(){
    rings_time ++;
}

void GraphicsPattern::draw_Rings(int num, float radius, float speed, float maxR, float ampInput){
    ofPushMatrix();
    float offset = ofNormalize(radius, 0, maxR);
    ofRotateYDeg(rings_time * speed * offset);
    ofRotateXDeg(rings_time * speed * (1 -offset));
    ofRotateZDeg(rings_time * speed * offset * 0.5);
    
    shapeMaker(num, ofVec3f(0, 0, 0), radius * ampInput);
    ofPopMatrix();
}

//--------------------------------------------------------------
void GraphicsPattern::init_imageDistortion(){
    for(int i=0; i<25; i++){
        fileName[i]= "images/city/city" + ofToString(i + 1) + ".jpg";
        imgs[i].load(fileName[i]);
        imgs[i].resize(ofGetWidth(), ofGetHeight());
    }
    
    img_amp = ofVec3f(100.0, 100.0, 300.0);
    imageNum = (int)ofRandom(1, 25);
    
    img_time = 0;
}

void GraphicsPattern::update_imageDistortion(){
    for(int i = 0; i < 25; i ++){
        imgs[i].resize(ofGetWidth(), ofGetHeight());
    }
    
    imageNum = ((ofGetFrameNum() + imageNum) / 60) % 25;
    img_time ++;
    
    if((img_time / 60) % 25 == 0){
        img_amp += 1.5;
    }
}

void GraphicsPattern::draw_imageDistortion(float amp, float inputAmp, float threshold, float spread, float freq){
    ofPushMatrix();
    
    ofMesh particle;
    
    img_amp = ofVec3f(amp, amp, amp);
    
    ofDisableBlendMode();
    
    particle.setMode(OF_PRIMITIVE_POINTS);
    
    glPointSize(1);
    
    for(int x = 0; x < imgs[imageNum].getWidth(); x += 4){
        for(int y = 0; y < imgs[imageNum].getHeight(); y += 4){
            ofColor col = imgs[imageNum].getColor(x, y);
            
            float noise = ofNoise(x, y);
            
            drop = ofMap(col.getBrightness(), 0, 255, 1, 30);
            
            slip.x = ofMap(col.getBrightness(), 0, 255, -PI, PI);
            slip.y = ofMap(col.getBrightness(), 0, 255, -PI, PI);
            slip.z = ofMap(col.getBrightness(), 0, 255, 0, 8 * PI);
            
            wave.x = img_amp.x * (cos(TWO_PI * 8 * sin(img_time * freq - slip.x)) + sin( TWO_PI * 8 * sin(img_time * 0.0005 - slip.x)));
            wave.y = img_amp.y * (sin(TWO_PI * 8 * sin(img_time * freq - slip.y)) - cos(TWO_PI * 8 * sin(img_time * 0.0005 - slip.y)));
            wave.z = img_amp.z * cos(TWO_PI * cos(img_time * freq - slip.z));
            
            float d = ofDist(imgs[imageNum].getWidth()/2, imgs[imageNum].getHeight()/2, x, y);
            float distOffset = ofNormalize(d, 0, ofDist(0, 0, imgs[imageNum].getWidth()/2, imgs[imageNum].getHeight()/2)) + 1.0;
            
            pos.x = -(x - ofGetWidth()/2) * spread * distOffset;
            pos.y = -((ofGetHeight() - y) - ofGetHeight()/2) * spread * distOffset;
            pos.z = ofMap(col.getBrightness(), 0, 255, 0, 0) - 500;
            
            float dropY = pos.y;
            dropY -= drop;
            
            
            //音の大きさによって彩度を変化させる
            float saturation = ofMap(inputAmp, 0.0, 1.0, 120, 255);
            col.setSaturation(saturation);
            
            particle.addColor(col);
            particle.addVertex(pos + wave);
            
            
        }
    }
    
    ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    particle.draw();
    ofPopMatrix();
}

void GraphicsPattern::draw_imageDistortionB(float threshold, float amp){
    for (int x = 0; x < imgs[imageNum].getWidth(); x += 5) {
        for(int y = 0; y < imgs[imageNum].getHeight(); y += 5){
            int brightness = imgs[imageNum].getColor(x, y).getBrightness() / 2;
            
            ofColor col = ofColor(brightness, brightness, brightness);

            if (amp > threshold) {
                col  = ofColor(brightness, brightness, brightness);
            }else{
                col.setBrightness(0);
            }
            
            ofSetColor(col);
            float difference = ofMap(x, 0, imgs[imageNum].getWidth(), 0, TWO_PI * 4);
            ofDrawRectangle(x - ofGetWidth()/2, (-y + ofGetHeight()/2) + (img_time * 0.005) * sin(ofGetFrameNum() * 0.005 - difference), 10, 10);
            
        }
    }
}

//--------------------------------------------------------------

void GraphicsPattern::draw_redCross(float inputAmp, float speed){
    int lineH = 240;
    //ofEnableBlendMode(OF_BLENDMODE_SCREEN);
    
    for (int i = 0; i < lineH; i ++) {
        float amp = ofMap(abs(i - lineH), 0, lineH / 2, 50, 5);
        float freq = ofMap(abs(i - lineH), 0, lineH / 2, 10, 6);
        float bri = ofMap(abs(i - lineH / 2), 0, lineH / 2, 255, 0);
        float slip = ofMap(i, 0, lineH, 0.0, TWO_PI);
        
        draw_simpleWave(amp * inputAmp, ofGetFrameNum() * speed, PI * freq, slip, (i - lineH / 2) );
        
        
        ofPushMatrix();
        ofRotateZRad(HALF_PI);
        draw_simpleWave(amp * inputAmp, ofGetFrameNum() * speed, PI * freq, slip, (i - lineH / 2) );
        ofPopMatrix();
        
    }
    
    ofDisableBlendMode();
    
}

//--------------------------------------------------------------

void GraphicsPattern::init_fallingDown(){
    fall_pos.x = ofRandom(-ofGetWidth()/2, ofGetWidth()/2);
    fall_pos.y = ofRandom(-ofGetHeight()/2, ofGetHeight()/2);
    fall_pos.z = ofRandom(0, -300);
    
    isDisplay = true;
}

void GraphicsPattern::update_fallingDown(){
    if (isDisplay == false) {
        fall_pos.x = ofRandom(-ofGetWidth()/2, ofGetWidth()/2);
        fall_pos.y = ofGetHeight() / 2 - 50;
        fall_pos.z = ofRandom(0, -300);
        
        isDisplay = true;
    }
}

void GraphicsPattern::draw_fallingDown(float fall_speed){
    ofMesh point;
    
    point.setMode(OF_PRIMITIVE_LINES);
    
    point.addVertex(fall_pos);
    point.addVertex(ofVec3f(fall_pos.x, fall_pos.y + 50, fall_pos.z));
    
    if (isDisplay == true) {
        fall_pos.y -= fall_speed;
        point.draw();
    }
    
    if (fall_pos.y < -ofGetHeight() / 2 - 100) {
        isDisplay = false;
    }
    
}

//--------------------------------------------------------------
void GraphicsPattern::init_breaking(float initX, float initY, float size){
    debrisPos.x = initX;
    debrisPos.y = initY;
    debrisPos.z = 0;
    debrisSize = size;
    
    beginAngle = ofRandom(TWO_PI);
    
    isDebrisDisplay = true;
}

void GraphicsPattern::update_breaking(){
    float dist = ofDist(0, 0, debrisPos.x, debrisPos.y);
    float theta = atan2(debrisPos.y, debrisPos.x);
    
    float offset = ofMap(pow(dist, 2), 0, pow(ofDist(0, 0, ofGetWidth()/2, ofGetHeight()/2), 2), 10.0, 4.0);
   
    if(debrisPos.x < 0){
        debrisSp.x = -offset;
    }else{
        debrisSp.x = offset;
    }
    
    debrisSp.y = tan(theta) * debrisSp.x;
    debrisSp.y -= 5.0;
    
    debrisPos += debrisSp;
}

void GraphicsPattern::draw_breaking(){
    ofSetColor(208, 234, 237, 128);
    
    ofPushMatrix();
    ofTranslate(debrisPos);
    
    if (debrisPos.x > 0 && debrisPos.y > 0) {
        ofRotateXRad(ofGetFrameNum() * 0.07 - beginAngle);
        ofRotateYRad(ofGetFrameNum() * 0.07 - beginAngle);
    }else if (debrisPos.x > 0 && debrisPos.y < 0){
        ofRotateXRad(ofGetFrameNum() * 0.07 - beginAngle);
        ofRotateYRad(ofGetFrameNum() * (-0.07) - beginAngle);
    }else if (debrisPos.x < 0 && debrisPos.y > 0){
        ofRotateXRad(ofGetFrameNum() * (-0.07) - beginAngle);
        ofRotateYRad(ofGetFrameNum() * 0.07 - beginAngle);
    }else{
        ofRotateXRad(ofGetFrameNum() * (-0.07) - beginAngle);
        ofRotateYRad(ofGetFrameNum() * (-0.07) - beginAngle);
    }
    
    
    ofSetRectMode(OF_RECTMODE_CENTER);
    if (debrisPos.x < ofGetWidth()/2 && debrisPos.x > -ofGetWidth()/2
        && debrisPos.y < ofGetHeight()/2 && debrisPos.y > -ofGetHeight()/2) {
        ofDrawRectangle(0, 0, debrisSize, debrisSize);
    }else{
        isDebrisDisplay = false;
    }
    
    
    ofPopMatrix();
    
    ofSetRectMode(OF_RECTMODE_CORNER);
}
//--------------------------------------------------------------

void GraphicsPattern::init_activeMap(float initZ, float initNum, int initInterval){
    mapImg.load("images/map01.png");
    mapImg.resize(ofGetWidth(), ofGetHeight());
    
    for (int i = 0; i < 27; i ++) {
        string fileName = "images/map/map" + ofToString(i + 1) + ".png";
        mapImgs[i].load(fileName);
        mapImgs[i].resize(ofGetWidth(), ofGetHeight());
    }
    
    mapImgNum = initNum;
    globMapPos.z = initZ;
    mapImgInterval = initInterval;
    
}

void GraphicsPattern::update_activeMap(){
    if (globMapPos.z > (ofGetHeight()/2 + ofGetHeight())){
        mapImgNum += mapImgInterval;
        globMapPos.z = - (ofGetHeight()/2 + ofGetHeight() * mapImgInterval/2);
    }
    
    globMapPos.z += mapSpeed;
}

void GraphicsPattern::draw_activeMap(float amp, ofVec3f pos, float angle){
    ofPushMatrix();
    ofRotateXDeg(angle);
    globMapPos.x = pos.x;
    globMapPos.y = pos.y;
    ofTranslate(globMapPos);
    
    
    for (int x = 0; x < mapImg.getWidth(); x += 8) {
        for (int y = 0; y < mapImg.getHeight(); y += 8) {
            float noise = ofNoise(x, y);
            float differeence = ofMap(noise, 0.0, 1.0, -TWO_PI, TWO_PI);
            
            ofColor col = mapImgs[mapImgNum % 27].getColor(x, y);
            mapPos = ofVec3f(x - ofGetWidth()/2, 0, y - mapImg.getHeight()/2);
            
            int red = col.r;
            int green = col.g;
            int blue = col.b;
            
            float ampCalcY = ofMap(amp, 0.0, 0.5, 0.0, 180.0) * noise;
            if (col.r < 240 && col.g < 240 && col.b < 240) {
                //ofSetColor(200, 200, 200);
                //shapeMaker(8, mapPos, amp * 20);
                
                ofSetColor(col);
                ofEnableBlendMode(OF_BLENDMODE_SCREEN);
                ofEnableDepthTest();
                ofPushMatrix();
                ofTranslate(mapPos);
                ofRotateXRad(ofGetFrameNum() * 0.05 - differeence);
                ofRotateYRad(ofGetFrameNum() * 0.05 - differeence);
                ofDrawBox(0, 0, 0, 3, ampCalcY, 3);
                
                ofPopMatrix();
            }
            
            
        }
    }
    
    ofPopMatrix();
}
//--------------------------------------------------------------

void GraphicsPattern::init_face(){
    faceImg.load("images/b.JPG");
    faceImg.resize(ofGetWidth(), ofGetHeight() * 0.7);
}

void GraphicsPattern::draw_face(){
    for (int x = 0; x < faceImg.getWidth(); x += 18) {
        for (int y = 0; y < faceImg.getHeight(); y += 18) {
            ofColor col = faceImg.getColor(x, y);
            float difference = ofMap(x * y, 0, faceImg.getWidth() * faceImg.getHeight(),-PI, PI);
            float radius = ofMap(col.getBrightness(), 0, 255, 3, 8)/2 * cos(ofGetFrameNum() * 0.01 - PI) + ofMap(col.getBrightness(), 0, 255, 3, 8) * 1.5;
            
            
            col.setBrightness(100);
            ofSetColor(col);
            ofDrawEllipse(x - faceImg.getWidth()/2, -(y - faceImg.getHeight()/2), radius, radius);
            
            col.setBrightness(150);
            ofSetColor(col);
            shapeMaker(6, ofVec3f(x - faceImg.getWidth()/2, -(y - faceImg.getHeight()/2), 0), radius);
        }
    }
    
}

//--------------------------------------------------------------
void GraphicsPattern::draw_simpleWave(float amp, float speed, float width, float slip, float y_offset){
    ofMesh particleWave;
    
    particleWave.setMode(OF_PRIMITIVE_POINTS);
    for (int x = -ofGetWidth()/2; x < ofGetWidth()/2; x += 10) {
        float theta = ofMap(x, -ofGetWidth()/2, ofGetWidth()/2, -width/2, width);
        
        ofVec3f posA;
        posA.x = x;
        posA.y = amp * sin(theta - ofGetFrameNum() * 0.005 * speed - slip) + y_offset;
        posA.z = amp * cos(theta - ofGetFrameNum() * 0.005 * speed);
        
        ofVec3f posB;
        posB.x = x;
        posB.y = amp * cos(theta + ofGetFrameNum() * 0.005 * speed - slip) + y_offset;
        posB.z = amp * cos(theta - ofGetFrameNum() * (0.005 * speed) - PI);
        
        ofDrawLine(posA, posB);
        
    }
    
}
//--------------------------------------------------------------

void GraphicsPattern::shapeMaker(int n, ofVec3f pos, float radius){
    ofMesh vert;
    vert.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    for(float angle = 0; angle < TWO_PI; angle += TWO_PI/n){
        float x = radius * cos(angle) + pos.x;
        float y = radius * sin(angle) + pos.y;
        
        vert.addVertex(ofVec3f(x, y, pos.z));
    }
    
    vert.draw();
    
}
//--------------------------------------------------------------


void GraphicsPattern::drawWavingPlane(){
    ofMesh points;
    for (int x = 0; x < 40; x ++) {
        for (int y = 0; y < 30; y ++) {
            wavePoint[y + x * 30].x = (x - 20) * 100;
            wavePoint[y + x * 30].z = (y - 15) * 100;
            
            float thetaX = ofMap(wavePoint[y + x * 30].x, -2000, 2000, -TWO_PI, TWO_PI);
            float thetaY = ofMap(wavePoint[y + x * 30].z, -1500, 1500, -TWO_PI, TWO_PI);
            wavePoint[y + x * 30].y = 100 * (sin(thetaX - ofGetFrameNum() * 0.005) + cos(thetaY - ofGetFrameNum() * 0.005));
            
            points.addVertex(wavePoint[y + x * 30]);
            
        }
    }
    
    points.draw();
    
    for (int i = 0; i < 1200; i ++) {
        for(int j = 0; j < 1200; j ++){
            if (i != j) {
                float dist = ofDist(wavePoint[i].x, wavePoint[i].y, wavePoint[i].z, wavePoint[j].x, wavePoint[j].y, wavePoint[j].z);
                
                if (dist < 200) {
                    ofDrawLine(wavePoint[i], wavePoint[j]);
                }
            }
        }
    }
    
}

//--------------------------------------------------------------

void GraphicsPattern::draw_flash(float threShold, float amp){
    if (threShold < amp) {
        ofSetColor(255, 255, 255, 128);
        ofDrawRectangle(-ofGetWidth()/2, -ofGetHeight()/2, ofGetWidth(), ofGetHeight());
    }
}

//--------------------------------------------------------------

void GraphicsPattern::draw_rects(float n, ofColor col){
    ofPushMatrix();
    ofNoFill();
    ofSetColor(col);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofRotateZDeg(45);
    for (int i = 0; i < n; i ++) {
        ofDrawRectangle(0, 0, 750 + i * 30, 750 + i * 30);
    }
    ofPopMatrix();
    
    ofFill();
    ofSetRectMode(OF_RECTMODE_CORNER);
}

void GraphicsPattern::flashLight(float radius, float npow){
    for (int x = -ofGetWidth()/2; x < ofGetWidth()/2; x +=2) {
        for (int y = -ofGetHeight()/2; y < ofGetHeight()/2; y += 2) {
            float d = ofDist(0, 0, x, y);
            float alpha = ofMap(pow(d, npow), 0, pow(radius, npow), 255, 0);
            
            ofSetColor(alpha);
            ofDrawRectangle(x, y, 2, 2);
        }
    }
}

