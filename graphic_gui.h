//graphic_gui.h
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#ifndef GTKMM_GRAPHIC_GUI_H
#define GTKMM_GRAPHIC_GUI_H

#include <cairomm/context.h>
#include <gtkmm.h>
#include "graphic.h"

struct Wp
{
	double x_min;
	double x_max;
	double y_min;
	double y_max;
	
	int height;
	int width;
	
	double m_asp();
	double asp();
};


void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);

void graphic_set_prm(double w_x_max, double w_x_min, double w_y_max, double w_y_min, 
					 unsigned int win_height, unsigned int win_width);


#endif // GTKMM_GRAPHIC_GUI_H
