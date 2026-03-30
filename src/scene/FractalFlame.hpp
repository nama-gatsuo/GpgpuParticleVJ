#pragma once

#include "ofMain.h"
#include "../CommonUtil.hpp"
#include "SceneBase.hpp"

class FractalFlame : public SceneBase {
public:
    void setup() override;
    void update(float dt) override;
    void draw(float vol) override;
    void randomize() override;
    void setParam(int ch, float val) override;
    void debugDraw() const override;

private:
    static constexpr int kNumXform = 4;

    ofVboMesh mesh;
    ofShader updateData;
    ofShader updateRenderer;

    int wSize = 768;
    int hSize = 768;
    PingPongBuffer pp;

    std::array<glm::vec3, kNumXform> m0;
    std::array<glm::vec3, kNumXform> m1;
    std::array<glm::vec3, kNumXform> m2;
    std::array<glm::vec3, kNumXform> trans;
    std::array<float, kNumXform> weights;
    std::array<float, kNumXform> varSin;
    std::array<float, kNumXform> varSwirl;
    std::array<float, kNumXform> varSph;

    float phase = 0.0f;
    float phaseSpeed = 0.35f;
    float volDrive = 0.1f;
    SmoothValue volSmooth;

    float paramSmooth = 0.05f;
    std::array<SmoothPoint, kNumXform> m0S;
    std::array<SmoothPoint, kNumXform> m1S;
    std::array<SmoothPoint, kNumXform> m2S;
    std::array<SmoothPoint, kNumXform> tS;
    std::array<SmoothValue, kNumXform> wS;
    std::array<SmoothValue, kNumXform> varSinS;
    std::array<SmoothValue, kNumXform> varSwirlS;
    std::array<SmoothValue, kNumXform> varSphS;
};
