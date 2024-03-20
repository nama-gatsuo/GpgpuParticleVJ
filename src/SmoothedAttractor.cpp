//
//  SmoothedAttractor.cpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#include "SmoothedAttractor.hpp"

void SmoothedAttractor::setup(){
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    for(int x = 0; x < wSize; x++){
        for(int y = 0; y < hSize; y++){
            mesh.addVertex(ofVec3f(ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1)));
            mesh.addColor(ofFloatColor(ofRandom(1.0, 0.3), 0.5));
            mesh.addTexCoord(ofVec2f(x, y));
        }
    }
    
    vector<float> pos(wSize * hSize * 4);
    vector<float> vel(wSize * hSize * 4);
    
    for (int w = 0; w < wSize; w++) {
        for (int h = 0; h < hSize; h++) {
            
            int i = h * wSize + w;
            
            pos[i*4] = ofRandom(-1.0, 1.0);
            pos[i*4 + 1] = ofRandom(-1.0, 1.0);
            pos[i*4 + 2] = ofRandom(-1.0, 1.0);
            pos[i * 4 + 3] = 0;

            
            ofVec3f v = ofVec3f(0.);
            
            vel[i*4] = v.x;
            vel[i*4 + 1] = v.y;
            vel[i*4 + 2] = v.z;
            vel[i * 4 + 3] = 0;
            
        }
    }
    
    pp.allocate(wSize, hSize);
    pp.src->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    
    pp.src->getTexture(1).loadData(vel.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(1).loadData(vel.data(), wSize, hSize, GL_RGBA);
    
    updateData.load("shaders/Common/passThru.vert", "shaders/Scene/SmoothedAttractor/updateData.frag");
    updateRenderer.load("shaders/Scene/SmoothedAttractor/render");
    
    r.to(0.1);
    f.to(0.1);
    aPos.setSpeed(0.01);
}
void SmoothedAttractor::update(float dt){
    
    r.update(dt);
    f.update(dt);
    aPos.update(dt);
    
    pp.dst->begin();
    ofClear(0);
    pp.dst->activateAllDrawBuffers();
    
    updateData.begin();
    updateData.setUniformTexture("posData", pp.src->getTexture(0), 0);
    updateData.setUniformTexture("velData", pp.src->getTexture(1), 1);
    updateData.setUniform3f("aPos", aPos);
    updateData.setUniform1f("ur", r.getValue());
    updateData.setUniform1f("dt", dt);
    
    pp.src->draw(0, 0);
    updateData.end();
    
    pp.dst->end();
    
    pp.swap();
    
}
void SmoothedAttractor::draw(float vol){
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    enablePointSprite();
    ofClear(0);
    
    updateRenderer.begin();
    updateRenderer.setUniform1f("alpha", vol);
    updateRenderer.setUniform3f("aPos", aPos);
    updateRenderer.setUniformTexture("posData", pp.dst->getTexture(0), 0);
    
    mesh.draw();
    
    updateRenderer.end();
    ofDisableBlendMode();
}

void SmoothedAttractor::randomize(){
    
    aPos.to(ofPoint(ofRandom(-7.0, 7.0), ofRandom(-7.0, 7.0), ofRandom(-7.0, 7.0)));
    
}
void SmoothedAttractor::setParam(int ch, float val){
    
    switch (ch) {
        case 0: r.to(val / 128 * 0.1 + 0.04); break;
        case 1: f.to(val / 128 * 0.1 + 0.001); break;
        default: break;
    }
}
