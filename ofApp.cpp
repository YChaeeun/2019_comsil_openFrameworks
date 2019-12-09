#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(10); // Limit the speed of our program to 15 frames per second
    
// background
	width = ofGetWidth();
	height = ofGetHeight();

	white.set(255, 255, 255);
	black.set(0, 0, 0);
	brown.set(127, 23, 31);

	ofBackground(white);

// variables _num
	dot_idx = 0;
	dot_diameter = 20.0f;

	num_of_water_line = 10;

// flag
	load_flag = 0;
    draw_flag = 0;
	draw_sub_flag = 0;

	change_line_flag = 0;
	night_mode_flag = 0;

	fall_twice = 0;
	change_twice = 0;
	night_twice = 0;

// function
	setBackground();
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (night_mode_flag) {
		ofBackground(black);

		drawStar(ofRandom(100, width - 100), ofRandom(40, height - 300));
		drawStar(60, 120);
		drawStar(250, 200);
		drawStar(500, 100);
		drawStar(900, 150);
	}
	else {
		ofBackground(white);
	}

	if (draw_flag) {
		drawLine();
		drawDots();

		if (change_line_flag) {
			changeLine();
		}
		
		if (draw_sub_flag) {
			drawWaterStream();
		}
	}

	ofSetColor(brown);
	background.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        draw_flag = 0;
        
		for (int i = 0; i < num_of_line; i++) {
			delete line_array[i];
		}
		delete line_array;

		for (int i = 0; i < num_of_dot; i++) {
			delete dot_array[i];
		}
		delete dot_array;

        cout << "Dynamically allocated memory has been freed." << endl;
        
        _Exit(0);
    }
    if (key == 'd'){
        if(!load_flag) return;
        
		draw_flag = 1;       
    }
    if (key == 's'){
		if (!load_flag) return;
		if (!water_fall_flag) initializeWaterLines();

		checkTwice(fall_twice, water_fall_flag, draw_sub_flag);
    }
    if (key == 'e'){
		if (!load_flag) return;
		if (water_fall_flag) return;

		checkTwice(change_twice, change_line_flag);
    }
	if (key == 'n') {
		if (!load_flag) return;

		checkTwice(night_twice, night_mode_flag);
	}
}

