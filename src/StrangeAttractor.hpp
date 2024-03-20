//
//  StrangeAttractor.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#ifndef StrangeAttractor_hpp
#define StrangeAttractor_hpp

#include "ofMain.h"
#include "CommonUtil.hpp"
#include "SceneBase.hpp"

class StrangeAttractor : public SceneBase {
public:
    virtual void setup();
    virtual void update(float dt);
    virtual void draw(float vol);
    virtual void randomize();
    virtual void setParam(int ch, float val);
private:
    CustomShader shader;
    ofVboMesh mesh;
    int num = 1000000;
    
    SmoothValue ap[6];
    float params[6];
    
};

#endif /* StrangeAttractor_hpp */
