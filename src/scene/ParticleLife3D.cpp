#include "ParticleLife3D.hpp"
#include <glm/gtc/random.hpp>

void ParticleLife3D::setup() {

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int y = 0; y < hSize; y++) {
        for (int x = 0; x < wSize; x++) {
            mesh.addVertex(glm::vec3(0, 0, 0));
            mesh.addTexCoord(glm::vec2(x, y));
            mesh.addColor(ofFloatColor(1.0f));
        }
    }

    std::vector<float> pos(wSize * hSize * 4);
    std::vector<float> vel(wSize * hSize * 4);

    for (int y = 0; y < hSize; y++) {
        for (int x = 0; x < wSize; x++) {
            int i = y * wSize + x;
            glm::vec3 p = glm::sphericalRand(1.0f) * ofRandom(0.05f, 0.9f);
            glm::vec3 v = glm::sphericalRand(1.0f) * ofRandom(0.0f, 0.01f);
            int type = static_cast<int>(ofRandom(kNumTypes));

            pos[i * 4 + 0] = p.x;
            pos[i * 4 + 1] = p.y;
            pos[i * 4 + 2] = p.z;
            pos[i * 4 + 3] = static_cast<float>(type);

            vel[i * 4 + 0] = v.x;
            vel[i * 4 + 1] = v.y;
            vel[i * 4 + 2] = v.z;
            vel[i * 4 + 3] = ofRandom(0.90f, 0.98f);
        }
    }

    pp.allocate(wSize, hSize);
    pp.src->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    pp.src->getTexture(1).loadData(vel.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(1).loadData(vel.data(), wSize, hSize, GL_RGBA);

    updateData.load("shaders/Common/passThru.vert", "shaders/Scene/ParticleLife3D/updateData.frag");
    renderer.load("shaders/Scene/ParticleLife3D/render");

    volSmooth.setSpeed(0.02f);
    randomize();
}

void ParticleLife3D::update(float dt) {

    timePhase += dt;
    volSmooth.update(dt);
    interactionRadius = targetInteractionRadius;
    searchRange = targetSearchRange;
    forceScale = targetForceScale;
    baseRepulsion = targetBaseRepulsion;
    centerPull = targetCenterPull;
    noiseAmount = targetNoiseAmount;

    pp.dst->begin();
    ofClear(0.0f, 255.0f);
    pp.dst->activateAllDrawBuffers();

    updateData.begin();
    updateData.setUniformTexture("posData", pp.src->getTexture(0), 0);
    updateData.setUniformTexture("velData", pp.src->getTexture(1), 1);
    updateData.setUniform1f("dt", dt);
    updateData.setUniform1f("phase", timePhase);
    updateData.setUniform1i("numTypes", kNumTypes);
    updateData.setUniform1f("interactionRadius", interactionRadius);
    updateData.setUniform1f("searchRange", searchRange);
    updateData.setUniform1f("forceScale", forceScale);
    updateData.setUniform1f("baseRepulsion", baseRepulsion);
    updateData.setUniform1f("boundRadius", boundRadius);
    updateData.setUniform1f("centerPull", centerPull);
    updateData.setUniform1f("noiseAmount", noiseAmount);
    updateData.setUniform2f("simRes", static_cast<float>(wSize), static_cast<float>(hSize));
    updateData.setUniform1fv("interaction", interaction.data(), static_cast<int>(interaction.size()));
    pp.src->getTexture(0).draw(0, 0);
    updateData.end();

    pp.dst->end();
    pp.swap();
}

void ParticleLife3D::draw(float vol) {

    volSmooth.to(vol);

    enablePointSprite();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    renderer.begin();
    renderer.setUniformTexture("posData", pp.dst->getTexture(0), 0);
    renderer.setUniform1f("alpha", 0.45f + volSmooth.getValue() * 2.2f);
    renderer.setUniform1f("worldScale", 160.0f);
    renderer.setUniform1f("numTypes", static_cast<float>(kNumTypes));
    mesh.draw();
    renderer.end();

    ofDisableBlendMode();
}

void ParticleLife3D::randomize() {

    targetInteractionRadius = ofRandom(0.30f, 0.60f);
    targetSearchRange = ofRandom(6.0f, 16.0f);
    targetForceScale = ofRandom(1.1f, 3.0f);
    targetBaseRepulsion = ofRandom(0.02f, 0.08f);
    targetCenterPull = ofRandom(0.002f, 0.035f);
    targetNoiseAmount = ofRandom(0.001f, 0.012f);

    float interactionGain = ofRandom(0.85f, 1.2f);
    for (int i = 0; i < kNumTypes; i++) {
        for (int j = 0; j < kNumTypes; j++) {
            int d = (j - i + kNumTypes) % kNumTypes;
            float v = 0.0f;
            if (d == 0) {
                v = ofRandom(0.9f, 1.8f);          // same type attraction
            } else if (d == 1 || d == kNumTypes - 1) {
                v = ofRandom(0.2f, 1.1f);          // neighbor types weak-mid attraction
            } else if (d == 2 || d == kNumTypes - 2) {
                v = ofRandom(-0.45f, 0.35f);       // mixed band
            } else {
                v = ofRandom(-1.1f, -0.2f);        // opposite types repulsion
            }
            interaction[i * kNumTypes + j] = v * interactionGain;
        }
    }
}

void ParticleLife3D::setParam(int ch, float val) {

    switch (ch) {
        case 0: targetForceScale = ofMap(val, 0, 127, 0.4f, 5.2f, true); break;
        case 1: targetInteractionRadius = ofMap(val, 0, 127, 0.18f, 0.62f, true); break;
        case 2: targetCenterPull = ofMap(val, 0, 127, 0.0f, 0.2f, true); break;
        case 3: targetSearchRange = ofMap(val, 0, 127, 2.0f, 34.0f, true); break;
        case 4: targetBaseRepulsion = ofMap(val, 0, 127, 0.0f, 0.9f, true); break;
        default: break;
    }
}

void ParticleLife3D::debugDraw() const {
    pp.src->getTexture(0).draw(0, 0);
}
