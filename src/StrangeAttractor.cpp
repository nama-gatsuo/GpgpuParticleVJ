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
    shader.setUniform1f("scale", 80.);
    shader.end();
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < num; i++) {
        mesh.addVertex(ofVec3f(ofRandom(-1.0,1.0),ofRandom(-1.0,1.0),ofRandom(-1.0,1.0)));
        mesh.addColor(ofFloatColor(0.33 + ofRandom(-0.3, 0.3), 0.07, 0.28));
    }
    
    for (int i = 0; i < 6; i++) {
        ap[i].setSpeed(0.03);
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
    shader.setUniform1f("alpha", vol+0.1);
    shader.setUniform1fv("params", params, 6);
    mesh.draw();
    shader.end();
    
    ofDisableBlendMode();
}

void StrangeAttractor::randomize(){
    for (int i = 0; i < 6; i++) {
        if (ofRandom(1.0) < 0.4) ap[i].to(ofRandom(1.0));
    }
    
}
void StrangeAttractor::setParam(int ch, float val){
    
//    ap[ch * 2 + 1].to(val / 128.0);
    
}
