//
//  SmoothedAttractor.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#ifndef SmoothedAttractor_hpp
#define SmoothedAttractor_hpp

#include "ofMain.h"
#include "CommonUtil.hpp"
#include "SceneBase.hpp"

class SmoothedAttractor : public SceneBase {
public:
    virtual void setup();
    virtual void update(float dt);
    virtual void draw(float vol);
    virtual void randomize();
    virtual void setParam(int ch, float val);
private:
    
    ofVboMesh mesh;
    
    int wSize = 1024;
    int hSize = 1024;
    
    PingPongBuffer pp;
    
    ofShader updateData;
    ofShader updateRenderer;
    
    SmoothValue r, f;
    SmoothPoint aPos;
};


#endif /* SmoothedAttractor_hpp */
