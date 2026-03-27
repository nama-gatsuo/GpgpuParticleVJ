#include "ofMain.h"
#include "../CommonUtil.hpp"
#include "SceneBase.hpp"

class StrangeAttractor : public SceneBase {
public:
    void setup() override;
    void update(float dt) override;
    void draw(float vol) override;
    void randomize() override;
    void setParam(int ch, float val) override;
private:
    CustomShader shader;
    ofVboMesh mesh;
    int num = 1000000;
    
    SmoothValue ap[6];
    float params[6];
    
};
