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

    volSmooth.setSpeed(0.02f);
    for (int i = 0; i < kNumXform; i++) {
        m0S[i].setSpeed(paramSmooth);
        m1S[i].setSpeed(paramSmooth);
        m2S[i].setSpeed(paramSmooth);
        tS[i].setSpeed(paramSmooth);
        wS[i].setSpeed(paramSmooth);
        varSinS[i].setSpeed(paramSmooth);
        varSwirlS[i].setSpeed(paramSmooth);
        varSphS[i].setSpeed(paramSmooth);
    }
    randomize();
}

void FractalFlame::update(float dt) {

    phase += dt * phaseSpeed;
    volSmooth.update(dt);
    volDrive = volSmooth.getValue();
    for (int i = 0; i < kNumXform; i++) {
        m0S[i].update(dt);
        m1S[i].update(dt);
        m2S[i].update(dt);
        tS[i].update(dt);
        wS[i].update(dt);
        varSinS[i].update(dt);
        varSwirlS[i].update(dt);
        varSphS[i].update(dt);

        m0[i] = glm::vec3(m0S[i].x, m0S[i].y, m0S[i].z);
        m1[i] = glm::vec3(m1S[i].x, m1S[i].y, m1S[i].z);
        m2[i] = glm::vec3(m2S[i].x, m2S[i].y, m2S[i].z);
        trans[i] = glm::vec3(tS[i].x, tS[i].y, tS[i].z);
        weights[i] = wS[i].getValue();
        varSin[i] = varSinS[i].getValue();
        varSwirl[i] = varSwirlS[i].getValue();
        varSph[i] = varSphS[i].getValue();
    }

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
    std::array<float, kNumXform> wTarget;
    for (int i = 0; i < kNumXform; i++) {
        wTarget[i] = ofRandom(0.4f, 0.9f);
        sum += wTarget[i];

        glm::vec3 axis = glm::normalize(glm::vec3(ofRandom(-1.0f, 1.0f), ofRandom(-1.0f, 1.0f), ofRandom(-1.0f, 1.0f)));
        float ang = ofRandom(-PI * 0.7f, PI * 0.7f);
        float sx = ofRandom(0.35f, 0.95f);
        float sy = ofRandom(0.35f, 0.95f);
        float sz = ofRandom(0.35f, 0.95f);
        glm::mat3 rot = glm::mat3_cast(glm::angleAxis(ang, axis));
        glm::mat3 scale = glm::mat3(
            glm::vec3(sx, 0.0f, 0.0f),
            glm::vec3(0.0f, sy, 0.0f),
            glm::vec3(0.0f, 0.0f, sz)
        );
        glm::mat3 m = rot * scale;

        m0S[i].to(ofPoint(m[0].x, m[0].y, m[0].z));
        m1S[i].to(ofPoint(m[1].x, m[1].y, m[1].z));
        m2S[i].to(ofPoint(m[2].x, m[2].y, m[2].z));

        tS[i].to(ofPoint(
            ofRandom(-0.55f, 0.55f),
            ofRandom(-0.55f, 0.55f),
            ofRandom(-0.55f, 0.55f)
        ));

        varSinS[i].to(ofRandom(0.1f, 0.8f));
        varSwirlS[i].to(ofRandom(0.1f, 0.8f));
        varSphS[i].to(ofRandom(0.1f, 0.8f));
    }

    for (int i = 0; i < kNumXform; i++) {
        wS[i].to(wTarget[i] / sum);
    }

    phaseSpeed = ofRandom(0.25f, 0.6f);
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
