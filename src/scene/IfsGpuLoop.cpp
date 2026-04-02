#include "IfsGpuLoop.hpp"

void IfsGpuLoop::setup() {

    shader.load("shaders/Scene/IfsGpuLoop/shader");

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int i = 0; i < num; i++) {
        mesh.addVertex(glm::vec3(ofRandom(-1.0f, 1.0f), ofRandom(-1.0f, 1.0f), ofRandom(-1.0f, 1.0f)));

        // color.r = normalized class id, color.g = seed, color.b = brightness
        float cls = ofRandomuf();
        float seed = ofRandomuf();
        float bright = ofRandom(0.4f, 1.0f);
        mesh.addColor(ofFloatColor(cls, seed, bright, 1.0f));
    }

    tBase.setSpeed(0.03f);
    tSpread.setSpeed(0.03f);
    phaseSpeed.setSpeed(0.03f);
    volSmooth.setSpeed(0.025f);
    randomize();
}

void IfsGpuLoop::update(float dt) {

    tBase.update(dt);
    tSpread.update(dt);
    phaseSpeed.update(dt);
    volSmooth.update(dt);
}

void IfsGpuLoop::draw(float vol) {

    volSmooth.to(vol);
    enablePointSprite();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    shader.begin();
    shader.setUniform1f("alpha", 0.30f + volSmooth.getValue() * 2.2f);
    shader.setUniform1f("worldScale", worldScale);
    shader.setUniform1f("tBase", tBase.getValue());
    shader.setUniform1f("tSpread", tSpread.getValue());
    shader.setUniform1f("phaseSpeed", phaseSpeed.getValue());
    shader.setUniform1i("numClasses", numClasses);
    shader.setUniform1i("loopCount", loopCount);
    mesh.draw();
    shader.end();

    ofDisableBlendMode();
}

void IfsGpuLoop::randomize() {
    tBase.to(ofRandom(-0.25f, 0.25f));
    tSpread.to(ofRandom(0.03f, 0.34f));
    phaseSpeed.to(ofRandom(0.10f, 1.0f));
    numClasses = static_cast<int>(ofRandom(16, 72));
}

void IfsGpuLoop::setParam(int ch, float val) {

    switch (ch) {
        case 0: loopCount = static_cast<int>(ofMap(val, 0, 127, 8, 64, true)); break;
        case 1: tBase.to(ofMap(val, 0, 127, -0.6f, 0.6f, true)); break;
        case 2: tSpread.to(ofMap(val, 0, 127, 0.0f, 0.8f, true)); break;
        case 3: break;
        case 4: numClasses = static_cast<int>(ofMap(val, 0, 127, 4, 96, true)); break;
        default: break;
    }
}

void IfsGpuLoop::debugDraw() const {
    // no-op (vertex shader driven scene)
}
