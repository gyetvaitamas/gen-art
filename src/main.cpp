#include "ofMain.h"
#include "ofApp.h"

int main( ){
    ofGLFWWindowSettings settings;
    settings.visible = true;
	settings.resizable = false;
    ofCreateWindow(settings);
    ofRunApp(new ofApp);
}
