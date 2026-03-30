#include "FractalFlame.hpp"
#include <glm/gtx/quaternion.hpp>

void FractalFlame::setup() {

    mesh.setMode(OF_PRIMITIVE_POINTS);
    for (int x = 0; x < wSize; x++) {
        for (int y = 0; y < hSize; y++) {
            mesh.addVertex(glm::vec3(0, 0, 0));
            mesh.addTexCoord(glm::vec2(x, y));
            mesh.addColor(ofFloatColor(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    std::vector<float> pos(wSize * hSize * 4);
    std::vector<float> col(wSize * hSize * 4);

    for (int w = 0; w < wSize; w++) {
        for (int h = 0; h < hSize; h++) {
            int i = h * wSize + w;
            pos[i * 4 + 0] = ofRandom(-1.0f, 1.0f);
            pos[i * 4 + 1] = ofRandom(-1.0f, 1.0f);
            pos[i * 4 + 2] = ofRandom(-1.0f, 1.0f);
            pos[i * 4 + 3] = 1.0f;

            col[i * 4 + 0] = 0.0f;
            col[i * 4 + 1] = 0.0f;
            col[i * 4 + 2] = 0.0f;
            col[i * 4 + 3] = 1.0f;
        }
    }

    pp.allocate(wSize, hSize);
    pp.src->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(0).loadData(pos.data(), wSize, hSize, GL_RGBA);
    pp.src->getTexture(1).loadData(col.data(), wSize, hSize, GL_RGBA);
    pp.dst->getTexture(1).loadData(col.data(), wSize, hSize, GL_RGBA);

    updateData.load("shaders/Common/passThru.vert", "shaders/Scene/FractalFlame/updateData.frag");
    updateRenderer.load("shaders/Scene/FractalFlame/render");

    volSmooth.setSpeed(0.8f);
    randomize();
}

void FractalFlame::update(float dt) {

    phase += dt * phaseSpeed;
    volSmooth.update(dt);
    volDrive = volSmooth.getValue();

    pp.dst->begin();
    ofClear(0.0f, 255.0f);
    pp.dst->activateAllDrawBuffers();

    updateData.begin();
    updateData.setUniformTexture("posData", pp.src->getTexture(0), 0);
    updateData.setUniformTexture("colData", pp.src->getTexture(1), 1);
    updateData.setUniform1f("phase", phase);
    updateData.setUniform1f("react", volDrive);
    updateData.setUniform1f("dt", dt);
    updateData.setUniform3fv("uM0", &m0[0].x, kNumXform);
    updateData.setUniform3fv("uM1", &m1[0].x, kNumXform);
    updateData.setUniform3fv("uM2", &m2[0].x, kNumXform);
    updateData.setUniform3fv("uT", &trans[0].x, kNumXform);
    updateData.setUniform1fv("uW", weights.data(), kNumXform);
    updateData.setUniform1fv("uVarSin", varSin.data(), kNumXform);
    updateData.setUniform1fv("uVarSwirl", varSwirl.data(), kNumXform);
    updateData.setUniform1fv("uVarSph", varSph.data(), kNumXform);

    pp.src->getTexture(0).draw(0, 0);

    updateData.end();
    pp.dst->end();

    pp.swap();
}

void FractalFlame::draw(float vol) {

    volSmooth.to(vol);
    enablePointSprite();
    ofEnableBlendMode(OF_BLENDMODE_ADD);

    updateRenderer.begin();
    updateRenderer.setUniform1f("alpha", volDrive + 0.2f);
    updateRenderer.setUniformTexture("posData", pp.dst->getTexture(0), 0);
    updateRenderer.setUniformTexture("colData", pp.dst->getTexture(1), 1);

    mesh.draw();

    updateRenderer.end();
    ofDisableBlendMode();
}

void FractalFlame::randomize() {

    float sum = 0.0f;
    for (int i = 0; i < kNumXform; i++) {
        weights[i] = ofRandom(0.2f, 1.0f);
        sum += weights[i];

        glm::vec3 axis = glm::normalize(glm::vec3(ofRandom(-1.0f, 1.0f), ofRandom(-1.0f, 1.0f), ofRandom(-1.0f, 1.0f)));
        float ang = ofRandom(-PI, PI);
        float sx = ofRandom(0.2f, 1.1f);
        float sy = ofRandom(0.2f, 1.1f);
        float sz = ofRandom(0.2f, 1.1f);
        glm::mat3 rot = glm::mat3_cast(glm::angleAxis(ang, axis));
        glm::mat3 scale = glm::mat3(
            glm::vec3(sx, 0.0f, 0.0f),
            glm::vec3(0.0f, sy, 0.0f),
            glm::vec3(0.0f, 0.0f, sz)
        );
        glm::mat3 m = rot * scale;

        m0[i] = m[0];
        m1[i] = m[1];
        m2[i] = m[2];

        trans[i] = glm::vec3(ofRandom(-0.8f, 0.8f), ofRandom(-0.8f, 0.8f), ofRandom(-0.8f, 0.8f));

        varSin[i] = ofRandom(0.0f, 1.0f);
        varSwirl[i] = ofRandom(0.0f, 1.0f);
        varSph[i] = ofRandom(0.0f, 1.0f);
    }

    for (int i = 0; i < kNumXform; i++) {
        weights[i] /= sum;
    }

    phaseSpeed = ofRandom(0.2f, 0.8f);
}

void FractalFlame::setParam(int ch, float val) {

    switch (ch) {
        case 0: phaseSpeed = ofMap(val, 0, 127, 0.05f, 1.2f, true); break;
        default: break;
    }
}

void FractalFlame::debugDraw() const {
    pp.src->getTexture(0).draw(0, 0);
}
