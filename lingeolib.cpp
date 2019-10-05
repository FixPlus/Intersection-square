#include "lingeolib.h"
#include <cmath>
#include <iostream>

using namespace lingeo;

const float lingeo::flt_tolerance = 0.00001;
float lingeo::inter_area_width = 100.0;


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

bool line_t::valid() const {
	return (a != a || b != b || c != c) ? false : true; 
}

bool line_t::intersect(line_t const & another) const {
	return std::abs(a * another.b - another.a * b) < flt_tolerance ? false : true; 	
}

point_t line_t::point_of_intersect(line_t const & another) const {
	float det = (a * another.b - another.a * b);
	float det1 = ((-c) * another.b - (-another.c) * b);
	float det2 = (a * (-another.c) - another.a * (-c));
	point_t ret(det1/det, det2/det);
	return ret;
}

void line_t::print() const {
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

enum areas_t line_t::get_side_area(point_t const &point) const {
	
	float side_offset = a * point.x + b * point.y + c;

	if(side_offset > 0.0 + flt_tolerance * inter_area_width)
		return LEFT_SIDE;
	
	if(side_offset > 0.0 - flt_tolerance * inter_area_width)
		return INTER_SIDE;
	
	return RIGHT_SIDE;
}

bool line_t::separates(point_t const &pnt1, point_t const &pnt2) const {
	enum areas_t side1 = get_side_area(pnt1);
	enum areas_t side2 = get_side_area(pnt2);
	if(side1 == INTER_SIDE || side2 == INTER_SIDE)
		return false;
	return (side1 == side2) ? false : true;
}


//************END METHODS OF line_t STRUCTURE*********************************

//*********BEGIN METHODS OF polygon_t STRUCTURE*******************************



polygon_t::polygon_t(std::vector<point_t> vertices){
	this -> vertices = vertices;
}

polygon_t::polygon_t(){

}

float  polygon_t::square() const {
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

line_t polygon_t::get_side(int index) const {
	while(index < 0)
		index += vertices.size();

	line_t ret(vertices[index % vertices.size()], vertices[(index + 1) % vertices.size()]);
	
	return ret;
}

bool polygon_t::valid() const {
	if(vertices.size() == 0)
		return false;
	
	for(int i = 0; i < vertices.size(); i++)
		if(!vertices[i].valid())
			return false;

	return true;
}

point_t polygon_t::side_line_intersect(int index, line_t const & line) const {

	point_t ret(NAN, NAN);

	if(!line.separates(vertices[index % vertices.size()], vertices[(index + 1) % vertices.size()]))
		return ret;

	line_t poly_side = get_side(index);

	if(poly_side.intersect(line))
		ret = line.point_of_intersect(poly_side);
	
	return ret;
}


polygon_t polygon_t::cut_poly_by_line(line_t const & line, point_t const & half_space_pt) const{
	
	enum areas_t side = line.get_side_area(half_space_pt);



	if(side == INTER_SIDE){            //it must be LEFT_SIDE or RIGHT_SIDE

	#ifdef LINGEO_DEV

		std::cout << "	>Line or half-space is not valid: "; line.print(); std::cout  <<"	>CUTTING FAILED" << std::endl;

	#endif

		return *this;
	}

#ifdef LINGEO_DEV

	std::cout << "	>Cutting by line:"; line.print(); std::cout << std::endl;

#endif

	polygon_t ret;

	for(int i = 0; i < vertices.size(); i++){

		enum areas_t vert_side = line.get_side_area(vertices[i]); 

		if((side == vert_side || vert_side == INTER_SIDE) && !ret.holding(vertices[i])) // half_space_pt side points and inter side points are both acceptable, also checking if it is already in ret
			ret.vertices.insert(ret.vertices.end(), vertices[i]);

		point_t intersect = side_line_intersect(i, line);

		if(intersect.valid() && !ret.holding(intersect)){
			ret.vertices.insert(ret.vertices.end(), intersect);


		#ifdef LINGEO_DEV

			std::cout << "		Cutted side: "; vertices[i].print(); std::cout << "---"; vertices[(i + 1) % vertices.size()].print(); std::cout << "	(i = " << i<<")";
			std::cout << "  in point "; intersect.print(); std::cout << std::endl;

		#endif

		}

	}

#ifdef LINGEO_DEV

	std::cout << "	>CUTTED SUCCESFUL\n\n";



	std::cout << "	Cutted poly:\n	";
	ret.print();
	std::cout << std::endl;


#endif

	return ret;
}



polygon_t polygon_t::get_poly_intersection(polygon_t another) const{
	
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

	}




#ifdef LINGEO_DEV

	std::cout << "Intersection got:\n";
	another.print();
	std::cout << "#################<<<<\n\n\n";

#endif



	return another;
}

bool polygon_t::side_divided_from(polygon_t const & another) const{
	for(int i = 0; i < vertices.size(); i++){
		line_t div_line = get_side(i);
		
		enum areas_t half_space = div_line.get_side_area(vertices[(i + 2) % vertices.size()]);
		
		if(half_space == INTER_SIDE)
			continue;

		bool flag = true;
		for(int j = 0; j < another.vertices.size(); j++){
			point_t vertex = another.vertices[j];
			if(half_space == div_line.get_side_area(vertex)){
				flag = false;
				break;
			}
		}
		if(flag)
			return true;	
	}
	return false;
}

bool polygon_t::holding(point_t const & vert) const{
	for(int i = 0; i < vertices.size(); i++)
		if(vert == vertices[i])
			return true;
	return false;
}


bool polygon_t::intersect(polygon_t const & another) const {
	return !(side_divided_from(another) || another.side_divided_from(*this));	
}

void polygon_t::print() const {
	std::cout << "Polygon is " << vertices.size() << "angle: ";
	for(int i = 0; i < vertices.size(); i++){
		if(i != 0)
			std::cout << "---";
		vertices[i].print();
	}
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
	this->x = NAN;
	this->y = NAN;
}

point_t::point_t(const point_t &point) {
	this->x = point.x;
	this->y = point.y;
}

void point_t::print() const {
	std::cout << "(" << x << " ; " << y << ")";
}

bool point_t::valid() const {
	return (x != x || y != y) ? false : true;
}


bool point_t::operator==(point_t const &pnt) const {
	return (std::abs(x - pnt.x) < flt_tolerance && std::abs(y - pnt.y) < flt_tolerance) ? true : false;
}

bool point_t::operator!=(point_t const &pnt) const {
	return !operator==(pnt);
}


point_t& point_t::operator=(const point_t &pnt){
	x = pnt.x;
	y = pnt.y;
	return *this;
}

