#include "VortexRing.hpp"
#include <glm/gtc/random.hpp>

void VortexRing::setup() {

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int x = 0; x < wSize; x++) {
        for (int y = 0; y < hSize; y++) {
            mesh.addVertex(glm::vec3(0, 0, 0));
            mesh.addTexCoord(glm::vec2(x, y));
            float hue = ofMap(y, 0, hSize - 1, 0.5f, 0.9f, true);
            mesh.addColor(ofFloatColor::fromHsb(hue, 0.8f, 1.0f, 0.6f));
        }
    }

    std::vector<float> pos(wSize * hSize * 4);
    std::vector<float> vel(wSize * hSize * 4);

    for (int w = 0; w < wSize; w++) {
        for (int h = 0; h < hSize; h++) {

            int i = h * wSize + w;

            glm::vec3 p = glm::sphericalRand(1.0f);
            p *= ofRandom(0.6f, 1.2f);

            pos[i * 4 + 0] = p.x;
            pos[i * 4 + 1] = p.y;
            pos[i * 4 + 2] = p.z;
            pos[i * 4 + 3] = 1.0f;

            vel[i * 4 + 0] = 0.0f;
            vel[i * 4 + 1] = 0.0f;
            vel[i * 4 + 2] = 0.0f;
            vel[i * 4 + 3] = 0.0f;
        }
    }

    pp.allocate(wSize, hSize);
    pp.src->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    pp.src->getTexture(1).loadData(vel.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(1).loadData(vel.data(), wSize, hSize, GL_RGBA);

    updateData.load("shaders/Common/passThru.vert", "shaders/Scene/VortexRing/updateData.frag");
    updateRenderer.load("shaders/Scene/VortexRing/render");

    tilt.setSpeed(0.015f);
    randomize();
}

void VortexRing::update(float dt) {

    phase += dt * phaseSpeed;
    float tiltSpeed = 0.008f + volDrive * 0.06f;
    tilt.setSpeed(tiltSpeed);
    tilt.update(dt);

    pp.dst->begin();
    ofClear(0.0f, 255.0f);
    pp.dst->activateAllDrawBuffers();

    updateData.begin();
    updateData.setUniformTexture("posData", pp.src->getTexture(0), 0);
    updateData.setUniformTexture("velData", pp.src->getTexture(1), 1);
    float thicknessAnim = ringThickness * (0.3f + 1.7f * (0.5f + 0.5f * sin(phase * 0.7f)));
    updateData.setUniform1f("ringRadius", ringRadius);
    updateData.setUniform1f("ringThickness", thicknessAnim);
    updateData.setUniform1f("swirl", swirl);
    updateData.setUniform1f("pull", pull);
    updateData.setUniform1f("noiseAmount", noise);
    float deform = 0.6f + 1.8f * (0.5f + 0.5f * sin(phase * 0.35f));
    float twist = 0.4f + 1.6f * (0.5f + 0.5f * sin(phase * 0.22f + 0.8f));
    updateData.setUniform1f("deform", deform);
    updateData.setUniform1f("twist", twist);
    float jitterAmp = 0.35f + 1.35f * (0.5f + 0.5f * sin(phase * 1.4f + 0.8f));
    updateData.setUniform1f("jitterAmp", jitterAmp);
    updateData.setUniform1f("phase", phase);
    updateData.setUniform1f("dt", dt);

    pp.src->getTexture(0).draw(0, 0);

    updateData.end();
    pp.dst->end();

    pp.swap();
}

void VortexRing::draw(float vol) {

    enablePointSprite();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    volDrive = vol;

    updateRenderer.begin();
    float wobble = sin(phase * 0.9f);
    float wobble2 = sin(phase * 0.47f + 1.2f);
    updateRenderer.setUniform1f("alpha", vol);
    updateRenderer.setUniform3f(
        "tilt",
        tilt.x + wobble * 35.0f,
        tilt.y + wobble2 * 55.0f,
        tilt.z + wobble * 25.0f
    );
    updateRenderer.setUniformTexture("posData", pp.dst->getTexture(0), 0);

    mesh.draw();

    updateRenderer.end();
    ofDisableBlendMode();
}

void VortexRing::randomize() {

    ringRadius = ofRandom(0.35f, 1.6f);
    ringThickness = ofRandom(0.08f, 0.9f);
    swirl = ofRandom(0.4f, 1.8f);
    pull = ofRandom(0.7f, 2.2f);
    noise = ofRandom(0.04f, 0.16f);
    phaseSpeed = ofRandom(0.3f, 1.2f);
    tilt.to(glm::vec3(ofRandom(-80.0f, 80.0f), ofRandom(-80.0f, 80.0f), ofRandom(-80.0f, 80.0f)));
}

void VortexRing::setParam(int ch, float val) {

    switch (ch) {
        case 0: swirl = ofMap(val, 0, 127, 0.2f, 2.5f, true); break;
        case 1: pull = ofMap(val, 0, 127, 0.4f, 2.8f, true); break;
        case 2: ringThickness = ofMap(val, 0, 127, 0.05f, 0.4f, true); break;
        default: break;
    }
}

void VortexRing::debugDraw() const {
    pp.src->getTexture(0).draw(0, 0);
}
