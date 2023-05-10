//
//  PostEffect.cpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/09.
//
//

#include "PostEffect.hpp"

void PostEffect::setup(){
    
    ofFbo::Settings s;
    s.width = ofGetWidth();
    s.height = ofGetHeight();
    s.internalformat = GL_RGB32F;
    
    base.allocate(s);
    
    composite0.allocate(ofGetWidth(), ofGetHeight(), GL_RGB8);
    composite1.allocate(ofGetWidth(), ofGetHeight(), GL_RGB8);
    
    complexConv.load("shaders/Common/passThru.vert", "shaders/PostEffect/ComplexConv.frag");
    complexConv.begin();
    complexConv.setUniform2f("res", ofGetWidth(), ofGetHeight());
    complexConv.end();
    
    mirrorConv.load("shaders/Common/passThru.vert", "shaders/PostEffect/MirrorConv.frag");
    mirrorConv.begin();
    mirrorConv.setUniform2f("res", ofGetWidth(), ofGetHeight());
    mirrorConv.end();
    
    negaConv.load("shaders/Common/passThru.vert", "shaders/PostEffect/NegativeConv.frag");
    gammaConv.load("shaders/Common/passThru.vert", "shaders/PostEffect/GammaConv.frag");
}

void PostEffect::begin(){
    
    base.begin();
    ofClear(0,0);
    
}
void PostEffect::end(){
    
    base.end();
    
    composite0.begin();
    ofClear(0,0);
    ofSetColor(255);
    
    switch (mode) {
        case 0: break;
        case 1: {
            mirrorConv.begin();
            mirrorConv.setUniform1i("mode", 0); // vertical
            break;
        }
        case 2: {
            mirrorConv.begin();
            mirrorConv.setUniform1i("mode", 1); // horizontal
            break;
        }
        case 3: {
            complexConv.begin();
            complexConv.setUniform1f("s1", params[0]);
            complexConv.setUniform1f("s2", params[1]);
            break;
        }
        default: break;
    }
    
    base.draw(0,0);
    
    switch (mode) {
        case 0: break;
        case 1: complexConv.end(); break;
        case 2: mirrorConv.end(); break;
        case 3: mirrorConv.end(); break;
        default: break;
    }
    composite0.end();
    
    
}
void PostEffect::applyCorrection(){
    
    composite1.begin();
    ofClear(0);

    ofSetColor(255);
    
    gammaConv.begin();
    gammaConv.setUniform1f("isNega", bNega ? 1.0f : 0.0f);

    composite0.draw(0,0);
    
    gammaConv.end();
    composite1.end();

}

void PostEffect::bang() {

    float coin = ofRandom(1.0);
    
    if (coin < 0.1) bNega = true;
    else bNega = false;
    
    coin = ofRandom(1.0);
    
    if (coin < 0.4) mode = 0;
    else if (coin < 0.6) mode = 2;
    else if (coin < 0.8) mode = 3;
    else mode = 3;
    
}

void PostEffect::setMode(int mode) { this->mode = mode; }
void PostEffect::setParam(int ch, float val) { params[ch] = val; }

