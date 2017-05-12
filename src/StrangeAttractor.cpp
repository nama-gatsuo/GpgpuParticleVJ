//
//  StrangeAttractor.cpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#include "StrangeAttractor.hpp"

void StrangeAttractor::setup(){
    
    shader.load("shaders/Scene/StrangeAttractor/shader");
    
    shader.begin();
    shader.setUniform1f("scale", 150);
    shader.end();
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < num; i++) {
        mesh.addVertex(ofVec3f(ofRandom(-1.0,1.0),ofRandom(-1.0,1.0),ofRandom(-1.0,1.0)));
        mesh.addColor(ofFloatColor(0.33, 0.07, 0.28));
    }
    
    for (int i = 0; i < 6; i++) {
        ap[i].setSpeed(0.01);
    }
}
void StrangeAttractor::update(float dt){
    
    for (int i = 0; i < 6; i++) {
        ap[i].update(dt);
        params[i] = ap[i].getValue();
    }
}
void StrangeAttractor::draw(float vol){
    
    enablePointSprite();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    shader.begin();
    shader.setUniform1f("alpha", vol);
    shader.setUniform1fv("params", params, 6);
    mesh.draw();
    shader.end();
    
    ofDisableBlendMode();
}

void StrangeAttractor::randomize(){
    for (int i = 0; i < 3; i++) {
        ap[i * 2].to(ofRandom(1.0));
    }
    float coin = ofRandom(1.0);
    if (coin < 0.1) {
        for(int i = 0; i < 6; i++) {
            ap[i].setSpeed(0.001);
        }
    } else {
        for(int i = 0; i < 6; i++) {
            ap[i].setSpeed(0.01);
        }
    }
}
void StrangeAttractor::setParam(int ch, float val){
    
    ap[ch * 2 + 1].to(val / 128.0);
    
}
