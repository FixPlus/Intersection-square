#include "lingeolib.h"
#include <stdlib.h>
#include <iostream>
#include <cmath>
#include <stdio.h>

using namespace lingeo; 

int rand_bounds = 32000;
polygon_t generate_triangle(){
	std::vector<point_t> verts;
	point_t temp;
	for(int i = 0; i < 3; i++){
		temp.x = (float)(rand() % rand_bounds - rand_bounds/2)/1000.0;
		temp.y = (float)(rand() % 32000 - rand_bounds/2)/1000.0;
		verts.insert(verts.end(), temp);
	}
	polygon_t ret(verts);
	return ret;
}


int main(){
	srand(time(NULL));
	
	int failures_count(0);
	
	unsigned int max_devi = 0;
	unsigned int index_md = 0;
	
	float avg_dev = 0.0;
	
	int ntimes = 100000;
	inter_area_width = 0.0; // declared in lingeolib.h, defined in lingeolib.cpp
	
	polygon_t corner_case1;
	polygon_t corner_case2;

	for(int j = 0; j < 11; j++){ //testing inter_area_width in bounds[0 .. 10]
		avg_dev = 0.0;
		max_devi = 0;
		index_md = 0;
		failures_count = 0;
		for(int i = 0; i < ntimes; i++){

			polygon_t triangle = generate_triangle();
			float square = triangle.square();
			
			polygon_t new_triangle = triangle.get_poly_intersection(triangle);
			float after_proc_square = new_triangle.square();
			
			unsigned int int_data1 = *((unsigned int*)(&square));
			unsigned int int_data2 = *((unsigned int*)(&after_proc_square));
			

			unsigned int mask = 0x1FFu << 23u;
			
			int_data1 &=  ~mask; 
			int_data2 &=  ~mask;
			
			unsigned int deviation = (int_data1 > int_data2) ? int_data1 - int_data2 : int_data2 - int_data1;
			unsigned int log2_dev = std::log(deviation)/ std::log(2) + 1;
			
			avg_dev += log2_dev;  
			
			if(max_devi < log2_dev){
				max_devi = log2_dev;
				index_md = i;
				corner_case1 = triangle;
				corner_case2 = new_triangle;
			}

			if(deviation > 0){
				//std::cout << "(Iteration " << i << ")FAILED:" << std::endl << "	>input tri: "; triangle.print(); std::cout << " sq = " << square << std::endl << "	>operated tri: "; new_triangle.print(); std::cout << " sq = " << after_proc_square << std::endl;
				failures_count++;
				//std::cout << "	>Digital deviation: " << deviation << " ("<< log2_dev << ") " << std::endl;
			}
		}
		avg_dev /= ntimes; 
			std::cout << "K = " << j << std::endl << "Done! Count of failures: " << failures_count << " (" << (float)failures_count * 100.0/ (float)ntimes <<"%)"<< std::endl << "Max dev is: " << max_devi << " ( on iteration " << index_md << ")" << std::endl << "Average deviation(in float mantissa bits) is: " << avg_dev << std::endl;
			std::cout << "	Corner case:\n		tri:"; corner_case1.print(); std::cout << std::endl << "		proc:"; corner_case2.print(); std::cout << std::endl;
		inter_area_width += 1.0;
	}
	return 0;
}