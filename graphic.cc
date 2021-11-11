//graphic.cc
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#include "graphic_gui.h"

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr); 

static Wp prm;


double Wp::m_asp()
{
	return ((double)(x_max-x_min)/(y_max-y_min));
}


double Wp::asp()
{
	return ((double)width/height);
}


void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr)
{
	ptcr = &cr;
}


void graphic_set_prm(double m_x_max, double m_x_min, double m_y_max, double m_y_min, 
					 unsigned int win_height, unsigned int win_width)
{
	prm.x_max = m_x_max;
	prm.x_min = m_x_min;
	prm.y_max = m_y_max;
	prm.y_min = m_y_min;
	
	prm.height = win_height;
	prm.width = win_width;
	
	if (prm.asp() != prm.m_asp())
	{
		if (prm.height > prm.width)
		{
			prm.height = prm.width/prm.m_asp();
		}
		else
		{
			prm.width = prm.height*prm.m_asp();
		}
	}
}


void graphic_set_color(Crgb color)
{
	switch (color)
	{	
		case WHITE	:(*ptcr)->set_source_rgb(1.0, 1.0, 1.0); break;
		case BLACK	:(*ptcr)->set_source_rgb(0.0, 0.0, 0.0); break;
		case RED	:(*ptcr)->set_source_rgb(1.0, 0.0, 0.0); break;
		case GREEN	:(*ptcr)->set_source_rgb(0.0, 1.0, 0.0); break;
	}
}


void graphic_draw_cercle(double radius, double x, double y, Crgb color)
{
	radius = convertDist(radius);
	
	x = convertX(x);
	y = convertY(y);
	
	(*ptcr)->set_line_width(3.);
	(*ptcr)->arc(x, y, radius , 0.0, 2.0 * M_PI);
	graphic_set_color(WHITE);
	(*ptcr)->fill_preserve();
	graphic_set_color(color);

	(*ptcr)->stroke();
}


void graphic_draw_rectangle(double radius, double x, double y, Crgb color)
{
	radius = convertDist(radius);
	
	x = convertX(x);
	y = convertY(y);
	
	(*ptcr)->set_line_width(3.);
	graphic_set_color(color);
	(*ptcr)->move_to(x + (radius*0.75), y - (radius/8));
	(*ptcr)->line_to(x + (radius*0.75), y + (radius/8));
	(*ptcr)->line_to(x - (radius*0.75), y + (radius/8));
	(*ptcr)->line_to(x - (radius*0.75), y - (radius/8));
	(*ptcr)->line_to(x + (radius*0.75), y - (radius/8));
	(*ptcr)->line_to(x + (radius*0.75), y + (radius/8));
	
	(*ptcr)->stroke();
}


void graphic_draw_etoile(double radius, double x, double y, Crgb color)
{
	radius = convertDist(radius);

	x = convertX(x);
	y = convertY(y);

	(*ptcr)->set_line_width(3.);
	graphic_set_color(color);
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x, y + radius);
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x + radius*(sqrt(2)/2), y + radius*(sqrt(2)/2));
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x + radius, y);
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x + radius*(sqrt(2)/2), y - radius*(sqrt(2)/2));
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x, y - radius);
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x - radius*(sqrt(2)/2), y - radius*(sqrt(2)/2));
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x - radius, y);
	(*ptcr)->move_to(x, y);
	(*ptcr)->line_to(x - radius*(sqrt(2)/2), y + radius*(sqrt(2)/2));

	(*ptcr)->stroke();
}


void graphic_draw_seg(double x1, double y1, double x2, double y2, Crgb color)
{
	(*ptcr)->set_line_width(2.0);
	graphic_set_color(color);
	(*ptcr)->move_to(convertX(x1), convertY(y1));
	(*ptcr)->line_to(convertX(x2), convertY(y2));

	(*ptcr)->stroke();
}


int convertX(double coord)
{
	return ((coord - prm.x_min)/(prm.x_max - prm.x_min))*prm.width;	
}


int convertY(double coord)
{
	return ((prm.y_max - coord)/(prm.y_max - prm.y_min))*prm.height;
}


int convertDist(double dist)
{
	return (dist/(prm.y_max - prm.y_min))*prm.height;
}


int convert_backX(double coord)
{
	return (((prm.x_max - prm.x_min)*coord)/prm.width) + prm.x_min;
}


int convert_backY(double coord)
{
	return prm.y_max - ((coord*(prm.y_max - prm.y_min))/prm.height);
}


int convert_backDist(double dist)
{
	return ((dist/prm.height)*(prm.y_max - prm.y_min));
}

