#pragma once
#include "vjEvents.h"

namespace ofxVJ {
	ofEvent<Events::BangParam> Events::Bang;
	ofEvent<int> Events::sceneStart;
	ofEvent<int> Events::sceneEnd;
	ofEvent<Events::MidiParam> Events::MidiIn;
	ofEvent<int> Events::DrumIn;
	ofEvent<int> Events::Beat;
	ofEvent<int> Events::Bar;
	ofEvent<float> Events::Reverb;
	ofEvent<float> Events::Lowcut;
	ofEvent<float> Events::DelayFeedback;
}