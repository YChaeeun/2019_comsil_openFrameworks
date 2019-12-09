#pragma once

#include "ofMain.h"
#include <vector>
#include "water.h"

class ofApp : public ofBaseApp {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

	/* ------------------ Background ------------------*/
		int width;
		int height;

		ofFbo background;

		ofColor white;
		ofColor black;
		ofColor brown;
		ofColor red;

		void setBackground();

    /* ------------------ WATER FALL BASIC ------------------*/
	// flag variables
		int draw_flag;
		int load_flag;
		int draw_sub_flag;
		int water_fall_flag;
		int fall_twice;

	// Line segment and dot related variables
		int num_of_line, num_of_dot;
		float dot_diameter;
		int dot_idx;

		int** line_array;
		int** dot_array;

		int num_of_water_line;
		vector<water> water_line;
    
	// member function
		void processOpenFileSelection(ofFileDialogResult openFileResult);
		void initializeWaterLines();

		void drawLine();
		void drawDots();
		void drawWaterStream();

		void checkTwice(int& check, int& flag1, int& flag2);
		void checkTwice(int& check, int& flag);

	/* ------------------ CHANGE LINE MODE ------------------*/
		int change_line_flag;
		int change_twice;

		void changeLine();

	/* --------------------- NIGHT MODE ---------------------*/
		int night_mode_flag;
		int night_twice;
		
		void drawStar(int x, int y);
};