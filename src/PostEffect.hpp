//
//  PostEffect.hpp
//  LPVJ
//
//  Created by 永松 歩 on 2017/03/09.
//
//

#ifndef PostEffect_hpp
#define PostEffect_hpp

#include "ofMain.h"

class PostEffect {
public:
    void setup();
    void begin();
    void end();
    void applyCorrection();
    const ofTexture& getTexture() const {
        return composite1.getTexture();
    }
    
    void bang();
    void setMode(int mode);
    void setNega(bool bNega) {
        this->bNega = bNega;
    }
    bool isNega() const { return bNega; }
    void setParam(int ch, float val);
    
private:
    ofFbo base;
    ofFbo composite0;
    ofFbo composite1;
    
    int mode = 0;
    float params[3] = { 1.0, 0.2, 1.0 };
    ofShader complexConv;
    ofShader mirrorConv;
    
    bool bNega = false;
    ofShader negaConv;
    ofShader gammaConv;
    
};

#endif /* PostEffect_hpp */
