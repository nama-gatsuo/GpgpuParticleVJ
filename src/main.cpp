#include "ofApp.h"
#include "ofMain.h"


//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.setSize(960, 540);
    ofCreateWindow(settings);
    
	ofRunApp(new ofApp());
}
