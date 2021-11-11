//gui.cc
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#include "gui.h"

using namespace std;


MyArea::MyArea(): empty(false), count_zoom(0)
{
}


void MyArea::clear()
{
	empty = true; 
	refresh();
}


void MyArea::draw()
{
	empty = false;
}


void MyArea::refresh()
{
	auto win = get_window();
	
	if(win)
	{
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
							  get_allocation().get_height());
		win->invalidate_rect(r,false);
	}
}


void MyArea::zoom_in()
{
	if (default_zoom + delta_zoom * count_zoom >= max_zoom)
	{
		return;
	}
	
	++count_zoom;
}


void MyArea::zoom_out()
{
	if (default_zoom + delta_zoom * count_zoom <= min_zoom)
	{
		return;
	}
	
	--count_zoom;
}


void MyArea::zoom_reset()
{
	count_zoom = 0;
}


double MyArea::zoom_critere()
{
	return (default_zoom + delta_zoom * count_zoom);
}


bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	cr->set_source_rgb(1.0,1.0,1.0);
	cr->paint();
	
	Gtk::Allocation allocation = get_allocation();
	
	double espace_v = dim_max /(1 + delta_zoom * count_zoom);
	
	graphic_set_prm(espace_v, -espace_v, espace_v, -espace_v,
					allocation.get_height(),allocation.get_width());
	
	if (not empty)
	{
		graphic_set_context(cr);
		ville_draw_all();
	}
	return true;
}


MyEvent::MyEvent():
	m_Box(Gtk::ORIENTATION_HORIZONTAL,5),
	m_Box_Graphic(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_Buttons(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_General(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_Display(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_Editor(Gtk::ORIENTATION_VERTICAL, 5),
	m_Box_Informations(Gtk::ORIENTATION_VERTICAL, 5),
	m_Button_Exit("exit"),
	m_Button_New("new"),
	m_Button_Open("open"),
	m_Button_Save("save"),
	m_Button_Zoom_In("zoom in"),
	m_Button_Zoom_Out("zoom out"),
	m_Button_Zoom_Reset("zoom reset"),
	m_Button_Shortest_Path("shortest path"),
	m_Button_Edit_Link("edit link"),
	m_Button_Housing("housing"),
	m_Button_Transport("transport"),
	m_Button_Production("production"),
	m_Frame_General("General"),
	m_Frame_Display("Display"),
	m_Frame_Editor("Editor"),
	m_Frame_Informations("Informations"),
	m_Label_Zoom("zoom: x1.00", true),
	m_LabelENJ("ENJ: 0.0000", true),
	m_LabelCI("CI: 0", true),
	m_LabelMTA("MTA: 0", true)
{
	set_title("Archipelago");
	set_border_width(5);

	add(m_Box);
	
	this->init_button();

	m_Box_Buttons.pack_start(m_Frame_General, Gtk::PACK_SHRINK);
	m_Box_Buttons.pack_start(m_Frame_Display, Gtk::PACK_SHRINK);
	m_Box_Buttons.pack_start(m_Frame_Editor, Gtk::PACK_SHRINK);
	m_Box_Buttons.pack_start(m_Frame_Informations, Gtk::PACK_SHRINK);

	

	this->refresh_critere();

	this->on_button_clicked();

	show_all_children();
}


void MyEvent::refresh_critere()
{
	m_LabelENJ.set_text("ENJ: " + notation_s(villeENJ()));
	m_LabelCI.set_text("CI: " + notation_s(villeCI()));
	m_LabelMTA.set_text("MTA: " + notation_s(villeMTA()));
	
	m_Label_Zoom.set_text("zoom: x" + notation_s(m_Area.zoom_critere()));
}


void MyEvent::init_button()
{
	m_Box.pack_start(m_Box_Buttons, Gtk::PACK_SHRINK);
	m_Box.pack_start(m_Box_Graphic);

	m_Area.set_size_request(default_drawing_size, default_drawing_size);
	m_Box_Graphic.pack_start(m_Area);

	m_Box_General.pack_start(m_Button_Exit,false,false);
	m_Box_General.pack_start(m_Button_New,false,false);
	m_Box_General.pack_start(m_Button_Open,false,false);
	m_Box_General.pack_start(m_Button_Save,false,false);

	m_Frame_General.add(m_Box_General);

	m_Box_Display.pack_start(m_Button_Shortest_Path,false,false);
	m_Box_Display.pack_start(m_Button_Zoom_In,false,false);
	m_Box_Display.pack_start(m_Button_Zoom_Out,false,false);
	m_Box_Display.pack_start(m_Button_Zoom_Reset,false,false);
	m_Box_Display.pack_start(m_Label_Zoom,false,false);

	m_Frame_Display.add(m_Box_Display);

	m_Box_Editor.pack_start(m_Button_Edit_Link,false,false);

	m_Button_Transport.join_group(m_Button_Housing);
	m_Button_Production.join_group(m_Button_Housing);

	m_Box_Editor.pack_start(m_Button_Housing,false,false);
	m_Box_Editor.pack_start(m_Button_Transport,false,false);
	m_Box_Editor.pack_start(m_Button_Production,false,false);

	m_Frame_Editor.add(m_Box_Editor);
	
	m_Box_Informations.pack_start(m_LabelENJ,false,false);
	m_Box_Informations.pack_start(m_LabelCI,false,false);
	m_Box_Informations.pack_start(m_LabelMTA,false,false);

	m_Frame_Informations.add(m_Box_Informations);
}


void MyEvent::on_button_clicked()
{
	m_Button_Exit.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_exit));
			  
	m_Button_New.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_new));	
			  	  
	m_Button_Open.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_open));
			  
	m_Button_Save.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_save));
			  
	m_Button_Shortest_Path.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_shortest_path));
			  
	m_Button_Zoom_In.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_zoom_in));
			  
	m_Button_Zoom_Out.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_zoom_out));
			  
	m_Button_Zoom_Reset.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_zoom_reset));
			  
	m_Button_Edit_Link.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_edit_link));
			  
	m_Button_Housing.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_housing));
			  
	m_Button_Transport.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_transport));
			  
	m_Button_Production.signal_clicked().connect(sigc::mem_fun(*this,
			  &MyEvent::on_button_clicked_production));
}


