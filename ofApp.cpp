#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFullscreen(true);
    shader.load("", "shader/4redcross.frag");
    
    fft.setup(16384);
    
    ofSetBackgroundColor(0);
    //ofSetBackgroundColor(235);
    
    receiver.setup(PORT);
    
    for (int i=0; i < 3; i++) {
        shapeGrid[i].init_shapeGrid(i * 330);
    }
    
    for (int i = 0; i < 2048; i ++) {
        rains[i].init_fallingDown();
    }
    
    graphics.init_imageDistortion();
    graphics.init_fallingDown();
    graphics.init_Rings();
    graphics.init_face();
    
    modeActivator(1);
    
    fbo.allocate(ofGetWidth(), ofGetHeight());
    
    ofTrueTypeFont::setGlobalDpi(72);
    
    ofTrueTypeFontSettings settings("HanaMinA.ttf", 44);
    settings.addRanges(ofAlphabet::Emoji);
    settings.addRanges(ofAlphabet::Japanese);
    settings.addRange(ofUnicode::Space);
    settings.addRange(ofUnicode::Hiragana);
    settings.addRange(ofUnicode::Katakana);
    
    hanamin.load(settings);
    
    for (int x = 0; x < glassW; x ++) {
        for (int y = 0; y < glassH; y ++) {
            float Dsize = ofRandom(10, 25);
            glass[y + x * glassH].init_breaking((x - glassW/2) * debrisSize, (y - glassH/2) * debrisSize, Dsize);
        }
    }
    
    for (int i = 0; i < mapNum; i++) {
        maps[i].init_activeMap(-i * ofGetHeight(), i, mapNum);
    }
    
    ofSoundStreamListDevices();
}

//--------------------------------------------------------------
void ofApp::modeActivator(int num){
    for(int i = 0; i < 8; i++){
        mode[i] = false;
    }
    
    ofDisableBlendMode();
    mode[num - 1] = true;
}

