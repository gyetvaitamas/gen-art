/*
	#############################################################
    #															#
	#	gen-art on GitHub:										#
	#	https://github.com/gyetvaitamas/gen-art					#
	#															#
	#	Original idea / credit:									#
	#	https://github.com/dvalim/art-automata					#
	#															#
	#############################################################
*/

#include "ofApp.h"
#include <random>

ofFbo buffer;
std::string seedstring;
std::mt19937 engine;

// These could be changed on the panel, however width and height should be constant and to be set here for states[width][height]
// These are also the default width/height values if you click on the refresh button on the panel
const int width = 4000;
const int height = 4000;
int states[width][height];

// Gaussian calculation
double gaussian(double mean, double deviation) {
    std::normal_distribution<double> nd(mean, deviation);
    return nd(engine);
}

// Getting states
int getState(int n, int x, int y, int _res) {
    int l = 0, m, r = 0;
	int res = _res;
    if(x > 0) l = states[x - 1][y];
    if(x < res - 1) r = states[x + 1][y];
    m = states[x][y];
    
    int pos = (l << 2) + (m << 1) + r;
    return (n >> pos) & 1;
}

// Making some noise
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

// Saving to file
void saveSession() {
	ofPixels pix;
	buffer.readToPixels(pix);
	ofSaveImage(pix, "../images/" + seedstring + ".jpg");
}

void ofApp::setup() {
	// Setting up GUI panel
	gui.setup("Config panel");

	int windowWidth = 300;															// Set window width
	int windowHeight = 300;															// Set window height
	ofSetWindowShape(windowWidth, windowHeight);									// Set window shape
	gui.setDefaultWidth(windowWidth);												// Set panel width = window width, so it's fill the window properly
	gui.setPosition(0, 0);															// Set panel position to zero (left upper corner)
	ofSetColor(ofColor::white);														// Set panel color

	// Adding elements to the panel
	gui.add(widthField.setup("Width (px)", 4000, 0, 12000));						// Image width in pixels. Might crash over 12,000 px!
	gui.add(heightField.setup("Heigth (px)", 4000, 0, 12000));						// Image height in pixels. Might crash over 12,000 px!
	gui.add(resolutionField.setup("Resolution", 1000, 0, 4000));					// Image resolutuion. The resolution value basically is how many square unit should be draw on the image. Try to find the best setting based on the width and height values.
	gui.add(noiseSeedField.setup("Noise seed", 100, 0, 10000));						// For getNoise() funciton, it gives some extra simplicity or more details based on the value.
	gui.add(coordScaleField.setup("Coordinate scale", 3, 1, 10000));				// Scale of the sample(s) on the image, less number results less scale, large number results large scale of sample(s).
	gui.add(gaussianNoiseField.setup("Gaussian noise", 0.0001, 0.000001, 0.1));		// For getNoise() funciton, it gives some extra simplicity or more details based on the value.
	gui.add(rulesCountField.setup("Rules count", 24, 2, 99));						// How many rules should be used, less number results less complex sample(s), large number results more complex sample(s).
	gui.add(rulesMinValueField.setup("Rules - minimum value", 150, 0, 256));		// Lower limit of the rule for generating random value. Might result boring samples under value 150.
	gui.add(rulesMaxValueField.setup("Rules - maximum value", 200, 0, 256));		// Upper limit of the rule for generating random value. Might result boring samples above value 200.
	gui.add(predefinedRules.setup("<- Use pre-defined rules", false));				// Using the pre-defined rules what you adding in the source
	gui.add(hardRandomizeButton.setup("<- Hard randomize"));						// Wildcard values. Might result bad / boring samples.
	gui.add(processButton.setup("<- Process image"));								// Make the image and save to ..\bin\images\ folder

	configFile.open("configs.txt", ofFile::Append);									// Open text file for logging
}