bool MyEvent::on_button_press_event(GdkEventButton * event)
{
	if(event->type == GDK_BUTTON_PRESS)
	{
		double clic_x = event->x ;
		double clic_y = event->y ;
		double origin_x = m_Area.get_allocation().get_x();
		double origin_y = m_Area.get_allocation().get_y();
		double width = m_Area.get_allocation().get_width();
		double height= m_Area.get_allocation().get_height();
		
		if(clic_x >= origin_x && clic_x <= origin_x + width &&
		   clic_y >= origin_y && clic_y <= origin_y + height)
		{
			click_press = {(double)convert_backX(clic_x - origin_x),
						   (double)convert_backY(clic_y - origin_y)};
					 
			if (ville_p_in(click_press) and event->button != 3)
			{
				ville_modify_current_node(click_press,m_Button_Edit_Link.get_active(),
												m_Button_Shortest_Path.get_active());
			}
			else if (ville_v_alive())
			{
				if(event->button == 3)
				{
					ville_new_coordinates(click_press,
												m_Button_Shortest_Path.get_active());
				}
			}
			else if(event->button == 1)
			{
				event_add_node(click_press);
			}
		}
		refresh_critere();
		m_Area.refresh();
		m_Area.draw();
	}
	return true;
}


bool MyEvent::event_add_node(Point p)
{
	if (m_Button_Housing.get_active() == true)
	{
		if (ville_new_nodespp(p))
		{
			return true;
		}
	ville_add_node(p,'h');
	}
	
	if (m_Button_Transport.get_active() == true)
	{
		if (ville_new_nodespp(p))
		{
			return true;
		}
		ville_add_node(p,'t');
	}
	
	if (m_Button_Production.get_active() == true)
	{
		if (ville_new_nodespp(p))
		{
			return true;
		}
		ville_add_node(p,'p');
	}
	
	return false;
}