//--------------------------------------------------------------
void ofApp::update(){
    fft.update();
    
    ampAvg = 0;
    
    for(int i = 0; i < 1024; i ++){
        ampAvg += fft.getBins()[i];
    }
    
    ampAvg = ampAvg / 1024;
    
    while(receiver.hasWaitingMessages()){
        ofxOscMessage msg;
        receiver.getNextMessage(msg);
        
        if (msg.getAddress() == "/osc"){
            nChan = msg.getArgAsInt32(0);
            nVal = msg.getArgAsInt32(1);
            nPitch = msg.getArgAsInt32(2);
        }
    }
    
    switch (nPitch) {
        case 36:
            modeActivator(1);
            
            flowerPos.z = 0;
            for (int x = 0; x < glassW; x ++) {
                for (int y = 0; y < glassH; y ++) {
                    float Dsize = ofRandom(10, 25);
                    glass[y + x * glassH].init_breaking((x - glassW/2) * debrisSize, (y - glassH/2) * debrisSize, Dsize);
                }
            }
            
            break;
            
        case 37:
            modeActivator(2);
            break;
            
        case 38:
            modeActivator(3);
            break;
            
        case 39:
            modeActivator(4);
            graphics.img_time = 0;
            break;
            
        case 40:
            modeActivator(5);
            graphics.img_time = 0;
            break;
            
        case 41:
            modeActivator(6);
            break;
            
        case 42:
            modeActivator(7);
            break;
            
        case 43:
            modeActivator(8);
            break;
            
        case 0:
            switch (nChan) {
                case 1:
                    
                    if (mode[0] || mode[1] || mode[2]) {
                        speed = ofMap(nVal, 0, 127, 0.01, 0.5);
                    }
                    
                    if (mode[4]) {
                        speed = ofMap(nVal, 0, 127, 1.0, 100.0);
                    }
                    
                    if (mode[5]) {
                        imgAmp = ofMap(nVal, 0, 127, 0, 1000);
                    }
                    
                    if (mode[6]) {
                        cityPos.z = ofMap(nVal, 0, 127, -1000, 1000);
                    }
                    
                    break;
                    
                case 2:
                    
                    if (mode[0] || mode[1] || mode[2]) {
                        rotSpeed = ofMap(nVal, 0, 127, 0, 100);
                    }
                    
                    if (mode[5]) {
                        imgSpread = ofMap(nVal, 0, 127, -5.0, 5.0);
                    }
                    
                    if (mode[6]){
                        for(int i = 0; i < mapNum; i ++){
                            maps[i].mapSpeed = ofMap(nVal, 0, 127, 0.0, 20.0);
                        }
                    }
                    
                    size = ofMap(nVal, 0, 127, 0.1, 5);
                    break;
                    
                case 3:
                    if (mode[4]) {
                        fallSpeed = ofMap(nVal, 0, 127, 0.0, 30.0);
                    }
                    
                    if (mode[6]){
                        cityPos.y = ofMap(nVal, 0, 127, -1000, 1000);
                    }
                    
                    break;
                    
                case 4:
                    if (mode[0] || mode[1] || mode[2]) {
                        beltY = ofMap(nVal, 0, 127, 0, ofGetWidth()/2);
                        crossZ = ofMap(nVal, 0, 127, -500, 200);
                    }
                    
                    if (mode[4]) {
                        rainAmount = ofMap(nVal, 0, 127, 0, 2048);
                    }
                    
                    if (mode[6]){
                        cityAngle = ofMap(nVal, 0, 127, 30, 90);
                    }
                    
                    
                    break;
                    
                case 5:
                    if (mode[0] || mode[1] || mode[2] || mode[5]){
                        freq = ofMap(nVal, 0, 127, 0.00001, 0.05);
                    }
                    
                    break;
                    
                case 6:
                    if(mode[0] || mode[1] || mode[2]){
                        crossRotX = ofMap(nVal, 0, 127, 0, TWO_PI);
                        crossRotY = ofMap(nVal, 0, 127, 0, TWO_PI);
                    }
                    break;
                    
                case 7:
                    ampMax = ofMap(nVal, 0, 127, 1.0, 50.0);
                    break;
                    
                case 8:
                    threshold = ofMap(nVal, 0, 127, 0.0, 0.4);
                    break;
            }
            
            break;
            
            
    }
    
    if (mode[4]) {
        for (int i = 0; i < 2048; i ++) {
            rains[i].update_fallingDown();
        }
    }
    
    if (mode[0] || mode[1] || mode[2]) {
        graphics.update_Rings();
        flowerPos.z -= 2.0;
        rot += rotSpeed;
    }
    
    if (mode[0]) {
        for (int i = 0; i < 42000; i ++) {
            glass[i].update_breaking();
        }
    }
    
    if (mode[4] || mode[5]) {
        graphics.update_imageDistortion();
    }
    
    if (mode[6]) {
        for (int i = 0; i < mapNum; i++) {
            maps[i].update_activeMap();
        }
    }
    
    graphics.update_imageDistortion();
    
    
    
    

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    float w = ofMap(ampAvg, 0.0, 0.5, 0, ofGetWidth());
    
    //ofDrawRectangle(0, ofGetHeight()/2, w, 50);
    
    cam.begin();
    float lineMax = ofMap(ampAvg, 0, 0.6, 1, 30);
    
    if (mode[0]) {
        ofClear(208, 234, 237);
        float sizeAmp = ofMap(ampAvg, 0, 0.8, 1.0, ampMax);
        
        ofPushMatrix();
        ofTranslate(0, 0, crossZ);
        
        ofRotateXRad(crossRotX);
        ofRotateYRad(crossRotY);
        ofSetColor(150, 30, 30, 128);
       // ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        graphics.draw_redCross(sizeAmp, freq);
        ofPopMatrix();
        
        if(ampAvg > 0.3){
            float plusOffset = ofMap(ampAvg, 0.0, 0.8, 0.5, 10.0);
            rotSpeed += plusOffset;
        }
        
        ofPushMatrix();
        ofTranslate(0, 0, flowerPos.z);
        
        ofPushMatrix();
        ofRotateXRad((ofGetFrameNum() + rot) * 0.001);
        ofRotateYRad((ofGetFrameNum() + rot) * 0.001);
        ofRotateZRad((ofGetFrameNum() + rot) * 0.001);
        
        
        for(int i = 0; i < 1024; i += 2){
            float col = ofMap(i, 0, 1024, 200, 255);
            
            ofSetColor(27, 140 , 27, 255  - col);
            float radius = ofMap(i, 0, 1024, 0, 2 * ofGetWidth());
            
            ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
            graphics.draw_Rings(6, radius, speed, 2 * ofGetWidth(), sizeAmp);
            
        }
        ofPopMatrix();
        
        ofPopMatrix();
        
        
        //ofDisableBlendMode();
        for (int i = 0; i < 42000; i ++) {
            if (glass[i].isDebrisDisplay){
                glass[i].draw_breaking();
            }
        }
        
    }
    
    if (mode[1]) {
        ofClear(124, 40, 40);
        float sizeAmp = ofMap(ampAvg, 0, 0.8, 1.0, ampMax);
        
        ofPushMatrix();
        ofTranslate(0, 0, crossZ);
        
        ofRotateXRad(crossRotX);
        ofRotateYRad(crossRotY);
        ofSetColor(208, 234, 245, 128);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        graphics.draw_redCross(sizeAmp, freq);
        ofPopMatrix();
        
        if(ampAvg > 0.3){
            float plusOffset = ofMap(ampAvg, 0.0, 0.8, 0.5, 10.0);
            rotSpeed += plusOffset;
        }
        
        ofPushMatrix();
        ofTranslate(0, 0, flowerPos.z);
        
        ofPushMatrix();
        ofRotateXRad((ofGetFrameNum() + rot) * 0.001);
        ofRotateYRad((ofGetFrameNum() + rot) * 0.001);
        ofRotateZRad((ofGetFrameNum() + rot) * 0.001);
        
        
        
        for(int i = 0; i < 1024; i += 2){
            float col = ofMap(i, 0, 1024, 200, 255);
            
            ofSetColor(35, 214, 35, 255  - col);
            ofSetColor(48, col/2, 183, 255  - col);
            float radius = ofMap(i, 0, 1024, 0, 2 * ofGetWidth());
            
            //ofEnableBlendMode(OF_BLENDMODE_SUBTRACT);
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
            graphics.draw_Rings(10, radius, speed, 2 * ofGetWidth(), sizeAmp);
            
        }
        ofPopMatrix();
        
        ofPopMatrix();
    }
    
    if (mode[2]) {
        ofClear(0);
       // graphics.flashLight(ofGetWidth()/4, 0.25);
        shader.begin();
        shader.setUniform1f("u_time", ofGetElapsedTimef());
        shader.setUniform2f("u_resolution", ofGetWidth(), ofGetHeight());
        ofPushMatrix();
        ofTranslate(0, 0, crossZ);
        
        ofRotateXRad(crossRotX);
        ofRotateYRad(crossRotY);
        graphics.draw_redCross(1.0, freq);
        ofPopMatrix();
        shader.end();
        
        if(ampAvg > 0.3){
            float plusOffset = ofMap(ampAvg, 0.0, 0.8, 0.5, 10.0);
            rotSpeed += plusOffset;
        }
        
        ofPushMatrix();
        
        ofTranslate(flowerPos);
        
        ofPushMatrix();
        ofRotateXRad((ofGetFrameNum() + rot) * 0.001);
        ofRotateYRad((ofGetFrameNum() + rot) * 0.001);
        ofRotateZRad((ofGetFrameNum() + rot) * 0.001);
        
        float sizeAmp = ofMap(ampAvg, 0, 0.8, 1.0, ampMax);
        float colAmp = ofMap(ampAvg, 0, 0.8, 0, 255);
        
        for(int i = 0; i < 1024; i += 2){
            float col = ofMap(i, 0, 1024, 0, 255);
            ofSetColor(col - ampAvg, 0, (255 - colAmp) - col );
            float radius = ofMap(i, 0, 1024, 0, 2 * ofGetWidth());
            
            ofEnableBlendMode(OF_BLENDMODE_SCREEN);
            graphics.draw_Rings(18, radius, speed, 2 * ofGetWidth(), sizeAmp);
        }
        ofPopMatrix();
        
        ofPopMatrix();
        
        if (ampAvg > threshold) {
            //graphics.draw_rects(4, ofColor(255, 255, 255));
        }
        
    }
    
    //悲壮
    //第一楽章
    if (mode[4]) {
        ofClear(66);
        graphics.draw_imageDistortionB(threshold, ampAvg);
        
        for (int i = 0; i < 2048; i ++) {
            float slip = ofMap(i, 0, 1024, -PI * 10, PI * 10);
            ofColor col = ofColor(30, 30, 230);
            col.setBrightness(127 * cos(ofGetFrameNum() * 0.05 - slip));
            
            ofSetColor(col);
            
            if (i < rainAmount) {
                rains[i].draw_fallingDown(fallSpeed);
            }
            
        }
        
        ofSetColor(255);
        ofPushMatrix();
        ofTranslate(0, -200);
        ofEnableBlendMode(OF_BLENDMODE_SCREEN);
        graphics.drawWavingPlane();
        ofPopMatrix();

    }
    
    //第二楽章
    if (mode[5]) {
        ofSetBackgroundColor(0);
        //graphics.draw_flash(threshold, ampAvg);
        float sizeAmp = ofMap(ampAvg, 0, 0.6, 1.0, 3.0);
        float inputFreq = ofMap(ampAvg, 0.0, threshold, 0.00001, 0.00007);
        graphics.draw_imageDistortion(imgAmp, ampAvg, threshold, imgSpread, inputFreq);
        
    }
    
    //第三楽章
    if (mode[6]) {
        //ofClear(124, 40, 40);
        ofClear(40, 70, 64);
        
        graphics.draw_face();
        
        for (int i = 0; i < mapNum; i++) {
            maps[i].draw_activeMap(ampAvg, cityPos, cityAngle);
        }
        
        
    }
    
    if (mode[7]) {
        
    }
    
    
    cam.end();
    ofDisableDepthTest();
    ofDisableBlendMode();
    
    //caption belt
    ofFill();
    ofSetColor(0);
    ofSetRectMode(OF_RECTMODE_CENTER);
    ofDrawRectangle(ofGetWidth()/2, 75, ofGetWidth(), 150 + beltY);
    ofDrawRectangle(ofGetWidth()/2 , ofGetHeight() - 75, ofGetWidth(), 150 + beltY);
    
    ofSetRectMode(OF_RECTMODE_CORNER);
    
    //ofSetColor(255);
    //hanamin.drawString("本能", ofGetWidth()/2 - 20, ofGetHeight() - 75);
    
    
}
//--------------------------------------------------------------