void ofApp::draw() {
	gui.draw();																		// Spawn the GUI panel

	// Hard randomization (randomizing every input)									//
	if (hardRandomizeButton) {														//
		noiseSeedField = ofRandom(0, 1000);											//
		coordScaleField = ofRandom(1, 100);											//
		gaussianNoiseField = ofRandom(0.000001, 0.1);								// Personalize randomization here
		coordScaleField = ofRandom(1, 50);											//
		rulesCountField = ofRandom(2, 99);											//
		rulesMinValueField = ofRandom(150, 230);									//
		rulesMaxValueField = ofRandom(230, 256);									//
	}

	// Processing the image
	if (processButton) {
		// Logging date/time
		cout << "Date: \t\t\t" << ofGetTimestampString("%Y-%m-%d %r") << endl;
		configFile << "Date: \t\t\t\t" << ofGetTimestampString("%Y-%m-%d %r") << endl;

		// Generating seed
		int seed = std::chrono::system_clock::now().time_since_epoch().count();
		ofSeedRandom(seed);
		engine.seed(seed);

		// Generating the filename
		std::stringstream sstream;
		sstream << std::hex << seed;
		seedstring = sstream.str();
		cout << "Filename: \t\t" << seedstring << ".jpg" <<endl;
		configFile << "Filename: \t\t\t" << seedstring << ".jpg" << endl;

		// Setting up dimensions and resolution
		int width = widthField;
		int height = heightField;
		buffer.allocate(width, height);
		int res = resolutionField;
		cout << "Dimensions: \t\t" << width << "x" << height << "px (Resolution: " << res << ")" << endl;
		configFile << "Dimensions: \t\t" << width << "x" << height << "px (Resolution: " << res << ")" << endl;

		// Setting up coordinate scale
		double coord_scale = coordScaleField;
		cout << "Coord. scale: \t\t" << coord_scale << endl;
		configFile << "Coord. scale: \t\t" << coord_scale << endl;

		// Setting up noise seed for getNoise() function
		double noise_seed = noiseSeedField;
		cout << "Noise seed: \t\t" << noise_seed << endl;
		configFile << "Noise seed: \t\t" << noise_seed << endl;

		// Setting up guassian noise for getNoise() function
		double gaussian_noise = gaussianNoiseField;
		cout << "Gaussian noise: \t" << gaussian_noise << endl;
		configFile << "Gaussian noise: \t" << gaussian_noise << endl;

		// Setting up the number of rules
		int rules_count = rulesCountField;

		// Setting up states
		if (predefinedRules) {
			// Preventing shape deformation and using pre-defined values. Minimizing randomization
			cout << "Pre-defined rules: \t" << "Enabled... using pre-defined" << endl;
			configFile << "Pre-defined rules: \t" << "Enabled... using pre-defined" << endl;

			for (int x = 0; x < res; x++) states[x][0] = 0;
			
			// Populating rules with pre-defined ones
			vector<int> rules = {
				// Pre-defined rules should be added above:
				172, 219, 233, 191, 174, 172, 213, 230, 180, 231, 230, 158, 218, 177, 190, 229, 203, 193, 201, 178, 226, 165, 171, 208, 199, 200, 203, 192, 209, 210, 233, 233, 197, 179, 233, 207, 224, 230, 214, 175, 193, 212, 166, 167, 207, 225, 199, 175, 176, 214, 225, 227, 191, 204, 171, 170, 198, 188, 214, 163, 159, 206, 193, 222, 189, 172, 170, 224, 200, 168, 165, 222, 201, 188, 179, 164, 226, 207, 183, 157, 164, 212, 164, 192, 225, 183, 230, 157, 233, 222, 203, 184, 157, 166, 166, 224
			};

			// Ignoring the panel's rules count value, setting array size instead
			rules_count = rules.size();

			// Logging rules count
			cout << "Rules count: \t\t" << rules_count << endl;
			configFile << "Rules count: \t\t" << rules_count << endl;

			// Logging rule minimum value
			cout << "Rules min.: \t\t" << "n/a" << endl;
			configFile << "Rules min.: \t\t" << "n/a" << endl;

			// Logging rule maximum value
			cout << "Rules max.: \t\t" << "n/a" << endl;
			configFile << "Rules max.: \t\t" << "n/a" << endl;

			// Logging the rules
			cout << "Rules:" << endl;
			configFile << "Rules:" << endl;
			for (int i = 0; i < rules_count; i++) {
				if (i == rules_count - 1) {
					cout << rules.at(i) << endl << endl;
					configFile << rules.at(i) << endl << endl;
				}
				else {
					cout << rules.at(i) << ", ";
					configFile << rules.at(i) << ", ";
				}
			}

			// Populating states with the pre-defined rules
			for (int y = 0; y < res; y++)
				for (int x = 0; x < res; x++) {
					double noise = getNoise(x, y, res, noise_seed, coord_scale, gaussian_noise);
					int ind = (rules_count - 1) * noise;
					states[x][y] = getState(rules[ind], x, y - 1, res);
				}
		} else {
			// Giving more randomness
			cout << "Pre-defined rules: \t" << "Disabled... using random values" << endl;
			configFile << "Pre-defined rules: \t" << "Disabled... using random values" << endl;

			for (int x = 0; x < res; x++) states[x][0] = (ofRandom(1) <= 0.5 ? 1 : 0);
			
			// Populating rules within the given interval
			vector<int> rules = {
				(int)ofRandom(rulesMinValueField, rulesMaxValueField)
			};

			for (int i = 1; i < rules_count; i++) rules.push_back(ofRandom(rulesMinValueField, rulesMaxValueField));

			// Logging rules count
			cout << "Rules count: \t\t" << rules_count << endl;
			configFile << "Rules count: \t\t" << rules_count << endl;

			// Logging rule minimum value
			cout << "Rules min.: \t\t" << rulesMinValueField << endl;
			configFile << "Rules min.: \t\t" << rulesMinValueField << endl;

			// Logging rule maximum value
			cout << "Rules max.: \t\t" << rulesMaxValueField << endl;
			configFile << "Rules max.: \t\t" << rulesMaxValueField << endl;
			
			// Logging the rules
			cout << "Rules:" << endl;
			configFile << "Rules:" << endl;
			for (int i = 0; i < rules_count; i++) {
				if (i == rules_count - 1) {
					cout << rules.at(i) << endl << endl;
					configFile << rules.at(i) << endl << endl;
				}
				else {
					cout << rules.at(i) << ", ";
					configFile << rules.at(i) << ", ";
				}
			}

			// Populating with the generated random rules
			for (int y = 0; y < res; y++)
				for (int x = 0; x < res; x++) {
					double noise = getNoise(x, y, res, noise_seed, coord_scale, gaussian_noise);
					int ind = (rules_count - 1) * noise;
					states[x][y] = getState(rules[ind], x, y - 1, res);
				}
		}

		// Image defaults
		buffer.begin();																// Starting the buffer
		ofBackground(255);															// Set image background color
		ofSetColor(0);																// Set image foreground color

		// Drawing with the given setups
		double cell_size = width / res;
		for (double y = 0, y_ind = 0; y_ind < res; y += cell_size, y_ind++)
			for (double x = 0, x_ind = 0; x_ind < res; x += cell_size, x_ind++) {
				if (states[(int)x_ind][(int)y_ind]) {
					ofDrawRectangle(x, y, cell_size, cell_size);
					double noise = getNoise(res - x_ind, res - y_ind, res, noise_seed, coord_scale, gaussian_noise);
				}
			}

		// Close the buffer, then save the session
		buffer.end();																// Closing the buffer
		cout << endl << endl;
		configFile << endl << endl;
		saveSession();																// Saving the image
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