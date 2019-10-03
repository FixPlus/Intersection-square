#include "lingeolib.h"
#include <iostream>

using namespace std; 
using namespace lingeo;


int main(){
	point_t tri1[3];
	point_t tri2[3];

	for(int i = 0; i < 3; i++){
		cin >> tri1[i].x;
		cin >> tri1[i].y;
	}
	for(int i = 0; i < 3; i++){
		cin >> tri2[i].x;
		cin >> tri2[i].y;
	}

	polygon_t triangle1(tri1, 3);
	polygon_t triangle2(tri2, 3);
	triangle1 = triangle1.get_poly_intersection(triangle2);
	cout << triangle1.square() << endl;

}