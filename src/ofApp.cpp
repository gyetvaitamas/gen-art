#include "ofApp.h"
#include <random>

ofFbo buffer;
std::string seedstring;
std::mt19937 engine;

// TODO: escape it from here. Useless, as it has user input below, however width and height should be constant for states[width][height]. RIP
const int width = 4000;
const int height = 4000;
int states[width][height];

// Gaussian calculation
double gaussian(double mean, double deviation) {
    std::normal_distribution<double> nd(mean, deviation);
    return nd(engine);
}

// Don't-touch-it part, getting states 
int getState(int n, int x, int y, int _res) {
    int l = 0, m, r = 0;
	int res = _res;
    if(x > 0) l = states[x - 1][y];
    if(x < res - 1) r = states[x + 1][y];
    m = states[x][y];
    
    int pos = (l << 2) + (m << 1) + r;
    return (n >> pos) & 1;
}

// Make some noise
// TODO: moar tweak options here
double getNoise(int _x, int _y, int _res, double _noise_seed, double _coord_scale, double _gaussian_noise) {
	int res = _res;
    double x = _x, y = _y;
	double noise_seed = _noise_seed;
	double coord_scale = _coord_scale;
	double gaussian_noise = _gaussian_noise;
    double noise = ofNoise(x/res * coord_scale, y/res * coord_scale, noise_seed);
    noise += 0.5 * ofNoise(x/res * coord_scale * 2, y/res * coord_scale * 2, noise_seed);
    noise += 0.25 * ofNoise(x/res * coord_scale * 4, y/res * coord_scale * 4, noise_seed);
    noise += 0.125 * ofNoise(x/res * coord_scale * 8, y/res * coord_scale * 8, noise_seed);
    noise = min(1.0, noise * gaussian(1, gaussian_noise));
    return noise;
}

void saveSession() {
	ofPixels pix;
	buffer.readToPixels(pix);
	ofSaveImage(pix, "../images/" + seedstring + ".jpg");
}

void ofApp::setup() {
	// "Settings" window size
	int windowWidth = 300;
	int windowHeight = 250;
	ofSetWindowShape(windowWidth, windowHeight);

	gui.setup();

	// Panel width and positioning
	gui.setDefaultWidth(windowWidth);
	gui.setPosition(0, 0);

	// Adding elements to the panel
	gui.add(label.setup("", "Settings"));
	gui.add(widthField.setup("Width (px)", 4000, 0, 12000));
	gui.add(heightField.setup("Heigth (px)", 4000, 0, 12000));
	gui.add(resolutionField.setup("Resolution", 1000, 0, 4000));
	gui.add(noiseSeedField.setup("Noise seed", 1000, 0, 10000));
	gui.add(coordScaleField.setup("Coordinate scale", 3, 1, 10000));
	gui.add(gaussianNoiseField.setup("Gaussian noise", 0.0001, 0.000001, 0.1));
	gui.add(rulesCountField.setup("Rules count", 24, 2, 100));
	gui.add(rulesMinValueField.setup("Rules - minimum value", 150, 0, 256));
	gui.add(rulesMaxValueField.setup("Rules - maximum value", 200, 0, 256));
	gui.add(processButton.setup("<- Process image"));

	// Open config file for append text
	configFile.open("configs.txt", ofFile::Append);
}

