#pragma once

#include "ofMain.h"
#include "../CommonUtil.hpp"
#include "SceneBase.hpp"

class IfsGpuLoop : public SceneBase {
public:
    void setup() override;
    void update(float dt) override;
    void draw(float vol) override;
    void randomize() override;
    void setParam(int ch, float val) override;
    void debugDraw() const override;

private:
    CustomShader shader;
    ofVboMesh mesh;
    int num = 1000000;

    int numClasses = 48;
    int loopCount = 64;
    float worldScale = 50.0f;

    SmoothValue tBase;
    SmoothValue tSpread;
    SmoothValue phaseSpeed;

    SmoothValue volSmooth;
};
