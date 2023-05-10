#pragma once
#include "ofEvents.h"

namespace ofxVJ {
	class Events {
	public:

		// Bang (attack detection from audio processing)
		struct BangParam {
			int index;
			float volumeJump;
		};
		static ofEvent<BangParam> Bang;

		// Scene change
		static ofEvent<int> sceneStart;
		static ofEvent<int> sceneEnd;

		struct MidiParam {
			int midi, velocity;
		};
		static ofEvent<MidiParam> MidiIn;
		
		static ofEvent<int> DrumIn;
		static ofEvent<int> Beat;
		static ofEvent<int> Bar;

		static ofEvent<float> Reverb;
		static ofEvent<float> Lowcut;
		static ofEvent<float> DelayFeedback;
	};
}
