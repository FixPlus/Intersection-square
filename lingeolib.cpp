#include "lingeolib.h"
#include <cmath>
#include <iostream>

using namespace lingeo;





//************BEGIN METHODS OF line_t STRUCTURE*********************************



line_t::line_t(){ // x = y default line
	a = -1.0;
	b = 1.0;
	c = 0.0;

}

line_t::line_t(float a, float b, float c){ // ax + by + c = 0
	this->a = a;
	this->b = b;
	this->c = c;
}

line_t::line_t(struct point_t a1, struct point_t a2){ // line passing the points a1 and a2
	float angle = std::atan((a2.y - a1.y)/(a2.x - a1.x));
	float sin_angle = std::sin(angle);
	float cos_angle = std::sqrt(1.0 - sin_angle * sin_angle);
	point_t normal_vect(-sin_angle, cos_angle);
	a = normal_vect.x;
	b = normal_vect.y;
	c = -(a1.x * normal_vect.x + a1.y * normal_vect.y);
}

bool line_t::intersect(line_t &another){
	return std::abs(a * another.b - another.a * b) < flt_tolerance ? false : true; 	
}

point_t line_t::point_of_intersect(line_t &another){
	float det = (a * another.b - another.a * b);
	float det1 = ((-c) * another.b - (-another.c) * b);
	float det2 = (a * (-another.c) - another.a * (-c));
	point_t ret(det1/det, det2/det);
	return ret;
}

void line_t::print(){
	const char* str_1 = (b > 0.0) ? "+ " : "- ";
	const char* str_2 = (c > 0.0) ? "+ " : "- ";

	std::cout << a << "x " << str_1 << std::abs(b) << "y " << str_2 << std::abs(c) << " = 0";
}

line_t& line_t::operator=(const line_t& line){
	a = line.a;
	b = line.b;
	c = line.c;
	return *this;
}


//************END METHODS OF line_t STRUCTURE*********************************

//*********BEGIN METHODS OF polygon_t STRUCTURE*******************************



polygon_t::polygon_t(std::vector<point_t> vertices){
	this->vertices = vertices;
}

polygon_t::polygon_t(point_t* vertices, int n_verts){
	if(!vertices)
		return;
	for(int i = 0; i < n_verts; i++)
		this->vertices.insert(this->vertices.end(), vertices[i]);
}

polygon_t::polygon_t(){

}

float  polygon_t::square(){
	if(vertices.size() < 3)
		return 0;

	float sum1 = 0;
	float sum2 = 0;
	for(int i = 0; i < vertices.size(); i++){
		sum1 += vertices[i].x * vertices[(i + 1)%vertices.size()].y;
		sum2 -= vertices[(i + 1)%vertices.size()].x * vertices[i].y;
	}
	float result = (sum1 + sum2) / 2.0;
	if(result < 0.0)
		result = -result;
	return result;
}

line_t polygon_t::get_side(int index){
	while(index < 0)
		index += vertices.size();

	line_t ret(vertices[index % vertices.size()], vertices[(index + 1) % vertices.size()]);
	return ret;
}


polygon_t polygon_t::cut_poly_by_line( line_t& line, point_t & half_space_pt){
	float side_offset = line.a * half_space_pt.x + line.b * half_space_pt.y + line.c;


#ifdef LINGEO_DEV

	std::cout << "	>Cutting by line:"; line.print(); std::cout << std::endl << "		Offset is " << side_offset << std::endl;

#endif

	if(std::abs(side_offset) < flt_tolerance){
		return *this;
	}

	bool side = (side_offset > 0.0);
	std::vector<point_t> new_verts = std::vector<point_t>();

	for(int i = 0; i < vertices.size(); i++){
		if(side == (line.a * vertices[i].x + line.b * vertices[i].y + line.c > 0.0)){

			int next_index = (i + vertices.size() + 1) % vertices.size();
			int prev_index = (i + vertices.size() - 1) % vertices.size();

			if(side != (line.a * vertices[prev_index].x + line.b * vertices[prev_index].y + line.c > 0.0)){

				line_t poly_side(vertices[i], vertices[prev_index]);
				point_t intersect = line.point_of_intersect(poly_side);

				bool flag  = false;
				for(int i = 0; i < new_verts.size(); i++)
					if(intersect == new_verts[i]){
						flag = true;
						break;
					}
					
				if(!flag && intersect != vertices[i])
					new_verts.insert(new_verts.end(), intersect);


			#ifdef LINGEO_DEV

				std::cout << "		Cutted line: "; poly_side.print(); std::cout << "	(i = " << i<<")" << std::endl;

			#endif

			}

			new_verts.insert(new_verts.end(), vertices[i]);

			if(side != (line.a * vertices[next_index].x + line.b * vertices[next_index].y + line.c > 0.0)){

				line_t poly_side(vertices[i], vertices[next_index]);
				point_t intersect = line.point_of_intersect(poly_side);


				bool flag  = false;
				for(int i = 0; i < new_verts.size(); i++)
					if(intersect == new_verts[i]){
						flag = true;
						break;
					}
					
				if(!flag)
					new_verts.insert(new_verts.end(), intersect);

			#ifdef LINGEO_DEV

				std::cout << "		Cutted line: "; poly_side.print(); std::cout << "	(i = " << i<<")" << std::endl;

			#endif

			}
		}
	}
#ifdef LINGEO_DEV

	std::cout << "	>CUTTED SUCCESFUL\n\n";

#endif

	polygon_t ret(new_verts);
	return ret;
}



