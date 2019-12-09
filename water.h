#pragma once
#include "ofMain.h"
#include <cstdlib>

typedef struct _coordinate {
	int x, y;
}coordinate;

class water {
	public:
		water(int num_of_line);
		~water();

	/* ------------------ water line VARIABLES ------------------*/
	// flag
		int water_draw_flag;
		int calc_path_flag;

	// color
		float local_r, local_g, local_b;

	// path
		coordinate* inter_path;
		int num_of_path;


	/* ------------------ water line FUNCTION ------------------*/
	// draw water line
		void draw();

	// calculate coordinate
		void computation(int** line_array, int** dot_array, int num_of_line , int num_of_dots, int dot_idx, int change_flag);
		int distance(int* line_array, int start_x, int start_y, int change_flag);
		int findNearestLine(int** line_array, int num_of_line, int start_x, int start_y);

	// random water line color
		void reset();
};