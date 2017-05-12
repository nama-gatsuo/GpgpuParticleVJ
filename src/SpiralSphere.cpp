//
//  SpiralSphere.cpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/11.
//
//

#include "SpiralSphere.hpp"

void SpiralSphere::setup(){
    
    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int s = 0; s < sSize; s++) {
        for (int t = 0; t < tSize; t++) {
            for (int r = 0; r < rSize; r++) {
                
                mesh.addVertex(ofVec3f(0,0,0));
                mesh.addTexCoord(ofVec2f(t*rSize + r, s));
                mesh.addColor(ofFloatColor(0.3, 0.3, 0.1));
                
            }
        }
    }
    
    shader.load("shaders/Scene/SpiralSphere/shader");
    shader.begin();
    shader.setUniform1f("rSize", rSize);
    shader.setUniform1f("sSize", sSize);
    shader.setUniform1f("tSize", tSize);
    shader.end();
    
    for (int i = 0; i < spNum; i++) {
        ps[i].init();
    }
    
}
void SpiralSphere::update(float dt){
    
    for (int i = 0; i < spNum; i++) {
        ps[i].update(dt);
    }

}
void SpiralSphere::draw(float vol){
    
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    enablePointSprite();
    
    for (int i = 0; i < spNum; i++) {
        
        ofPushMatrix();
        ofRotateX(-i  * 36);
        ofRotateZ(i * 36);
        
        shader.begin();
        shader.setUniform1f("rInit", ps[i].rInit.getValue());
        shader.setUniform1f("rStep", ps[i].rStep.getValue());
        shader.setUniform1f("sStep", ps[i].sStep.getValue());
        shader.setUniform1f("tStep", ps[i].tStep.getValue());

        mesh.draw();
        
        shader.end();
        ofPopMatrix();
    }
    ofDisableBlendMode();

}
void SpiralSphere::randomize(){
    for (int i = 0; i < spNum; i++) {
        ps[i].randomize();
    }
}
void SpiralSphere::setParam(int ch, float val){}
