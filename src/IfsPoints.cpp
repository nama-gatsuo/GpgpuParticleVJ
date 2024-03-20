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
    shader.setUniform1f("scale", 100.);
    shader.end();

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < num; i++) {
        mesh.addVertex(ofVec3f(ofRandom(-1.0,1.0),ofRandom(-1.0,1.0),ofRandom(-1.0,1.0)));
        mesh.addColor(ofFloatColor(ofRandom(1.0, 0.3), 1.0));
    }
}

void IfsPoints::update(float dt){
    ap[0].update(dt);
    ap[1].update(dt);
}

void IfsPoints::draw(float vol){
    enablePointSprite();
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    shader.begin();
    shader.setUniform1f("alpha", vol + 0.3);
    shader.setUniform1f("w0", ap[0].weight);
    shader.setUniformMatrix4f("mat0", ap[0].mat);
    shader.setUniform1f("w1", ap[1].weight);
    shader.setUniformMatrix4f("mat1", ap[1].mat);
    
    mesh.draw();
    
    shader.end();
    
    ofDisableBlendMode();
}

void IfsPoints::randomize(){
    ap[1].r.to(ofPoint(ofRandom(-180.0, 180.0), ofRandom(-180.0, 180.0), ofRandom(-180.0, 180.0)));
    ap[1].s.to(ofPoint(ofRandom(0.8)+0.4, ofRandom(0.8)+0.4, ofRandom(0.8)+0.4));
    ap[1].t.to(ofPoint(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
    
    ap[0].r.to(ofPoint(ofRandom(-180.0, 180.0), ofRandom(-180.0, 180.0), ofRandom(-180.0, 180.0)));
    ap[0].s.to(ofPoint(ofRandom(0.8) + 0.4, ofRandom(0.8) + 0.4, ofRandom(0.8) + 0.4));
    ap[0].t.to(ofPoint(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1)));
}

void IfsPoints::setParam(int ch, float val){
    
    switch (ch) {
        case 0: ap[0].r.to(ofPoint(val / 128 * 180.0, ap[0].r.y, ap[0].r.z)); break;
        case 1: ap[0].r.to(ofPoint(ap[0].r.x, val / 128 * 180.0, ap[0].r.z)); break;
        case 2: ap[0].r.to(ofPoint(ap[0].r.x, ap[0].r.y, val / 128 * 180.0)); break;
        default: break;
    }
    
}
