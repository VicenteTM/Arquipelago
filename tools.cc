//tools.cc
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#include "graphic.h"
#include "tools.h"


double Segment::x()
{
	return p2.x - p1.x;
}


double Segment::y()
{
	return p2.y - p1.y;
}


double Segment::longueur ()
{
	return sqrt(pow(x(),2)+pow(y(),2));
}


void Segment::draw_segment(Crgb color)
{
	graphic_draw_seg(p1.x, p1.y, p2.x, p2.y, color);
}


bool Segment::in_intervalle(Point p_test)
{
	Segment lien_1{p1, p_test};
	Segment lien_2{p2, p_test};
	
	if (longueur() < lien_1.longueur() or 
		longueur() < lien_2.longueur())
	{
		return false;
	}
	
	return true;
}


void Cercle::draw_cercle(Crgb color)
{
	graphic_draw_cercle(rayon,centre.x,centre.y,color);
}


void Cercle::draw_rectangle(Crgb color)
{
	graphic_draw_rectangle(rayon,centre.x,centre.y,color);
}


void Cercle::draw_etoile(Crgb color)
{
	graphic_draw_etoile(rayon,centre.x,centre.y,color);
}

bool Cercle::in_cercle(Point p)
{
	Segment lien_tmp{centre,p};
	
	return (lien_tmp.longueur() < rayon);
}

bool nodeSuperposition(Cercle node_1, Cercle node_2, bool prm_dist)
{
	Segment lien_actif{node_1.centre,node_2.centre};
	if (prm_dist)
	{
		return (lien_actif.longueur() <= (node_1.rayon + node_2.rayon + dist_min));
	}
	return (lien_actif.longueur() <= (node_1.rayon + node_2.rayon));
}


bool nodeLinkSuperposition(Cercle node_1, Cercle node_2, Cercle node_test,
																		bool prm_dist)
{
	double x1, x2, y1, y2;
	
	Segment lien_actif{node_1.centre, node_2.centre};
	Segment lien_tmp{node_1.centre, node_test.centre};
	
	x1 = lien_actif.x();
	x2 = lien_tmp.x();
	y1 = lien_actif.y();
	y2 = lien_tmp.y();
	
	double prodscalaire, distNN, coeff, distLN;

	prodscalaire = x1 * x2 + y1 * y2;
	
	distNN = lien_actif.longueur();
	
	coeff = prodscalaire/pow(distNN,2);
	
	double point_x = coeff * x1 + node_1.centre.x;
	double point_y = coeff * y1 + node_1.centre.y;
	
	Point p = {point_x,point_y};
		
	if (not lien_actif.in_intervalle(p))
	{
		return false;
	}
	
	distLN = sqrt(pow((p.x - node_test.centre.x),2) +
				  pow((p.y - node_test.centre.y),2));
	
	if (prm_dist)
	{
		return (distLN <= (node_test.rayon + dist_min));
	}
	
	return (distLN <= node_test.rayon);
}
