#include "lingeolib.h"
#include <iostream>
#include <array>

using namespace std; 
using namespace lingeo;


polygon_t input_triangle(){
	std::vector<point_t> verts;
	point_t temp;
	for(int i = 0; i < 3; i++){
		cin >> temp.x;
		cin >> temp.y;
		verts.insert(verts.end(), temp);
	}
	polygon_t ret(verts);
	return ret;
}

int main(){

	polygon_t triangle1 = input_triangle();
	polygon_t triangle2 = input_triangle();

	polygon_t inter = triangle1.get_poly_intersection(triangle2);
	
	cout << inter.square() << endl;

}