void ofApp::draw() {
	// Panel color
	ofSetColor(ofColor::white);

	gui.draw();

	// Process button fired, then
	if (processButton) {
		// Generate seed
		int seed = std::chrono::system_clock::now().time_since_epoch().count();
		ofSeedRandom(seed);
		engine.seed(seed);

		// This will be the name of the image
		std::stringstream sstream;
		sstream << std::hex << seed;
		seedstring = sstream.str();
		cout << "Seed string: " << "\t" << seedstring <<endl;
		configFile << "Seed string: " << "\t" << seedstring << endl;

		// Set up dimensions and resolution of the image
		int width = widthField;
		int height = heightField;
		buffer.allocate(width, height);
		int res = resolutionField;
		cout << "Dimensions: " << "\t" << width << "x" << height << "px (" << res << " resolution)" << endl;
		configFile << "Dimensions: " << "\t" << width << "x" << height << "px (" << res << " resolution)" << endl;

		// Set up coordinate scale
		double coord_scale = coordScaleField;
		cout << "Coord. scale: " << "\t" << coord_scale << endl;
		configFile << "Coord. scale: " << "\t" << coord_scale << endl;

		// Set up noise seed for getNoise() function
		double noise_seed = noiseSeedField;
		cout << "Noise seed: " << "\t" << noise_seed << endl;
		configFile << "Noise seed: " << "\t" << noise_seed << endl;

		// Set up guassian noise for getNoise() function
		double gaussian_noise = gaussianNoiseField;
		cout << "Gaussian noise:" << " " << gaussian_noise << endl;
		configFile << "Gaussian noise:" << " " << gaussian_noise << endl;

		// Set up the number of rules
		int rules_count = rulesCountField;

		// Rule minimum value
		cout << "Rules min.: " << "\t" << rulesMinValueField << endl;
		configFile << "Rules min.: " << "\t" << rulesMinValueField << endl;

		// Rule maximum value
		cout << "Rules max.: " << "\t" << rulesMaxValueField << endl;
		configFile << "Rules max.: " << "\t" << rulesMaxValueField << endl;

		cout << "Rules (" << rules_count << "): " << endl;
		configFile << "Rules (" << rules_count << "): " << endl;

		// Populate rules within the given interval
		vector<int> rules = {
			// If you want to use pre-generated rules, just write here, as example:
			// 188, 162, 157, 195, 175, 198, 186, 154, 162, 190, 168, 174, 172, 158, 164, 150, 156, 150, 197, 187, 162, 178, 157, 158
			// and comment this line below:
			(int)ofRandom(rulesMinValueField, rulesMaxValueField)
		};  //
			// and also comment this line below:
		for (int i = 1; i < rules_count; i++) rules.push_back(ofRandom(rulesMinValueField, rulesMaxValueField));
		for (int i = 0; i < rules_count; i++) {
			if (i == rules_count - 1) {
				cout << rules.at(i) << endl << endl;
				configFile << rules.at(i) << endl << endl;
			} else {
				cout << rules.at(i) << ", ";
				configFile << rules.at(i) << ", ";
			}
		}

		// Set up states, keeping it random for uniqueness, for art
		for (int x = 0; x < res; x++) states[x][0] = (ofRandom(1) <= 0.5 ? 1 : 0);
		for (int y = 1; y < res; y++)
			for (int x = 0; x < res; x++) {
				double noise = getNoise(x, y, res, noise_seed, coord_scale, gaussian_noise);
				int ind = (rules_count - 1) * noise;
				states[x][y] = getState(rules[ind], x, y - 1, res);
			}

		// Start buffer, set up background and foreground color of the image
		buffer.begin();
		ofBackground(255);
		ofSetColor(0);

		// Draw with the given instructions
		double cell_size = width / res;
		for (double y = 0, y_ind = 0; y_ind < res; y += cell_size, y_ind++)
			for (double x = 0, x_ind = 0; x_ind < res; x += cell_size, x_ind++) {
				if (states[(int)x_ind][(int)y_ind]) {
					ofDrawRectangle(x, y, cell_size, cell_size);
					double noise = getNoise(res - x_ind, res - y_ind, res, noise_seed, coord_scale, gaussian_noise);
				}
			}

		// Close buffer, then save the session
		buffer.end();
		cout << endl << endl;
		configFile << endl << endl;
		saveSession();
	}
}

void ofApp::update() {}
void ofApp::keyPressed(int key){}
void ofApp::keyReleased(int key){}
void ofApp::mouseMoved(int x, int y ){}
void ofApp::mouseDragged(int x, int y, int button){}
void ofApp::mousePressed(int x, int y, int button){}
void ofApp::mouseReleased(int x, int y, int button){}
void ofApp::mouseEntered(int x, int y){}
void ofApp::mouseExited(int x, int y){}
void ofApp::windowResized(int w, int h){}
void ofApp::gotMessage(ofMessage msg){}
void ofApp::dragEvent(ofDragInfo dragInfo){}
