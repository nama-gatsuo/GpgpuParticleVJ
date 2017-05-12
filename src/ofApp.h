#pragma once

#include "ofMain.h"
#include "ofxOsc.h"
#include "PostEffect.hpp"
#include "CommonUtil.hpp"
#include "SceneBase.hpp"

#include "IfsPoints.hpp"
#include "SmoothedAttractor.hpp"
#include "StrangeAttractor.hpp"
#include "GalaxyCollision.hpp"
#include "SpiralSphere.hpp"

class ofApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed(int key);
    
    void changeScene();
    void updateSceneList(int num);
    
    float dt = 0.1;
    float vol = 0.1;
    PostEffect pe;
    MovingCamera cam;
    
    int state = 0;
    vector<boost::shared_ptr<SceneBase>> scenes;
    vector<int> sceneList;
    
    ofxOscReceiver receiver;
};
