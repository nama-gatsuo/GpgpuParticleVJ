#include "ofApp.h"

void ofApp::setup(){
    
    ofBackground(0);
    ofDisableAlphaBlending();
    ofDisableBlendMode();
    ofSetFrameRate(30);
    pe.setup();
    cam.setRadius(500);
    
    ofPtr<SceneBase> sp0 = std::make_shared<IfsPoints>();
    ofPtr<SceneBase> sp1 = std::make_shared<SmoothedAttractor>();
    ofPtr<SceneBase> sp2 = std::make_shared<StrangeAttractor>();
    ofPtr<SceneBase> sp3 = std::make_shared<GalaxyCollision>();
    ofPtr<SceneBase> sp4 = std::make_shared<SpiralSphere>();
    sp0->setup();
    sp1->setup();
    sp2->setup();
    sp3->setup();
    sp4->setup();
    scenes.push_back(sp0);
    scenes.push_back(sp1);
    scenes.push_back(sp2);
    scenes.push_back(sp3);
    scenes.push_back(sp4);
    sceneList.push_back(state);
    
    receiver.setup(7401);

    spoutSender.init("ParticleVJ");
}

void ofApp::update(){
    
    while (receiver.hasWaitingMessages()){
        ofxOscMessage m;
        receiver.getNextMessage(m);
        
        string address = m.getAddress();
        vector<string> dirs = ofSplitString(address, "/");
        
        if (dirs[1] == "p") {

            int i = ofToInt(dirs[2]);
            int val = m.getArgAsInt32(0);

            if (i == 3) cam.setRadius(1.0 + 1000.0 * val / 128.0);
            else if (i == 4) dt = 0.001 + 0.1 * val / 128.0;
            else scenes[state]->setParam(i, val);

        }
        else if (dirs[1] == "scene") { state = ofClamp(m.getArgAsInt(0), 0, scenes.size() - 1); }
        else if (dirs[1] == "vol") vol = m.getArgAsFloat(0);
        else if (dirs[1] == "bang") {
            
            if (dirs[2] == "0") {
                scenes[state]->randomize();
                //if (ofRandom(1.0) < 0.1) changeScene();
            } else if (dirs[2] == "1") {
                pe.bang();
            }
            
        }
        
    }

    scenes[state]->update(dt);
    cam.update(dt);
    
}

void ofApp::draw(){
    
    ofClear(0);
    
    pe.begin();
    cam.begin();
    
    scenes[state]->draw(vol);
    
    cam.end();
    pe.end();

    pe.applyCorrection();
    pe.getTexture().draw(0, 0);
    
    spoutSender.send(pe.getTexture());

    // ofDrawBitmapString("fps:" + ofToString(ofGetFrameRate(), 4), 10, 20);
}

void ofApp::keyPressed(int key){
    
    if (key == ' ') scenes[state]->randomize();
    else if (key == '2') pe.setMode(1);
    else if (key == '3') pe.setMode(2);
    else if (key == '4') pe.setMode(3);
    else if (key == '1') pe.setMode(0);
    else if (key == 'b') pe.setNega(!pe.isNega());
    else if (key == 'q') state = 0;
    else if (key == 'w') state = 1;
    else if (key == 'e') state = 2;
    else if (key == 'r') state = 3;
    else if (key == 't') state = 4;
}

void ofApp::changeScene(){
    
    int n = sceneList.size();
    if (n == 0) return;
    int coin = (int)floor(ofRandom(n));
    state = sceneList[coin];
    
}

void ofApp::updateSceneList(int num){
    
    vector<int>::iterator it;
    
    for (it = sceneList.begin(); it < sceneList.end(); ++it) {
        if (*it == num) {
            sceneList.erase(it);
            return;
        }
    }
    sceneList.push_back(num);
    
}
