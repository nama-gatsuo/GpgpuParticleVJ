#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 5);
    settings.setSize(960, 540);
    ofCreateWindow(settings);
    
	ofRunApp(new ofApp());
}
