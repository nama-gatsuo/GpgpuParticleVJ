//
//  GalaxyCollision.cpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#include "GalaxyCollision.hpp"

void GalaxyCollision::setup(){

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int h = 0; h < hSize; h++) {
        
        ofFloatColor c;
        if (h < hSize / 2) {
            c = ofFloatColor(0.12, 0.3, 0.4);
        } else {
            c = ofFloatColor(0.36, 0,4, 0.1);
        }
        
        for (int w = 0; w < wSize; w++) {
            mesh.addVertex(ofVec3f(0,0,0));
            mesh.addTexCoord(ofVec2f(w, h));
            mesh.addColor(c);
        }
    }
    
    createData.load("shaders/Common/passThru.vert", "shaders/Scene/GalaxyCollision/createData.frag");
    updateData.load("shaders/Common/passThru.vert", "shaders/Scene/GalaxyCollision/updateData.frag");
    updateRenderer.load("shaders/Scene/GalaxyCollision/render");
    
    pp.allocate(wSize, hSize);
    
    randomize();
}

void GalaxyCollision::update(float dt){
    
    dt *= 20.0;
    
    float d = p1.distance(p0);
    if (d > 1.0) {
        a0 = (p1 - p0).normalize() * GM / d;
        a1 = - a0 * massRatio;
        
        v0 += a0 * dt;
        v1 += a1 * dt;
        
        p0 += v0 * dt;
        p1 += v1 * dt;
    }
    
    pp.dst->begin();
    ofClear(0);
    pp.dst->activateAllDrawBuffers();
    
    updateData.begin();
    updateData.setUniformTexture("posData", pp.src->getTexture(0), 0);
    updateData.setUniformTexture("velData", pp.src->getTexture(1), 1);
    updateData.setUniform1f("GM", GM);
    updateData.setUniform1f("dt", dt);
    updateData.setUniform1f("massRatio", massRatio);
    updateData.setUniform3f("p0", p0);
    updateData.setUniform3f("p1", p1);
    
    pp.src->draw(0,0);
    
    updateData.end();
    pp.dst->end();
    
    pp.swap();
    
}

void GalaxyCollision::draw(float vol){

    enablePointSprite();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    
    updateRenderer.begin();
    updateRenderer.setUniformTexture("posData", pp.dst->getTexture(0), 0);
    
    mesh.draw();
    
    updateRenderer.end();
    
    ofDisableBlendMode();

}

void GalaxyCollision::randomize(){
    
    // global param
    GM = ofRandom(1.0, 10.0);
    massRatio = ofRandom(0.3, 1.0);
    distance = ofRandom(20, 200);
    bulgeRad[0] = ofRandom(50, 300);
    diskRad[0] = ofRandom(200, 500);
    bulgeRad[1] = ofRandom(50, 300);
    diskRad[1] = ofRandom(200, 500);
    speed = sqrt(GM / 2.0);
    
    angleX[0] = ofRandom(-45, 45);
    angleX[1] = ofRandom(-45, 45);
    angleY[0] = ofRandom(-45, 45);
    angleY[1] = ofRandom(-45, 45);
    
    p0 = ofVec3f(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0)).normalize() * distance;
    v0 = ofVec3f(speed, 0.0, 0.0);
    p1 = (- massRatio) * p0;
    v1 = (- massRatio) * v0;
    
    // create initial value in GPU
    pp.dst->begin();
    ofClear(0);
    pp.dst->activateAllDrawBuffers();
    
    createData.begin();
    createData.setUniform1f("GM", GM);
    updateData.setUniform2f("res", 1.0 * wSize, 1.0 * hSize);
    createData.setUniform1f("massRatio", massRatio);
    createData.setUniform3f("p0", p0.x, p0.y, p0.z);
    createData.setUniform3f("p1", p1.x, p1.y, p1.z);
    createData.setUniform3f("v0", v0.x, v0.y, v0.z);
    createData.setUniform3f("v1", v1.x, v1.y, v1.z);
    createData.setUniform1fv("bulgeRad", bulgeRad, 2);
    createData.setUniform1fv("diskRad", diskRad, 2);
    createData.setUniform1fv("angleX", angleX, 2);
    createData.setUniform1fv("angleY", angleY, 2);
    
    pp.src->draw(0,0);
    
    createData.end();
    pp.dst->end();
    
    pp.swap();

}

void GalaxyCollision::setParam(int ch, float val){}






