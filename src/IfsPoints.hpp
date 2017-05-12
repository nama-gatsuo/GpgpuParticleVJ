//
//  IfsPoints.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/10.
//
//

#ifndef IfsPoints_hpp
#define IfsPoints_hpp

#include "ofMain.h"
#include "CommonUtil.hpp"
#include "SceneBase.hpp"

class AffineParams {
public:
    AffineParams():weight(0.5){
        s.to(ofPoint(1.0, 1.0, 1.0));
    };
    void update(float dt){
        
        r.update(dt);
        s.update(dt);
        t.update(dt);
        
        mat = ofMatrix4x4();
        mat.glTranslate(t.x, t.y, t.z);
        mat.glScale(s.x, s.y, s.z);
        mat.glRotate(r.x, 1.0, 0.0, 0.0);
        mat.glRotate(r.y, 0.0, 1.0, 0.0);
        mat.glRotate(r.z, 1.0, 0.0, 1.0);
    };
    
    float weight;
    SmoothPoint r;
    SmoothPoint s;
    SmoothPoint t;
    ofMatrix4x4 mat;
};

class IfsPoints : public SceneBase {
public:
    virtual void setup();
    virtual void update(float dt);
    virtual void draw(float volume);
    virtual void randomize();
    virtual void setParam(int ch, float val);
private:
    CustomShader shader;
    ofVboMesh mesh;
    int num = 200000;
    
    AffineParams ap[2];
    
};

#endif /* IfsPoints_hpp */
