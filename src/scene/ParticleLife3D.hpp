#pragma once

#include "ofMain.h"
#include "../CommonUtil.hpp"
#include "SceneBase.hpp"
#include <array>

class ParticleLife3D : public SceneBase {
public:
    void setup() override;
    void update(float dt) override;
    void draw(float vol) override;
    void randomize() override;
    void setParam(int ch, float val) override;
    void debugDraw() const override;

private:
    static constexpr int kNumTypes = 6;

    ofVboMesh mesh;
    ofShader updateData;
    ofShader renderer;

    int wSize = 256;
    int hSize = 256;
    PingPongBuffer pp;

    std::array<float, kNumTypes * kNumTypes> interaction = {};
    float interactionRadius = 0.44f;
    float targetInteractionRadius = 0.44f;
    float searchRange = 14.0f;
    float targetSearchRange = 14.0f;
    float forceScale = 2.0f;
    float targetForceScale = 2.0f;
    float baseRepulsion = 0.35f;
    float targetBaseRepulsion = 0.35f;
    float boundRadius = 1.55f;
    float centerPull = 0.05f;
    float targetCenterPull = 0.05f;
    float noiseAmount = 0.02f;
    float targetNoiseAmount = 0.02f;
    float timePhase = 0.0f;

    SmoothValue volSmooth;
};
