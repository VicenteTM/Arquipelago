//projet.cc
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1


#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include "ville.h"
#include "gui.h"


using namespace std;


int main(int argc,char* argv[])
{
	if (argc == 1)
	{
		auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example");
		MyEvent eventWindow;
		eventWindow.set_default_size(default_drawing_size, default_drawing_size);
		return app->run(eventWindow);
	}
	
	if (argc == 2)
	{
		int fake_argc(1);
		
		ifstream fichier(argv[1]);
	
		if(fichier.fail()) 
		{
			cout << "error opening file" << endl;
			exit(1);
		}
		
		initialisation(fichier);
		fichier.close();
		
		auto app = Gtk::Application::create(fake_argc, argv, "org.gtkmm.example");
		MyEvent eventWindow;
		eventWindow.set_default_size(default_drawing_size, default_drawing_size);
		return app->run(eventWindow);
	}
	return 0;
}




