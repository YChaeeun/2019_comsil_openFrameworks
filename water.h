#pragma once
#include "ofMain.h"
#include <cstdlib>

#define EPSIL 0.01
typedef struct _coordinate {
	int x, y;
}coordinate;

class water{
public:
	water(int num_of_line);
	~water();
	
	// draw
	void draw();
	void reset();

	// compute and fine coordinates
	void computation(int** line_array, int** dot_array, int num_of_line , int num_of_dots, int dot_idx);
	int distance(int* line_array, int start_x, int start_y);
	int find_nearest(int** line_array, int num_of_line, int start_x, int start_y);
	
	void update();

	float local_r, local_g, local_b;
	int target_1, target_2;


	coordinate* inter_path;
	int num_of_path;

	int calc_path;
	int water_draw_flag;
};
