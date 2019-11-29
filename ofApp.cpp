#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(15); // Limit the speed of our program to 15 frames per second
    
    // We still want to draw on a black background, so we need to draw
    // the background before we do anything with the brush
    ofBackground(255,255,255);
    ofSetLineWidth(4);
    
    draw_flag = 0;
    load_flag = 0;
	draw_sub_flag = 0;
    dot_diameter = 20.0f;
	num_of_waterline = 10;
}

//--------------------------------------------------------------
void ofApp::update(){
	/*
	ofSetLineWidth(5);
	if (draw_sub_flag) {
		for (unsigned int i = 0; i < waterline.size(); i++) {
			if (!waterline[i].calc_path) waterline[i].computation(line_array, dot_array, num_of_line, num_of_dot, dot_idx);
			waterline[i].draw();
		}

	}
	*/
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(127,23,31);  // Set the drawing color to brown
    
    // Draw shapes for ceiling and floor
    ofDrawRectangle(0, 0, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofDrawRectangle(0, 728, 1024, 40); // Top left corner at (50, 50), 100 wide x 100 high
    ofSetLineWidth(5);
    
    
    ofSetLineWidth(5);
    if( draw_flag ){
        
        /* COMSIL1-TODO 3 : Draw the line segment and dot in which water starts to flow in the screen.
         Note that after drawing line segment and dot, you have to make selected water start dot colored in red.
         */

		for (int i = 0; i < num_of_line; i++) {
			ofSetColor(0, 0, 0);
			ofDrawLine(line_array[i][0], line_array[i][1], line_array[i][2], line_array[i][3]);
		}

		for (int i = 0; i < num_of_dot; i++) {
			if (i == dot_idx) {
				ofSetColor(255, 0, 0);
			}
			else ofSetColor(0, 0, 0);
			
			ofDrawCircle(dot_array[i][0], dot_array[i][1], 10);
		}

        
        
        // 2nd week portion.
        ofSetLineWidth(2);
		if (draw_sub_flag) {
			for (unsigned int i = 0; i < waterline.size(); i++) {
				if (!waterline[i].calc_path) waterline[i].computation(line_array, dot_array, num_of_line, num_of_dot, dot_idx);
				waterline[i].draw();
			}

		}
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'v') {
        // HACK: only needed on windows, when using ofSetAutoBackground(false)
        glReadBuffer(GL_FRONT);
        ofSaveScreen("savedScreenshot_"+ofGetTimestampString()+".png");
    }
    if (key == 'q'){
        // Reset flags
        draw_flag = 0;
        
        // Free the dynamically allocated memory exits.
		
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
        /* COMSIL1-TODO 2: This is draw control part.
        You should draw only after when the key 'd' has been pressed.
        */
    }
    if (key == 's'){
        // water path computation
		if (!load_flag) return;
		if (!water_fall_flag) {
			initializeWaterLines();
		}

		draw_sub_flag = 1;
		water_fall_flag = 1;
    }
    if (key == 'e'){
        // 2nd week portion.
		draw_sub_flag = 0;
		water_fall_flag = 0;

		for (unsigned int i = 0; i < waterline.size(); i++) {
			waterline[i].calc_path = 0;
			waterline[i].inter_path[i].x = waterline[i].inter_path[i].y = -1;
		}
    }
}

//--------------------------------------------------------------
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
    
    /* COMSIL1-TODO 4: This is selection dot control part.
     You can select dot in which water starts to flow by left, right direction key (<- , ->).
     */
	if (!water_fall_flag) {
		if (key == OF_KEY_RIGHT) {
			dot_idx++;
			if (dot_idx >= num_of_dot)
				dot_idx = 0;
			cout << "Selcted Dot Coordinate is (" << dot_idx<<", " << ")" << endl;
		}
		if (key == OF_KEY_LEFT) {
			dot_idx--;
			if (dot_idx < 0)
				dot_idx = num_of_dot - 1;
			cout << "Selcted Dot Coordinate is (" <<dot_idx<< ", " << ")" << endl;
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

	// array �ʱ�ȭ?


    
    string fileName = openFileResult.getName();
    ofFile file(fileName);
    
    if( !file.exists()) cout << "Target file does not exists." << endl;
    else cout << "We found the target file." << endl;
    
    ofBuffer buffer(file);
    
    /* This variable is for indicating which type of input is being received.
     IF input_type == 0, then work of getting line input is in progress.
     IF input_type == 1, then work of getting dot input is in progress.
     */
    int input_type = 0;
	int lineN = 0, dotN = 0;
    
    /* COMSIL1-TODO 1 : Below code is for getting the number of line and dot, getting coordinates.
     You must maintain those information. But, currently below code is not complete.
     Also, note that all of coordinate should not be out of screen size.
     However, all of coordinate do not always turn out to be the case.
     So, You have to develop some error handling code that can detect whether coordinate is out of screen size.
    */
	
    
    // Read file line by line
    for (ofBuffer::Line it = buffer.getLines().begin(), end = buffer.getLines().end(); it != end; ++it) {
        string line = *it;
        
        // Split line into strings
        vector<string> words = ofSplitString(line, " ");
		
        if( words.size() == 1){
            if( input_type == 0){ // Input for the number of lines.
                num_of_line = atoi(words[0].c_str());
                cout << "The number of line is: " << num_of_line << endl;

				line_array = new int*[num_of_line];
				for (int i = 0; i < num_of_line; i++) {
					line_array[i] = new int[4];
				}
            }
            else{ // Input for the number of dots.
                num_of_dot = atoi(words[0].c_str());
                cout << "The number of dot is: " << num_of_dot << endl;

				dot_array = new int*[num_of_dot];
				for (int i = 0; i < num_of_dot; i++) {
					dot_array[i] = new int[2];
				}
            }
        }
		else if (words.size() >= 2) {
			int x1, y1, x2, y2;
			if (input_type == 0) { // Input for actual information of lines
				x1 = atoi(words[0].c_str());
				y1 = atoi(words[1].c_str());
				x2 = atoi(words[2].c_str());
				y2 = atoi(words[3].c_str());

				cout << "line: " << x1 << ' ' << y1 << ' ' << x2 << ' ' << y2 << endl;
				cout << "lineN" << lineN << endl;

				if (x2 < 0 || x2 > ofGetWidth()) {
					return;
				}

				if (y2 < 0 || y2 > ofGetHeight()) {
					return;
				}


			}
			else { // Input for actual information of dots.
				x1 = atoi(words[0].c_str());
				y1 = atoi(words[1].c_str());

				cout << "dots: " << x1 << ' ' << y1 << endl;

			}

			if (x1 < 0 || x1 > ofGetWidth()) {
				return;
			}

			if (y1 < 0 || y1 > ofGetHeight()) {
				return;
			}


			if (input_type == 0) {
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
	int num = 50;
	int local_x, local_y;

	if (waterline.empty()) {
		waterline.assign(num_of_waterline, water(num_of_line));
	}
	
	// find (x,y) of selected dot
	for (int i = 0; i < num_of_dot; i++) {
		if (i == dot_idx) {
			local_x = dot_array[i][0];
			local_y = dot_array[i][1];
			break;
		}
	}
	cout << "water dot_idx" << dot_idx;

	// starting point (x,y)
	for (int i = 0; i < num_of_waterline; i++) {
		waterline[i].inter_path[0].x = local_x;
		waterline[i].inter_path[0].y = local_y;
	}
}