bool MyEvent::on_button_release_event(GdkEventButton * event)
{
	if(event->type == GDK_BUTTON_RELEASE)
	{
		double clic_x = event->x ;
		double clic_y = event->y ;
		double origin_x = m_Area.get_allocation().get_x();
		double origin_y = m_Area.get_allocation().get_y();
		double width = m_Area.get_allocation().get_width();
		double height= m_Area.get_allocation().get_height();
		
		if(clic_x >= origin_x && clic_x <= origin_x + width &&
		   clic_y >= origin_y && clic_y <= origin_y + height)
		{
			click_release = {(double)convert_backX(clic_x - origin_x), 
					   (double)convert_backY(clic_y - origin_y)};
			
			if (!ville_p_in(click_press) and ville_v_alive())
			{
				if(!(event->button == 3))
				{
					Segment tmp = {click_press, click_release};
					
					if (tmp.longueur() == 0 and !m_Button_Edit_Link.get_active())
					{
						ville_v_dead();
					}
					else if(tmp.longueur() != 0 and !m_Button_Edit_Link.get_active())
					{
						ville_new_capacity(tmp);
					}
				}
			}
		}
		refresh_critere();
		m_Area.refresh();
		m_Area.draw();
	}
	return true;
}


bool MyEvent::on_key_press_event(GdkEventKey * key_event)
{
	if(key_event->type == GDK_KEY_PRESS)
	{
		switch(gdk_keyval_to_unicode(key_event->keyval))
		{
			case 'o':
				m_Area.zoom_out();
				refresh_critere();
				m_Area.refresh();
				break;
			case 'i':
				m_Area.zoom_in();
				refresh_critere();
				m_Area.refresh();
				break;
			case 'r':
				m_Area.zoom_reset();
				refresh_critere();
				m_Area.refresh();
				break;
			case 'q':
				ville_delete();
				exit(0);
				break;
		}
	}
	
	return Gtk::Window::on_key_press_event(key_event);
}


void MyEvent::on_button_clicked_exit()
{
	ville_delete();
	exit(0);
}


void MyEvent::on_button_clicked_new()
{
	ville_delete();
	this->refresh_critere();
	m_Area.clear();
}


void MyEvent::on_button_clicked_open()
{
	Gtk::FileChooserDialog dialog("Please open a file",
								  Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	int result = dialog.run();

	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			m_Area.clear();
			
			std::string filename = dialog.get_filename();
			ifstream fichier(filename);
			initialisation(fichier);
			
			this->refresh_critere();
			
			m_Area.draw();
			
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			break;
		}
		default:
		{
			break;
		}
	}
}


void MyEvent::on_button_clicked_save()
{
	Gtk::FileChooserDialog dialog("Save your file",
								  Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);

	int result = dialog.run();

	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			std::string filename = dialog.get_filename();
			ville_sauvegarde(filename);

			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			break;
		}
		default:
		{
			break;
		}
	}
}


void MyEvent::on_button_clicked_shortest_path()
{
	if (m_Button_Shortest_Path.get_active())
	{
		if (ville_v_alive())
		{
			ville_sp();
			m_Area.refresh();
		}
		return;
	}
	ville_no_parent();
	m_Area.refresh();
}


void MyEvent::on_button_clicked_zoom_in()
{
	m_Area.zoom_in();
	
	refresh_critere();
	m_Area.refresh();
}


void MyEvent::on_button_clicked_zoom_out()
{
	m_Area.zoom_out();
	
	refresh_critere();
}


void MyEvent::on_button_clicked_zoom_reset()
{
	m_Area.zoom_reset();
	
	refresh_critere();
	m_Area.refresh();
}


void MyEvent::on_button_clicked_edit_link()
{
	if (m_Button_Edit_Link.get_active())
	{
		if (ville_v_alive())
		{
			return;
		}
		else
		{
			cout << "Selectionner un node avant" << endl;
			m_Button_Edit_Link.set_active(false);
			return;
		}
	}
}


string notation_s(double nombre)
{
	std::ostringstream stream_obj;
	stream_obj << nombre;
	return stream_obj.str();
}
