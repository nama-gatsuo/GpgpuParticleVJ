#pragma once

#include "ofxSpout.h"

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

#include "ofxLayerFx.h"
#include "ofxGui.h"

#include "vjOscReceiver.h"


class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void keyPressed(int key);

	void changeScene();
	void updateSceneList(int num);
	void setFX(int mode);

	void onFeedbackChanged(float& v);
	void onReverbChanged(float& t);
	void onFilterChanged(float& t);

	float dt = 0.05;
	float vol = 0.1;
	ofxLayerFx::Processor pe;
	MovingCamera cam;

	int state = 0;
	std::vector<ofPtr<SceneBase>> scenes;
	std::vector<int> sceneList;

	ofxOscReceiver receiver;
	ofxSpout::Sender spoutSender;

	ofPtr<ofxLayerFx::FeedbackPass> feedback;
	ofPtr<ofxLayerFx::PsuedoFocusBlurPass> focusBlur;
	ofPtr<ofxLayerFx::ComplexConvPass> complexConv;
	ofPtr<ofxLayerFx::MirrorPass> mirror;
	ofPtr<ofxLayerFx::InvertPass> invert;

	ofxPanel pane;

	ofxVJ::OscReceiver oscReceiver;
};
