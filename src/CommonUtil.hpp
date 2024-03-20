//
//  CommonUtil.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/09.
//
//

#ifndef CommonUtil_hpp
#define CommonUtil_hpp

#include "ofMain.h"

static void enablePointSprite(){
    glEnable(GL_POINT_SPRITE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
}


class CustomShader : public ofShader{
public:
    void begin(){
        ofShader::begin();
        setLazyUniforms();
    };
    void setLazyUniforms(){
        setUniform1f("time", ofGetElapsedTimef());
        setUniform1f("frame", ofGetFrameNum());
        setUniform2f("res", ofGetWidth(), ofGetHeight());
    };
    
};


class SmoothPoint : public ofPoint{
public:
    SmoothPoint():mSpeed(0.05){};
    void update(float dt){
        this->x += (mTarget.x - this->x) * mSpeed * dt * 10.0;
        this->y += (mTarget.y - this->y) * mSpeed * dt * 10.0;
        this->z += (mTarget.z - this->z) * mSpeed * dt * 10.0;
    }
    void to(ofPoint target){
        mTarget = target;
    }
    void setSpeed(float speed){
        mSpeed = speed;
    }
    
private:
    ofPoint mTarget;
    float mSpeed;
};


class SmoothValue {
public:
    SmoothValue():mSpeed(0.05),mValue(0),mTarget(0){}
    
    void update(float dt){
        mValue += (mTarget - mValue) * mSpeed * dt * 10.0;
    }
    void to(float target){
        mTarget = target;
    }
    float getValue(){
        return mValue;
    };
    void setSpeed(float speed){
        mSpeed = speed;
    }
    
private:
    float mValue;
    float mTarget;
    float mSpeed;
};


struct PingPongBuffer {
public:
    void allocate( int _width, int _height){
        
        for(int i = 0; i < 2; i++){
            
            ofFbo::Settings s;
            
            s.width = _width;
            s.height = _height;
            //s.internalformat = GL_RGB32F;
            s.minFilter = GL_NEAREST;
            s.maxFilter = GL_NEAREST;
            //s.numColorbuffers = 2;
            s.colorFormats.push_back(GL_RGBA32F);
            s.colorFormats.push_back(GL_RGBA32F);
            s.useDepth = true;
            s.useStencil = false;
            s.depthStencilAsTexture = false;
            s.numSamples = 0;
            FBOs[i].allocate(s);
            //FBOs[i].createAndAttachTexture(GL_RGBA32F, 1);
            //FBOs[i].createAndAttachTexture(GL_RGBA32F, 2);
            //
            //FBOs[i].getTexture(0).setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
            //FBOs[i].getTexture(1).setTextureMinMagFilter(GL_NEAREST, GL_NEAREST);
        }
        
        clear();
    }
    
    void swap(){
        std::swap(src,dst);
    }
    
    void clear(){
        for(int i = 0; i < 2; i++){
            FBOs[i].begin();
            ofClear(0,255);
            FBOs[i].end();
        }
    }
    
    ofFbo& operator[]( int n ){ return FBOs[n];}
    ofFbo *src = &FBOs[0];
    ofFbo *dst = &FBOs[1];
    
private:
    ofFbo FBOs[2];
};


class MovingCamera : public ofCamera {
public:
    void to() {
        
    };
    void setSpeed(float speed){
        
    };
    void update(float dt){
        
        radius.update(dt);
        phi += speed * dt * 10.0;
        
        x = radius.getValue() * sin(theta) * sin(phi);
        y = radius.getValue() * cos(theta) * sin(phi);
        z = radius.getValue() * cos(phi);
        
        this->setPosition(x, y, z);
        this->lookAt(ofVec3f(0,0,0));
    };
    void setRadius(float radius){
        this->radius.to(radius);
    };
    
private:
    float speed = 0.01;
    float theta = PI/2;
    float phi = 0;
    SmoothValue radius;
    
    float x, y, z;
};


#endif /* CommonUtil_hpp */
