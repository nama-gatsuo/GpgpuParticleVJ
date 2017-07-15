//
//  SpiralSphere.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/11.
//
//

#ifndef SpiralSphere_hpp
#define SpiralSphere_hpp

#include "ofMain.h"
#include "CommonUtil.hpp"
#include "SceneBase.hpp"

class SParams {
public:
    void init() {
        
        rInit.to(ofRandom(50, 600));
        rStep.to(ofRandom(0.1, 3.0));
        sStep.to(ofRandom(-0.05, 0.05));
        tStep.to(ofRandom(-0.05, 0.05));
        
    };
    void update(float dt) {
        rInit.update(dt);
        rStep.update(dt);
        sStep.update(dt);
        tStep.update(dt);
    };
    void randomize() {
        if (ofRandom(1.0) < 0.2) {
            rStep.to(ofRandom(-0.1, 0.1));
        } else {
            rStep.to(ofRandom(-0.5, 0.5));
        }
        
        rInit.to(ofRandom(10, 300));
        
        if (ofRandom(1.0) < 0.2) {
            sStep.to(ofRandom(-0.1, 0.1));
        } else {
            sStep.to(ofRandom(-0.03, 0.03));
        }
        
        if (ofRandom(1.0) < 0.2) {
            tStep.to(ofRandom(-0.01, 0.01));
        } else {
            tStep.to(ofRandom(-0.03, 0.03));
        }

    };
    
    SmoothValue rInit;
    SmoothValue rStep;
    SmoothValue sStep;
    SmoothValue tStep;
};

class SpiralSphere : public SceneBase {
public:
    virtual void setup();
    virtual void update(float dt);
    virtual void draw(float vol);
    virtual void randomize();
    virtual void setParam(int ch, float val);
    
private:
    ofShader shader;
    ofVboMesh mesh;
    
    int rSize = 100;
    int tSize = 24;
    int sSize = 12;
    
    int spNum = 10;
    SParams ps[10];
};

#endif /* SpiralSphere_hpp */
