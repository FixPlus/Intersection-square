#pragma once
#include <vector>
#include <array>

namespace lingeo{

	extern const float flt_tolerance;                 // tolerance that will be used in calculation of coordinates (e.g. for comparing the points)
	extern float inter_area_width;                    // defines the width (counted in flt_tolerances) of INTER_AREA that used to avoid some corner cases
	enum areas_t {LEFT_SIDE, INTER_SIDE, RIGHT_SIDE}; // there are 3 areas that are defined by the line cut of plane: left, right and inter area. The last one represents the same line with defined thickness (inter_are_width)
	
	struct point_t {      // (x , y)
		float x, y;
		point_t(float x, float y);
		point_t(const point_t &point);
		point_t();
		
		void print() const;		// prints on screen: "(x ; y)"

		bool valid() const;
		bool operator==(point_t const &pnt) const;
		bool operator!=(point_t const &pnt) const;
		point_t& operator=(const point_t &pnt);
	};

	struct line_t{        // ax + by + c = 0
		float a,b,c;
		
		line_t();                                     // default line x = y
		line_t(float a, float b, float c);            // constructs the line using exact parametrs
		line_t(struct point_t a1, struct point_t a2); // constructs the line passing through points a1 and a2
		
		enum areas_t get_side_area(point_t const &point) const;         // tells in what area the point is lying
		bool separates(point_t const &pnt1, point_t const &pnt2) const; // checks if the line separate two points
		bool valid() const;
		bool intersect(line_t const & another) const;              // check if intersects the line "another"
		point_t point_of_intersect(line_t const & another) const;  // return the point of intersection with line "another"
		void print() const;								           // prints on screen: "ax + by + c = 0"

		line_t& operator=(const line_t &line);
	};

	struct polygon_t{ // (x1, y1) (x2, y2) ... (xn, yn)
		std::vector<point_t> vertices;
		
		polygon_t();                                  // empty polygon
		polygon_t(std::vector<point_t> vertices);     // constructing by vector<point_t> array

		float square() const;                               // returns the square of polygon
		void print() const;                                 // prints: "Polygon is nangle: (x1, y1) (x2, y2) ... (xn, yn)"
		
		line_t get_side(int index) const;                   // return the line holding the polygon side (n side is a line constructed with nth and (n + 1)th vertex of polygon)

		bool valid() const;
		bool side_divided_from(polygon_t const & another) const;							  // tells if this polygon can be separate from another by any line holding a side of this polygon 
		bool intersect(polygon_t const & another) const;                                      // checks if polygon intersects with "another" polygon
		bool holding(point_t const & vert) const;											  // tells if this polygon is holding the vert as one of it's vertices
		point_t side_line_intersect(int index, line_t const & line) const;                    // return the point of polygon side intersection with line if it exists, returns non-valid point otherwise
		polygon_t cut_poly_by_line(line_t const & line, point_t const & half_space_pt) const; // returns the cutted polygon (which half of polygon will be cutted defines by half_space_pt)
		polygon_t get_poly_intersection(polygon_t another) const;                             // returns the polygon that is the intersection of this and another

		polygon_t& operator=(const polygon_t &poly);
	};

};