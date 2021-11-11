//tools.h
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1


#ifndef TOOLS_HEADER_H
#define TOOLS_HEADER_H

#include <math.h>
#include "graphic_color.h"
#include "constantes.h"

using namespace std;

struct Point{
	double x, y;
};


struct Segment{
	Point p1;
	Point p2;
	
	double x();
	double y();
	double longueur();
	
	void draw_segment(Crgb color = BLACK);
	
	bool in_intervalle (Point p_test);
};


struct Cercle{
	double rayon;
	Point centre;
	
	void draw_cercle(Crgb color);
	void draw_rectangle(Crgb color);
	void draw_etoile(Crgb color);
	
	bool in_cercle(Point point);
	
};


bool nodeSuperposition(Cercle node_1, Cercle node_2, bool prm_dist = false);
bool nodeLinkSuperposition(Cercle node_1, Cercle node_2, Cercle node_test,
														 bool prm_dist = false);

#endif // TOOLS_HEADER_H
