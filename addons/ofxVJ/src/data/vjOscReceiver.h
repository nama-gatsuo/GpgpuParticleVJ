#pragma once
#include "ofxOscReceiver.h"
#include "vjEvents.h"

class ofApp;

namespace ofxVJ {
class OscReceiver {
public:
	OscReceiver() {
		ofxOscReceiverSettings s;
		s.port = 8888;
		receiver.setup(s);
	}

	void update(ofApp& app);

	static float currentTime;
	static int bar;
	static float progress;
	static int beat;
	static float tempo;
	static int midi;
	static int sceneNum;

	static float reverb;
	static float lowcut;
	static float delayFeedback;

private:
	ofxOscReceiver receiver;
	
};
}