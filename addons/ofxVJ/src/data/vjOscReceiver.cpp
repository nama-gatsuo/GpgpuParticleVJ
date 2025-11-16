#include "ofApp.h"
#include "vjOscReceiver.h"

namespace ofxVJ {
	float OscReceiver::currentTime = 0;
	int OscReceiver::bar = 0;
	float OscReceiver::progress = 0;
	int OscReceiver::beat = 0;
	float OscReceiver::tempo = 0;
	int OscReceiver::midi = 0;

	float OscReceiver::reverb = 0;
	float OscReceiver::lowcut = 0;
	float OscReceiver::delayFeedback = 0;
	
	int OscReceiver::sceneNum = 0;

	void OscReceiver::update(ofApp& app) {

		while (receiver.hasWaitingMessages()) {

			ofxOscMessage m;
			receiver.getNextMessage(m);
			const auto& addr = m.getAddress();
			const auto& addrPaths = ofSplitString(addr, "/", true);
			if (addrPaths.empty()) continue;

			if (addrPaths[0] == "blur") {
				reverb = m.getArgAsFloat(0);
				ofNotifyEvent(ofxVJ::Events::Reverb, reverb);
				app.onBlurChanged(reverb);
			}
			else if (addrPaths[0] == "conv") {
				lowcut = m.getArgAsFloat(0);
				ofNotifyEvent(ofxVJ::Events::Lowcut, lowcut);
				app.onConvChanged(lowcut);
			}
			else if (addrPaths[0] == "feedback") {
				delayFeedback = m.getArgAsFloat(0);
				ofNotifyEvent(ofxVJ::Events::DelayFeedback, delayFeedback);
				app.onFeedbackChanged(delayFeedback);
			}
			else if (addrPaths[0] == "p") {

				int i = ofToInt(addrPaths[1]);
				int val = m.getArgAsInt(0);

				if (i == 3) app.cam.setRadius(1.0 + 1000.0 * val / 128.0);
				else if (i == 4) app.dt = 0.001 + 0.04 * val / 128.0;
				else app.scenes[app.state]->setParam(i, val);

			}
			else if (addrPaths[0] == "scene") { app.state = ofClamp(m.getArgAsInt(0), 0, app.scenes.size() - 1); }
			else if (addrPaths[0] == "volume") app.vol = m.getArgAsFloat(0);
			else if (addrPaths[0] == "bang") {
				int i = m.getArgAsInt(0);
				if (i == 0) {
					app.scenes[app.state]->randomize();
					// if (ofRandom(1.0) < 0.1) changeScene();
				}
				else if (i == 1) {
					app.setFX(floor(ofRandom(0, 4)));
					if (app.invert->isEnabled()) {
						if (ofRandom(1.0f) > 0.4) app.invert->setEnabled(false);
					}
					else {
						if (ofRandom(1.0f) > 0.9) app.invert->setEnabled(true);
					}
				}

			}
			else if (addrPaths[0] == "dt") {
				float f = m.getArgAsFloat(0);
				app.setDt(f);
			}
			else if (addrPaths[0] == "cam" && addrPaths[1] == "dist") {
				float dist = m.getArgAsFloat(0);
				app.setRadius(dist);
			}
		}
	}
}
