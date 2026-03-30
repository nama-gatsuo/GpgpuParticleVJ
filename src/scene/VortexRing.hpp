#pragma once

#include "ofMain.h"
#include "../CommonUtil.hpp"
#include "SceneBase.hpp"

class VortexRing : public SceneBase {
public:
    void setup() override;
    void update(float dt) override;
    void draw(float vol) override;
    void randomize() override;
    void setParam(int ch, float val) override;
    void debugDraw() const override;

private:
    ofVboMesh mesh;
    ofShader updateData;
    ofShader updateRenderer;

    int wSize = 512;
    int hSize = 512;
    PingPongBuffer pp;

    // normalized units (render scale is applied in shader)
    float ringRadius = 0.9f;
    float ringThickness = 0.18f;
    float swirl = 1.0f;
    float pull = 1.2f;
    float noise = 0.08f;
    float phase = 0.0f;
    float phaseSpeed = 0.6f;

    SmoothPoint tilt;
    float volDrive = 0.1f;
};