void ofApp::drawFlower(){
    
}

//--------------------------------------------------------------
void ofApp::dumpOSC(ofxOscMessage msg){
    string msg_string;
    msg_string = msg.getAddress();
    
    for (int i = 0; i < msg.getNumArgs(); i++) {
        msg_string += " ";
        
        if(msg.getArgType(i) == OFXOSC_TYPE_INT32){
            msg_string += ofToString(msg.getArgAsInt32(i));
        }else if(msg.getArgType(i) == OFXOSC_TYPE_FLOAT){
            msg_string += ofToString(msg.getArgAsFloat(i));
        }else if(msg.getArgType(i) == OFXOSC_TYPE_STRING){
            msg_string += msg.getArgAsString(i);
        }
    }
    
    cout << msg_string << endl;
}

//--------------------------------------------------------------
void ofApp::plot(vector<float>& buffer, float scale){
    ofNoFill();
    int n = MIN(1024, buffer.size());
    
    ofDrawRectangle(0, 0, n, scale);
    ofPushMatrix();
    ofTranslate(0, scale);
    ofScale(1, -scale);
    ofBeginShape();
    for (int i = 0; i < n; i++) {
        ofVertex(i, buffer[i]);
    }
    ofEndShape();
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case '1':
            modeActivator(1);
            break;
            
        case '2':
            modeActivator(2);
            break;
            
        case '3':
            modeActivator(3);
            break;
            
        case '4':
            modeActivator(4);
            break;
            
        case '5':
            modeActivator(5);
            break;
            
        case '6':
            modeActivator(6);
            break;
            
        case '7':
            modeActivator(7);
            break;
            
        case '8':
            modeActivator(8);
            
            break;
            
        case '9':
            flowerPos.z = 0;
            for (int x = 0; x < glassW; x ++) {
                for (int y = 0; y < glassH; y ++) {
                    float Dsize = ofRandom(10, 25);
                    glass[y + x * glassH].init_breaking((x - glassW/2) * debrisSize, (y - glassH/2) * debrisSize, Dsize);
                }
            }
            
            graphics.init_imageDistortion();
            
            break;
            
        case ' ':
            debugMode = !debugMode;
            break;
            
            
        default:
            break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