void ofApp::keyReleased(int key){
    if( key == 'l'){
        // Open the Open File Dialog
        ofFileDialogResult openFileResult= ofSystemLoadDialog("Select a only txt for Waterfall");
        
        // Check whether the user opened a file
        if( openFileResult.bSuccess){
            ofLogVerbose("User selected a file");
            
            // We have a file, so let's check it and process it
            processOpenFileSelection(openFileResult);
            load_flag = 1;
        }
    }
    
	if (!water_fall_flag) {
		if (key == OF_KEY_RIGHT) {
			dot_idx++;
			if (dot_idx >= num_of_dot)
				dot_idx = 0;
		}
		if (key == OF_KEY_LEFT) {
			dot_idx--;
			if (dot_idx < 0)
				dot_idx = num_of_dot - 1;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
 
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::processOpenFileSelection(ofFileDialogResult openFileResult) { 
    //Path to the comma delimited file
    //string fileName = "input.txt";
    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
   
    int input_type = 0;
	int lineN = 0, dotN = 0;
      
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
		
        if( words.size() == 1){
            if( !input_type){ // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());

				line_array = new int*[num_of_line];
				for (int i = 0; i < num_of_line; i++) {
					line_array[i] = new int[4];
				}
            }
            else{ // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());

				dot_array = new int*[num_of_dot];
				for (int i = 0; i < num_of_dot; i++) {
					dot_array[i] = new int[2];
				}
            }
        }
		else if (words.size() >= 2) {
			int x1, y1, x2, y2;
			if (!input_type) { // Input for actual information of lines
				x1 = atoi(words[0].c_str());
				y1 = atoi(words[1].c_str());
				x2 = atoi(words[2].c_str());
				y2 = atoi(words[3].c_str());

				if (x2 < 0 || x2 > ofGetWidth()) return;
				if (y2 < 0 || y2 > ofGetHeight()) return;
			}
			else { // Input for actual information of dots.
				x1 = atoi(words[0].c_str());
				y1 = atoi(words[1].c_str());
			}

			if (x1 < 0 || x1 > ofGetWidth()) return;
			if (y1 < 0 || y1 > ofGetHeight()) return;

			if (!input_type) {
				line_array[lineN][0] = x1;
				line_array[lineN][1] = y1;
				line_array[lineN][2] = x2;
				line_array[lineN][3] = y2;
				lineN += 1;
			}
			else {
				dot_array[dotN][0] = x1;
				dot_array[dotN][1] = y1;
				dotN += 1;
			}

			if (lineN > num_of_line - 1) {
				input_type = 1;
			}

		} // End of else if.
    } // End of for-loop (Read file line by line).
    
    initializeWaterLines();
}

void ofApp::initializeWaterLines() {
	int local_x, local_y;

	if (water_line.empty()) {
		water_line.assign(num_of_water_line, water(num_of_line));
	}
	else {
		for (unsigned int i = 0; i < water_line.size(); i++) {
			water_line[i].calc_path_flag = 0;
			water_line[i].inter_path[i].x = water_line[i].inter_path[i].y = -1;
		}
	}

	// find (x,y) of selected dot
	for (int i = 0; i < num_of_dot; i++) {
		if (i == dot_idx) {
			local_x = dot_array[i][0];
			local_y = dot_array[i][1];
			break;
		}
	}

	// starting point (x,y)
	for (int i = 0; i < num_of_water_line; i++) {
		water_line[i].inter_path[0].x = local_x;
		water_line[i].inter_path[0].y = local_y;
	}
}

void ofApp::drawLine()
{
	ofSetLineWidth(5);
	for (int i = 0; i < num_of_line; i++) {
		ofSetColor(black);
		if (night_mode_flag || change_line_flag) ofSetColor(white);
		if (night_mode_flag && change_line_flag) ofSetColor(black);
		ofDrawLine(line_array[i][0], line_array[i][1], line_array[i][2], line_array[i][3]);
	}
}

void ofApp::drawDots()
{
	for (int i = 0; i < num_of_dot; i++) {
		if (i == dot_idx) {
			ofSetColor(255, 0, 0);
		}
		else {
			ofSetColor(black);
			if (night_mode_flag) ofSetColor(white);
		}
		ofDrawCircle(dot_array[i][0], dot_array[i][1], 10);
	}
}

void ofApp::drawWaterStream()
{
	ofSetLineWidth(3);
	for (unsigned int i = 0; i < water_line.size(); i++) {
		if (!water_line[i].calc_path_flag)
			water_line[i].computation(line_array, dot_array, num_of_line, num_of_dot, dot_idx, change_line_flag);
		water_line[i].draw();
	}
}

void ofApp::checkTwice(int& check, int& flag1, int& flag2)
{
	if (check) {
		flag1 = flag2 = check = 0;
	}
	else {
		flag1 = flag2 = check = 1;
	}
}

void ofApp::checkTwice(int& check, int& flag)
{
	if (check) {
		check = flag = 0;
	}
	else {
		check = flag = 1;
	}
}

void ofApp::setBackground()
{
	background.allocate(width, height);
	background.begin();

	ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
	ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high

	background.end();
}

void ofApp::changeLine()
{
	for (int i = 0; i < num_of_line; i++) {
		ofSetColor(white);
		if (night_mode_flag || change_line_flag) ofSetColor(black);
		if (night_mode_flag && change_line_flag) ofSetColor(white);
		ofDrawLine(line_array[i][0], line_array[i][3], line_array[i][2], line_array[i][1]);
	}
}

void ofApp::drawStar(int x, int y)
{
	ofSetLineWidth(1);

	int numLines = 50;
	int minRadius = 15;
	int maxRadius = 80;
	for (int i = 0; i < numLines; i++) {
		
		float angle = ofRandom(ofDegToRad(360.0));
		float distance = ofRandom(minRadius, maxRadius);
		float xOffset = cos(angle) * distance;
		float yOffset = sin(angle) * distance;

		float alpha = ofMap(distance, minRadius, maxRadius, 50, 0); // Make shorter lines more opaque
		ofSetColor(255, alpha);

		ofSetLineWidth(ofRandom(1.0, 5.0)); // Remember, this doesn't work on all graphics cards
		ofDrawLine(x, y, x + xOffset, y+ yOffset);
	}
}