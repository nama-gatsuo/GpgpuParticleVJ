//
//  GalaxyCollision.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#ifndef GalaxyCollision_hpp
#define GalaxyCollision_hpp

#include "ofMain.h"
#include "CommonUtil.hpp"
#include "SceneBase.hpp"

class GalaxyCollision : public SceneBase{
public:
    virtual void setup();
    virtual void update(float dt);
    virtual void draw(float vol);
    virtual void randomize();
    virtual void setParam(int ch, float val);

private:
    
    ofVboMesh mesh;
    ofShader updateData;
    ofShader updateRenderer;
    ofShader createData;
    
    int wSize = 512, hSize = 512;
    PingPongBuffer pp;
    
    ofVec3f p0, p1, v0, v1, a0, a1;
    
    float GM;
    float diskRad[2];
    float bulgeRad[2];
    float ratio[2];
    
    float distance = 40.0;
    float massRatio = 0.07;
    float speed = sqrt(GM / 2.0);
    
    float angleX[2] = { 20.0, - 45.0 };
    float angleY[2] = { 10.0, 0.0 };
};


#endif /* GalaxyCollision_hpp */
