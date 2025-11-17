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
    void setup() override;
    void update(float dt) override;
    void draw(float vol) override;
    void randomize() override;
    void setParam(int ch, float val) override;
    void debugDraw() const override;
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
