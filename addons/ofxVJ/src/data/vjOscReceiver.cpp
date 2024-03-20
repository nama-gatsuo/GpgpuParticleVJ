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

			if (addrPaths[0] == "currentTime") {
				int id = m.getArgAsFloat(0);
				currentTime = m.getArgAsFloat(0);
			}
			else if (addrPaths[0] == "drum") {
				int id = m.getArgAsInt(0);
				ofNotifyEvent(ofxVJ::Events::DrumIn, id);
			}
			else if (addrPaths[0] == "bar") {
				bar = m.getArgAsInt(0);
				ofNotifyEvent(ofxVJ::Events::Bar, bar);
			}
			else if (addrPaths[0] == "progress") {
				float newProg = m.getArgAsFloat(0);
				if (abs(newProg - progress) > 0.9) {
					sceneNum++;
					ofNotifyEvent(ofxVJ::Events::sceneStart, sceneNum);
				}
				progress = newProg;
			}
			else if (addrPaths[0] == "beat") {
				beat = m.getArgAsInt(0);
				ofNotifyEvent(ofxVJ::Events::Beat, beat);
			}
			else if (addrPaths[0] == "tempo") {
				tempo = m.getArgAsInt(0);
			}
			else if (addrPaths[0] == "reverb_mix") {
				reverb = m.getArgAsFloat(0);
				ofNotifyEvent(ofxVJ::Events::Reverb, reverb);
				app.onReverbChanged(reverb);
			}
			else if (addrPaths[0] == "filter_freq") {
				lowcut = m.getArgAsFloat(0);
				ofNotifyEvent(ofxVJ::Events::Lowcut, lowcut);
				app.onFilterChanged(lowcut);
			}
			else if (addrPaths[0] == "delay_feedback") {
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
			else if (addrPaths[0] == "vol") app.vol = m.getArgAsFloat(0);
			else if (addrPaths[0] == "bang") {
				ofLog() << "bang";
				if (addrPaths[1] == "0") {
					app.scenes[app.state]->randomize();
					// if (ofRandom(1.0) < 0.1) changeScene();
				}
				else if (addrPaths[1] == "1") {
					app.setFX(floor(ofRandom(0, 4)));
					if (app.invert->isEnabled()) {
						if (ofRandom(1.0f) > 0.4) app.invert->setEnabled(false);
					}
					else {
						if (ofRandom(1.0f) > 0.9) app.invert->setEnabled(true);
					}
				}

			}
		}
	}
}
