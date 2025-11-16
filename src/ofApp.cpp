#include "ofApp.h"

void ofApp::setup() {

	ofBackground(0);
	ofDisableAlphaBlending();
	ofDisableBlendMode();
	ofSetFrameRate(60);

	pe.resize(1920, 1080);
	complexConv = pe.createPass<ofxLayerFx::ComplexConvPass>();
	feedback = pe.createPass<ofxLayerFx::FeedbackPass>();
	pe.createPass<ofxLayerFx::BurrelBlurChromaPass>();
	mirror = pe.createPass<ofxLayerFx::MirrorPass>();
	invert = pe.createPass<ofxLayerFx::InvertPass>();
	focusBlur = pe.createPass<ofxLayerFx::PsuedoFocusBlurPass>();
	pe.createPass<ofxLayerFx::VignettPass>();
	pe.createPass<ofxLayerFx::ColorCorrection>();

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

	// receiver.setup(8888);

	spoutSender.init("ParticleVJ");

	pane.setup();
	pane.add(pe.getParameters());
	pane.loadFromFile("settings.xml");
}

void ofApp::update() {

	oscReceiver.update(*this);

	scenes[state]->update(dt);
	cam.update(dt);

	pe.begin();
	{
		ofClear(0);
		cam.begin();
		scenes[state]->draw(vol);
		cam.end();
	}
	pe.end(false);


}

void ofApp::draw() {

	pe.getTexture().draw(0, 0, ofGetWidth(), ofGetHeight());
	spoutSender.send(pe.getTexture());
	ofDrawBitmapString("fps:" + ofToString(ofGetFrameRate(), 4), 10, 20);
	ofDrawBitmapString("cam/dist: " + ofToString(cam.getRadius(), 4), 10, 40);
	ofDrawBitmapString("dt: " + ofToString(dt, 4), 10, 60);
	pane.draw();
}

void ofApp::keyPressed(int key) {

	if (key == ' ') scenes[state]->randomize();
	else if (key == '2') {
		setFX(1);
	}
	else if (key == '3') {
		setFX(2);
	}
	else if (key == '4') {
		setFX(3);
	}
	else if (key == '1') {
		setFX(0);
	}
	else if (key == 'b') {
		invert->setEnabled(!invert->isEnabled());
	}
	else if (key == 'q') state = 0;
	else if (key == 'w') state = 1;
	else if (key == 'e') state = 2;
	else if (key == 'r') state = 3;
	else if (key == 't') state = 4;
}

void ofApp::changeScene() {

	int n = sceneList.size();
	if (n == 0) return;
	int coin = (int)floor(ofRandom(n));
	state = sceneList[coin];

}

void ofApp::updateSceneList(int num) {

	vector<int>::iterator it;

	for (it = sceneList.begin(); it < sceneList.end(); ++it) {
		if (*it == num) {
			sceneList.erase(it);
			return;
		}
	}
	sceneList.push_back(num);

}

void ofApp::setFX(int mode) {
	if (mode == 0) {
		mirror->setUseVertical(false);
		mirror->setUseHorizontal(false);
		complexConv->setScale1(0.0f);
		complexConv->setScale2(1.0f);
	}
	else if (mode == 1) {
		mirror->setUseVertical(true);
		mirror->setUseHorizontal(false);
		complexConv->setScale1(0.0f);
		complexConv->setScale2(1.0f);
	}
	else if (mode == 2) {
		mirror->setUseVertical(false);
		mirror->setUseHorizontal(true);
		complexConv->setScale1(0.0f);
		complexConv->setScale2(1.0f);
	}
	else if (mode == 3) {
		mirror->setUseVertical(false);
		mirror->setUseHorizontal(false);
		complexConv->setScale1(1.0f);
		complexConv->setScale2(0.2f);
	}
}

void ofApp::onFeedbackChanged(float& v) {
	if (feedback) {
		feedback->setAlpha(v * 0.98f);
	}
}

void ofApp::onBlurChanged(float& t) {
	if (focusBlur) {
		focusBlur->setAmount(t);
	}

}

void ofApp::onConvChanged(float& t) {
	if (complexConv) {
		complexConv->setScale1(glm::mix(0.0f, 1.0f, ofClamp(t * 2.0, 0, 1)));
		complexConv->setScale2(glm::mix(1.0f, 0.2f, ofClamp(t * 2.0, 0, 1)));
	}

}