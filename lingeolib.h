#pragma once
#include <vector>

namespace lingeo{
	const float flt_tolerance = 0.00001;   //tolerance that will be used in calculation of coordinates (e.g. for comparing the points)
	struct point_t {      // (x , y)
		float x, y;
		point_t(float x, float y);
		point_t(const point_t &point);
		point_t();
		
		void print();		//prints on screen: "(x ; y)"

		bool operator==(point_t &pnt);
		bool operator!=(point_t &pnt);
		point_t& operator=(const point_t &pnt);
	};

	struct line_t{        // ax + by + c = 0
		float a,b,c;
		
		line_t();                                     // default line x = y
		line_t(float a, float b, float c);            // constructs the line using exact parametrs
		line_t(struct point_t a1, struct point_t a2); // constructs the line passing through points a1 and a2
		
		bool intersect(line_t &another);              //check if intersects the line "another"
		point_t point_of_intersect(line_t &another);  //return the point of intersection with line "another"
		void print();								  //prints on screen: "ax + by + c = 0"

		line_t& operator=(const line_t &line);
	};

	struct polygon_t{ // (x1, y1) (x2, y2) ... (xn, yn)
		std::vector<point_t> vertices;
		
		polygon_t();                                  // empty polygon
		polygon_t(std::vector<point_t> vertices);     // constructing by vector<point_t> array
		polygon_t(point_t* vertices, int n_verts);    // constructing by point_t array

		float square();                               // returns the square of polygon
		void print();                                 // prints: "Polygon is nangle: (x1, y1) (x2, y2) ... (xn, yn)"
		
		line_t get_side(int index);                   // return the line holding the polygon side (n side is a line constructed with nth and (n + 1)th vertex of polygon)

		bool intersect(polygon_t &another);           // checks if polygon intersects with "another" polygon
		polygon_t cut_poly_by_line(line_t &line, point_t & half_space_pt); //returns the cutted polygon (which half of polygon will be cutted defines by half_space_pt)
		polygon_t get_poly_intersection(polygon_t another); // returns the polygon that is the intersection of this and another

		polygon_t& operator=(const polygon_t &poly);
	};

};