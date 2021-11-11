//graphic.h
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#ifndef GTKMM_GRAPHIC_H
#define GTKMM_GRAPHIC_H

#include "graphic_color.h"

void graphic_set_color(Crgb color);

void graphic_draw_cercle(double radius, double x, double y, Crgb color);
void graphic_draw_rectangle(double radius, double x, double y, Crgb color);
void graphic_draw_etoile(double radius, double x, double y, Crgb color);
void graphic_draw_seg(double x1, double y1, double x2, double y2, Crgb color);



int convertX(double coord);
int convertY(double coord);
int convertDist(double dist);

int convert_backX(double coord);
int convert_backY(double coord);
int convert_backDist(double dist);


#endif // GTKMM_GRAPHIC_H
