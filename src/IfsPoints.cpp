//
//  IfsPoints.cpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#include "IfsPoints.hpp"
void IfsPoints::setup(){
    shader.load("shaders/Scene/IfsPoints/shader");

    shader.begin();
    shader.setUniform1f("scale", 200);
    shader.end();

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < num; i++) {
        mesh.addVertex(ofVec3f(ofRandom(-1.0,1.0),ofRandom(-1.0,1.0),ofRandom(-1.0,1.0)));
        mesh.addColor(ofFloatColor(0.03, 0.24, 0.43 + ofRandom(-1.0, 1.0)));
    }
}

void IfsPoints::update(float dt){
    ap[0].update(dt);
    ap[1].update(dt);
}

void IfsPoints::draw(float volume){
    enablePointSprite();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    shader.begin();
    shader.setUniform1f("alpha", volume + 0.05);
    shader.setUniform1f("w0", ap[0].weight);
    shader.setUniformMatrix4f("mat0", ap[0].mat);
    shader.setUniform1f("w1", ap[1].weight);
    shader.setUniformMatrix4f("mat1", ap[1].mat);
    
    mesh.draw();
    
    shader.end();
    
    ofDisableBlendMode();
}

void IfsPoints::randomize(){
    ap[1].r.to(ofPoint(ofRandom(180.0), ofRandom(30.0), ofRandom(180.0)));
    ap[1].s.to(ofPoint(ofRandom(1.0)+0.3, ofRandom(1.0)+0.3, ofRandom(1.0)+0.3));
    ap[1].t.to(ofPoint(ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), ofRandom(0.5, 0.5)));
    
    ap[0].s.to(ofPoint(ofRandom(1.0)+0.3, ofRandom(1.0)+0.3, ofRandom(1.0)+0.3));
    ap[0].t.to(ofPoint(ofRandom(0.5, 0.5), ofRandom(0.5, 0.5), ofRandom(0.5, 0.5)));
    
    float coin = ofRandom(1.0);
    if (coin < 0.1) {
        ap[1].r.setSpeed(0.005);
        ap[1].s.setSpeed(0.005);
        ap[1].t.setSpeed(0.005);
        ap[0].r.setSpeed(0.005);
        ap[0].s.setSpeed(0.005);
        ap[0].t.setSpeed(0.005);
    } else {
        ap[1].r.setSpeed(0.02);
        ap[1].s.setSpeed(0.02);
        ap[1].t.setSpeed(0.02);
        ap[0].r.setSpeed(0.02);
        ap[0].s.setSpeed(0.02);
        ap[0].t.setSpeed(0.02);
    }
    
}

void IfsPoints::setParam(int ch, float val){
    
    switch (ch) {
        case 0: ap[0].r.to(ofPoint(val / 128 * 180.0, ap[0].r.y, ap[0].r.z)); break;
        case 1: ap[0].r.to(ofPoint(ap[0].r.x, val / 128 * 180.0, ap[0].r.z)); break;
        case 2: ap[0].r.to(ofPoint(ap[0].r.x, ap[0].r.y, val / 128 * 180.0)); break;
        default: break;
    }
    
}