polygon_t polygon_t::get_poly_intersection(polygon_t another){
	
#ifdef LINGEO_DEV

	std::cout << "\n\n###CALCULATING INTERSECTION BETWEEN:\n\n";
	print();
	another.print();
	std::cout << std::endl;

#endif



	if(!intersect(another)){

	#ifdef LINGEO_DEV

		std::cout << "Result: they don't intesect each other!\n#################<<<<\n\n\n";

	#endif

		polygon_t ret;
		return ret;
	}



	for(int i = 0; i < vertices.size(); i++){
		line_t side = get_side(i);
		another = another.cut_poly_by_line(side, vertices[(i + 2) % vertices.size()]);


	#ifdef LINGEO_DEV

		std::cout << "	Cutted poly:\n	";
		another.print();
		std::cout << std::endl;

	#endif

	}




#ifdef LINGEO_DEV

	std::cout << "Intersection got:\n";
	another.print();
	std::cout << "#################<<<<\n\n\n";

#endif



	return another;
}

bool polygon_t::intersect(polygon_t &another) {
	polygon_t poly1 = *this;
	polygon_t poly2 = another;
	for(int k = 0; k < 2; k++){
		for(int i = 0; i < poly1.vertices.size(); i++){
			line_t div_line = poly1.get_side(i);
			bool half_space = (div_line.a * poly1.vertices[(i + 2) % poly1.vertices.size()].x + div_line.b * poly1.vertices[(i + 2) % poly1.vertices.size()].y + div_line.c > 0.0);
			bool flag = false;
			for(int j = 0; j < poly2.vertices.size(); j++){
				point_t vertex = poly2.vertices[j];
				if(half_space == (div_line.a * vertex.x + div_line.b * vertex.y + div_line.c > 0.0)){
					flag = true;
					break;
				}
			}
			if(!flag)
				return false;
		}
		polygon_t temp = poly1;
		poly1 = poly2;
		poly2 = temp;
	}

	return true;	
}



void polygon_t::print(){
	std::cout << "Polygon is " << vertices.size() << "angle: ";
	for(int i = 0; i < vertices.size(); i++)
		std::cout << "(" << vertices[i].x << "; " << vertices[i].y <<  ") ";
	std::cout << std::endl;
}

polygon_t& polygon_t::operator=(const polygon_t &poly){
	vertices = poly.vertices;
	return *this;
}



//*********END METHODS OF polygon_t STRUCTURE*******************************

//*********BEGIN METHODS OF point_t STRUCTURE*******************************




point_t::point_t(float x, float y) {
	this->x = x;
	this->y = y;
}

point_t::point_t() {
	this->x = 0.0;
	this->y = 0.0;
}

point_t::point_t(const point_t &point) {
	this->x = point.x;
	this->y = point.y;
}

void point_t::print(){
	std::cout << "(" << x << " ; " << y << ")";
}

bool point_t::operator==(point_t &pnt){
	return (std::abs(x - pnt.x) < flt_tolerance && std::abs(y - pnt.y) < flt_tolerance) ? true : false;
}

bool point_t::operator!=(point_t &pnt){
	return !operator==(pnt);
}


point_t& point_t::operator=(const point_t &pnt){
	x = pnt.x;
	y = pnt.y;
	return *this;
}

