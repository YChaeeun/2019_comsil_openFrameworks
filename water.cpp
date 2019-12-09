#include "water.h"
#include <math.h>
#include <cstdlib>
#define MIN(x,y) (((x)<(y))? (x):(y))
#define MAX(x,y) (((x)>(y))? (x):(y))

#define IDX(x,y) ((x>y)? 0:2)
#define CHG_IDX(x,y) ((x>y)? 2:0)

#define SLOPE(x1,y1,x2,y2)((float)((y1)-(y2))/((x1)-(x2)))

water::water(int num_of_line)
{
	num_of_path = num_of_line * 2 + 2;
	if (inter_path) {
		inter_path = (coordinate*)malloc(sizeof(coordinate)*num_of_path+1); //  line °³¼ö
		for (int i = 0; i < num_of_path; i++) {
			inter_path[i].x = inter_path[i].y = -1;
		}
	}
	
	calc_path_flag = 0;
}

water::~water()
{
	if (!inter_path) {
		free(inter_path);
	}
}

void water::draw()
{
	reset();
	ofSetColor(local_r, local_g, local_b);
	
	if (calc_path_flag) {
		for (int i = 0; i < num_of_path - 1; i++) {
			
			float radius = ofRandom(-10, 10);
			float xOffset = ofRandom(-25, 25);
			float yOffset = ofRandom(-30, 20);
			float width = ofRandom(6, 12);

			if (inter_path[i].x < ofGetWidth() && width < ofGetWidth() && inter_path[i].y < ofGetHeight() && radius < ofGetHeight()) {
				ofDrawLine(inter_path[i].x + radius, inter_path[i].y, inter_path[i + 1].x + radius, inter_path[i + 1].y);
				ofDrawCircle(inter_path[i + 1].x + xOffset, inter_path[i + 1].y + yOffset, width);				
			}
		}
	}
}

void water::computation(int ** line_array, int ** dot_array, int num_of_line, int num_of_dots, int dot_idx, int change_flag)
{
	int start_x = inter_path[0].x, start_y = inter_path[0].y;
	int inter_idx=1;
	int n_idx=0, n_passed=0;
	int maxidx;

	for (int i = 0; i < num_of_path;i++) {
		n_idx = find_nearest(line_array, num_of_line, start_x, start_y);
		if (n_idx == n_passed) break;
		if (n_idx == -1) {
			inter_path[inter_idx].x = start_x;
			inter_path[inter_idx].y = ofGetHeight();
			break;
		}

		n_passed = n_idx;

		inter_path[inter_idx].x = start_x;
		inter_path[inter_idx].y = start_y + distance(line_array[n_idx], start_x, start_y, change_flag);
		inter_idx += 1;
		
		maxidx = IDX(line_array[n_idx][1], line_array[n_idx][3]);
		
		if (change_flag)
			maxidx = CHG_IDX(line_array[n_idx][1], line_array[n_idx][3]);

		start_x = line_array[n_idx][maxidx];
		start_y = MAX(line_array[n_idx][1], line_array[n_idx][3]);
		
		inter_path[inter_idx].x = start_x;
		inter_path[inter_idx].y = start_y;
		inter_idx += 1;
	}

	calc_path_flag = 1;
}

int water::distance(int * line_array, int start_x, int start_y, int change_flag)
{
	int temp_x = line_array[0], temp_y = line_array[1];
	float slope = SLOPE(line_array[0], line_array[1], line_array[2], line_array[3]);

	if (change_flag) {
		temp_y = line_array[3];
		slope = SLOPE(line_array[0], line_array[3], line_array[2], line_array[1]);
	}
	
	float slope_c = temp_y - (slope * temp_x);
	float start_c = start_y - (slope * start_x);
	return (int)(slope_c - start_c);
}

int water::find_nearest(int ** line_array, int num_of_line, int start_x, int start_y)
{
	int nearest_y = 100000;
	int n_idx = -1;
	for (int i = 0; i < num_of_line; i++) { // find closest line
		if (line_array[i][1] < start_y || line_array[i][3] < start_y) continue;
		if (MIN(line_array[i][0], line_array[i][2]) > start_x || MAX(line_array[i][0], line_array[i][2]) < start_x) continue;

		int min_y = MIN(line_array[i][1], line_array[i][3]);

		if (nearest_y > min_y) {
			nearest_y = min_y;
			n_idx = i; // n_idx : index of closest line
		}
	}
	return n_idx;
}

void water::reset()
{
	local_r = ofRandom(0, 100);
	local_g = ofRandom(0, 100);
	local_b = ofRandom(185, 250);
}
