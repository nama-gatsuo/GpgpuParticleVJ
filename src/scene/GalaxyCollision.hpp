#include "ofMain.h"
#include "../CommonUtil.hpp"
#include "SceneBase.hpp"

class GalaxyCollision : public SceneBase {
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
    ofShader createData;
    
    int wSize = 512;
    int hSize = 512;
    PingPongBuffer pp;
    
    ofVec3f p0, p1, v0, v1, a0, a1;
    
    float GM;
    float diskRad[2];
    float bulgeRad[2];
    float ratio[2];
    
    float distance = 40.0f;
    float massRatio = 0.07f;
    float speed = sqrt(GM / 2.0);
    
    float angleX[2] = { 20.0f, -45.0f };
    float angleY[2] = { 10.0f, 0.0f };
};
