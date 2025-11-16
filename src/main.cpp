#include "ofApp.h"
#include "ofMain.h"

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

//========================================================================
int main( ){
    ofGLFWWindowSettings settings;
    settings.setGLVersion(4, 1);
    settings.setSize(1280, 720);
    ofCreateWindow(settings);
    
	ofRunApp(new ofApp());
}
