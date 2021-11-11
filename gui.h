//gui.h
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#ifndef GUI_H
#define GUI_H

#include <glibmm/ustring.h>
#include <sstream>
#include <string>
#include "ville.h"
#include "graphic_gui.h"


class MyArea : public Gtk::DrawingArea
{
public:
	MyArea();
	virtual ~MyArea(){};
	
	void clear();
	void draw();
	void refresh();
	void zoom_in();
	void zoom_out();
	void zoom_reset();
	double zoom_critere();

protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;

private:
	bool empty;
	int count_zoom;
};


class MyEvent : public Gtk::Window
{
public:
	MyEvent();
	virtual ~MyEvent(){};

protected:
	void refresh_critere();
	void init_button();
	void on_button_clicked();
	
	bool on_button_press_event(GdkEventButton * event);
	bool event_add_node(Point p);
	
	bool on_button_release_event(GdkEventButton * event);
	
	bool on_key_press_event(GdkEventKey * key_event);
	
	void on_button_clicked_exit();
	void on_button_clicked_new();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_shortest_path();
	void on_button_clicked_zoom_in();
	void on_button_clicked_zoom_out();
	void on_button_clicked_zoom_reset();
	void on_button_clicked_edit_link();
	void on_button_clicked_housing(){};
	void on_button_clicked_transport(){};
	void on_button_clicked_production(){};

	MyArea              m_Area;

	Gtk::Box m_Box, m_Box_Graphic, m_Box_Buttons, m_Box_General, m_Box_Display, 
			 m_Box_Editor, m_Box_Informations;
			 
	Gtk::Button 		  m_Button_Exit;
	Gtk::Button 		  m_Button_New;
	Gtk::Button 		  m_Button_Open;
	Gtk::Button 		  m_Button_Save;
	Gtk::Button 		  m_Button_Zoom_In;
	Gtk::Button 		  m_Button_Zoom_Out;
	Gtk::Button 		  m_Button_Zoom_Reset;
	
	Gtk::ToggleButton 	  m_Button_Shortest_Path;
	Gtk::ToggleButton 	  m_Button_Edit_Link;

	Gtk::RadioButton	  m_Button_Housing;
	Gtk::RadioButton	  m_Button_Transport;
	Gtk::RadioButton	  m_Button_Production;

	Gtk::Frame 		  	  m_Frame_General;
	Gtk::Frame 		  	  m_Frame_Display;
	Gtk::Frame 		  	  m_Frame_Editor;
	Gtk::Frame 		  	  m_Frame_Informations;

	Gtk::Label 		  	  m_Label_Zoom;
	Gtk::Label 		  	  m_LabelENJ;
	Gtk::Label 		  	  m_LabelCI;
	Gtk::Label 		  	  m_LabelMTA;

private:
	Point click_press;
	Point click_release;
};


string notation_s(double nombre);


#endif // GUI_H
