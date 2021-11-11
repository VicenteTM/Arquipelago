//ville.cc
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#include "ville.h"


static Ville ville_v;


void Ville::lecteur_n(ifstream& fichier, char type_quartier)
{
	unsigned int nbNode;
	fichier >> nbNode;
	
	filter_in(fichier, nbNode);

	for (unsigned int i = 0; i < nbNode; i++)
	{
		unsigned int uid, capacity;
		double x, y;
		
		fichier >> uid;
		filter_in(fichier, uid);
		
		fichier >> x;
		fichier >> y;
		fichier >> capacity;
		
		quartiers.push_back(create_node(uid,capacity,x,y,type_quartier));
	}
}


void Ville::lecture_l(ifstream& fichier)
{
	unsigned int nbLink;
	fichier >> nbLink;
	
	filter_in(fichier, nbLink);
	
	for (unsigned int i = 0; i < nbLink; i++)
	{
		unsigned int uid1, uid2;
		fichier >> uid1;
		
		filter_in(fichier, uid1);
		
		fichier >> uid2;
		
		create_link(quartiers,uid1,uid2);
	}
}


void Ville::ville_reset()
{
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		delete quartiers[i];
		quartiers[i] = nullptr;
	}
	
	ville_alive = false;
	
	quartiers.clear();
}


void Ville::check_error(Error erreur)
{
	switch (erreur)
	{
		case E01: e01(quartiers); break;
		case E03: e03(quartiers); break;
		case E04: e04(quartiers); break;
		case E05: e05(quartiers); break;
		case E06: e06(quartiers); break;
		case E08: e08(quartiers); break;

		default: return;
	}
}


void Ville::no_parent()
{
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		quartiers[i]->set_parent_node(false);
		quartiers[i]->set_parent_current_node(false);
	}
}


void Ville::sp() 
{
	no_parent();
	unsigned int indice_depart;
	if (*current_node() != 'H')
	{
		return;
	}
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		if (quartiers[i] == current_node())
		{
			indice_depart = i;
			break;
		}
	}
	char d_type = 'P';
	for (int j = 0; j < 2; j++)
	{
		if (j != 0)
		{
			d_type = 'T';
		}
		
		if (dijkstra(indice_depart,d_type,quartiers) == no_link or 
		quartiers[dijkstra(indice_depart,d_type,quartiers)]->get_d_info().parent 
																			== no_link)
		{
			continue;
		}
		else
		{
			unsigned int parent_p = dijkstra(indice_depart,d_type,quartiers);
			while_sp(parent_p,d_type);
		}
	}
}


void Ville::while_sp(unsigned int parent_p,char d_type)
{
	do 
	{
		quartiers[parent_p]->set_parent_node(true,d_type);
		if (quartiers[parent_p]->get_d_info().parent == no_link)
		{
			quartiers[parent_p]->set_parent_node(false,d_type);
			break;
		}
		if (quartiers[quartiers[parent_p]->get_d_info().parent]
											->get_d_info().parent == no_link)
		{
			quartiers[parent_p]->set_parent_current_node(true);
		}
		parent_p = quartiers[parent_p]->get_d_info().parent;
	}while (parent_p != no_link);
}


double Ville::startDji()
{
	unsigned int nbLogement = 0;
	char d_type  = 'P';
	double mta = 0;
	
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		if (*quartiers[i] != 'H')
		{
			continue;
		}
		++ nbLogement;
		
		for (int j = 0; j < 2; j++)
		{
			if (j != 0)
			{
				d_type = 'T';
			}
			
			if (dijkstra(i,d_type,quartiers) == no_link)
			{
				mta += infinite_time;
			}
			else
			{
				mta += quartiers[dijkstra(i,d_type,quartiers)]->get_d_info().access;
			}
		}
	}
	if (nbLogement == 0)
	{
		return 0;
	}
	return mta/nbLogement;
}


double Ville::startENJ()
{
	if (quartiers.size() == 0)
	{
		return 0;
	}
	
	double nbpL(0);
	double nbpTP(0);
	
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		if (*quartiers[i] == 'H')
		{
			nbpL += quartiers[i]->get_cap();
		}
		else
		{
			nbpTP += quartiers[i]->get_cap();
		}
	}
	return (nbpL-nbpTP)/(nbpL+nbpTP);
}


double Ville::startCI()
{
	double ci(0);
	double capacite, vitesse;
	
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		for (size_t j = 0; j < quartiers[i]->get_nbVoisin(); j++)
		{
			if (*quartiers[i]->get_voisin(j) < *quartiers[i])
			{
				continue;
			}
			
			vitesse = default_speed;
			
			if (*quartiers[i] == 'T' and *quartiers[i]->get_voisin(j) == 'T')
			{
				vitesse = fast_speed;
			}
			
			if (quartiers[i]->get_cap() < quartiers[i]->get_voisin(j)->get_cap())
			{
				capacite = quartiers[i]->get_cap();
			}
			
			else
			{
				capacite = quartiers[i]->get_voisin(j)->get_cap();
			}
			
			Segment lien = quartiers[i]->show_segment(*quartiers[i]->get_voisin(j));
			
			ci += capacite*vitesse*lien.longueur();
		}
	}
	return ci;
}


void Ville::draw_all()
{
	for (unsigned int i = 0; i < quartiers.size(); i++)
	{
		quartiers[i]->draw_link();
	}
	
	for (unsigned int i = 0; i < quartiers.size(); i++)
	{
		quartiers[i]->draw();
	}
}


void Ville::sauvegarder_ville(string nom_sauvegarde)
{
	ofstream out(nom_sauvegarde);
	
	sauvegarde_noeud(out,quartiers,'H');
	sauvegarde_noeud(out,quartiers,'T');
	sauvegarde_noeud(out,quartiers,'P');
	
	sauvegarde_lien(out,quartiers);
}


unsigned int Ville::new_uid()
{
	bool uid_taken;
	unsigned int uid(0);
	
	do
	{
		uid_taken = false;
		++uid;
		
		for (size_t i = 0; i < quartiers.size(); i++)
		{
			if (*quartiers[i] == uid)
			{
				uid_taken = true;
			}
		}
	}while (uid_taken);
	
	return uid;
}


void Ville::add_node(Point p, char a)
{
	quartiers.push_back(create_node(new_uid(), min_capacity, p.x, p.y, a));
}


Node* Ville::p_in_ville(Point p)
{
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		if (quartiers[i]->p_in_node(p))
		{
			return quartiers[i];
		}
	}
	return nullptr;
}


void Ville::modify_current_node(Point p, bool edit_link, bool shortest_path)
{
	if (edit_link)
	{
		if (p_in_ville(p) == current_node())
		{
			cout << "No auto-link" << endl;
		}
		else if(p_in_ville(p) != nullptr)
		{
			 if (current_node()->check_voisin(p_in_ville(p)))
			{
				current_node()->delete_voisin(p_in_ville(p));
				p_in_ville(p)->delete_voisin(current_node());
			}
			else
			{
				current_node()->add_voisins(p_in_ville(p));
				p_in_ville(p)->add_voisins(current_node());
				
				if (ville_v.newLinkNodeSpp(current_node(),p_in_ville(p)) or 
					current_node()->check_max_link() or 
													p_in_ville(p)->check_max_link())
				{
					current_node()->delete_voisin(p_in_ville(p));
					p_in_ville(p)->delete_voisin(current_node());
					return;
				}
				if (shortest_path)
				{
					sp();
				}
			}
		}
	}
	else if (shortest_path)
	{
		if (p_in_ville(p) == current_node())
		{
			delete_node(current_node());
			set_ville_dead();
			no_parent();
		}
		else if(p_in_ville(p) != nullptr)
		{
			set_ville_dead();
			ville_alive = true;
			p_in_ville(p)->set_current_node(true);
			sp();
		}
	}
	else
	{
		if (p_in_ville(p) == current_node())
		{
			delete_node(current_node());
			set_ville_dead();
			no_parent();
		}
		else if(p_in_ville(p) != nullptr)
		{
			set_ville_dead();
			ville_alive = true;
			p_in_ville(p)->set_current_node(true);
		}
	}
}


void Ville::set_ville_dead()
{
	ville_alive = false;
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		if (quartiers[i]->get_current_node())
		{
			quartiers[i]->set_current_node(false);
		}
	}
	no_parent();
}


bool Ville::get_ville_alive() const
{
	return ville_alive;
}


void Ville::new_coordinates(Point p, bool shortest_path)
{
	current_node()->set_new_coordinates(p);
	if (ville_new_nodespp(p) or linksnodesspp())
	{
		current_node()->backup_centre();
	}
	
	if (shortest_path)
	{
		sp();
	}
	
	return;

}


void Ville::new_capacity(Segment seg)
{
	current_node()->make_new_capacity(seg);
	
	if (ville_new_nodespp())
	{
		current_node()->backup_cap();
	}
	return;

}

Node* Ville::current_node()
{
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		if (quartiers[i]->get_current_node())
		{
			return quartiers[i];
		}
	}
	
	return nullptr;
}


void Ville::delete_node(Node* dead_node)
{
	unsigned int k;
	
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		quartiers[i]->delete_voisin(dead_node);
		if (quartiers[i] == dead_node)
		{
			k = i;
		}
	}
	
	delete dead_node;
	quartiers[k] = quartiers[quartiers.size()-1];
	quartiers.pop_back();
}


bool Ville::newLinkNodeSpp(Node* node_1, Node* node_2)
{
	for (size_t i = 0; i < quartiers.size(); i++)
	{
		if (quartiers[i] != node_1 and quartiers[i] != node_2)
		{
			if (nodeLinkSuperposition(node_1->show_cercle(),node_2->show_cercle(),
									  quartiers[i]->show_cercle(),true))
			{
				cout << error::node_link_superposition(quartiers[i]->get_uid());
				return true;
			}
		}
	}
	
	return false;
}


bool Ville::newNodeLinkSpp(Point p)
{
	Cercle test;
	if (ville_alive)
	{
		test = current_node()->show_cercle();
		
		for (size_t i = 0; i < quartiers.size(); i++)
			for (size_t j = 0; j < quartiers[i]->get_nbVoisin(); j++)
			
				if(current_node() != quartiers[i] and 
				   current_node() != quartiers[i]->get_voisin(j))
				{
					if (nodeLinkSuperposition(quartiers[i]->show_cercle(),
						quartiers[i]->get_voisin(j)->show_cercle(),
						test,true))
					{
						cout << "Node Link Superposition" << endl;
						return true;
					}
				}
	}
	else
	{
		test = {sqrt(min_capacity), p};
		
		for (size_t i = 0; i < quartiers.size(); i++)
			for (size_t j = 0; j < quartiers[i]->get_nbVoisin(); j++)
			
				if (nodeLinkSuperposition(quartiers[i]->show_cercle(),
					quartiers[i]->get_voisin(j)->show_cercle(),
					test,true))
				{
					cout << "Node Link Superposition" << endl;
					return true;
				}
	}
	return false;
}


bool Ville::newNodeSpp(Point p)
{
	Cercle test;
	
	if (ville_alive)
	{
		test = current_node()->show_cercle();
		for (size_t i = 0; i < quartiers.size(); i++) 
		{
			if (nodeSuperposition(test,quartiers[i]->show_cercle(),true) and
				current_node() != quartiers[i])
			{
				cout << "error node node superposition" << endl;
				
				return true;
			}
		}
	}
	else
	{
		test = {sqrt(min_capacity), p};
		for (size_t i = 0; i < quartiers.size(); i++) 
		{
			if (nodeSuperposition(test,quartiers[i]->show_cercle(),true))
			{
				cout << "error node node superposition" << endl;
				
				return true;
			}
		}
	}
	return false;
}

bool Ville::linksnodesspp()
{
	for (size_t i = 0; i < current_node()->get_nbVoisin(); i++)
	{
		for (size_t j = 0; j < quartiers.size(); j++)
		{
			if (current_node()!=quartiers[j] and 
										quartiers[j] != current_node()->get_voisin(i))
			{	
				if (nodeLinkSuperposition(current_node()->show_cercle(),
					current_node()->get_voisin(i)->show_cercle(),
					quartiers[j]->show_cercle(),true))
				{
					cout << error::node_link_superposition(quartiers[j]->get_uid());
					return true;
				}
			}
		}
	}
		
	return false;
}


bool ville_new_nodespp(Point p)
{
	return (ville_v.newNodeSpp(p) or ville_v.newNodeLinkSpp(p));
}

void ville_new_capacity(Segment seg)
{
	return ville_v.new_capacity(seg);
}


void ville_new_coordinates(Point p, bool shortest_path)
{
	return ville_v.new_coordinates(p,shortest_path);
}


void ville_modify_current_node(Point p, bool edit_link, bool shortest_path)
{
	return ville_v.modify_current_node(p, edit_link, shortest_path);
}


void ville_v_dead()
{
	return ville_v.set_ville_dead();
}

bool ville_v_alive()
{
	return ville_v.get_ville_alive();
}


bool ville_p_in(Point p)
{
	return ville_v.p_in_ville(p);
}


void ville_add_node(Point p, char a)
{
	ville_v.add_node(p, a);
}


void ville_no_parent()
{
	ville_v.no_parent();
}

void ville_draw_all()
{
	ville_v.draw_all();
}


void ville_delete()
{
	reset_init_error();
	ville_v.ville_reset();	
}


void ville_sp()
{
	return ville_v.sp();
}


double villeENJ()
{
	return ville_v.startENJ();
}


double villeCI()
{
	return ville_v.startCI();
}


double villeMTA()
{
	return ville_v.startDji();
}


void ville_sauvegarde(string nom_sauvegarde)
{
	ville_v.sauvegarder_ville(nom_sauvegarde);
}


void sauvegarde_noeud(ofstream& out, const vector<Node*>& tab_n, char nodeType)
{	
	unsigned int nodeCount(0);
	
	for (size_t j = 0; j < tab_n.size(); j++)
		if (*tab_n[j] == nodeType)
			nodeCount ++;
			
	out << endl << nodeCount << endl;
	
	for (size_t j = 0; j < tab_n.size(); j++)
		if (*tab_n[j] == nodeType)
			out << *tab_n[j];
}


void sauvegarde_lien(ofstream& out, const vector<Node*>& tab_n)
{
	vector<vector<unsigned int>> tab_lien (0,vector<unsigned int>(2));
	
	for (size_t i = 0; i < tab_n.size(); i++)
		for (size_t j = 0; j < tab_n[i]->get_nbVoisin(); j++)
			if (*tab_n[i] < *tab_n[i]->get_voisin(j))
				tab_lien.push_back({tab_n[i]->get_uid(),
									tab_n[i]->get_voisin(j)->get_uid()});
	
	out << "\n#Lien\n" << tab_lien.size() << "\n";
	
	for (size_t i = 0; i < tab_lien.size(); i++)
		out << "\t" << tab_lien[i][0] << " " << tab_lien[i][1] << "\n";
}


void initialisation(ifstream& fichier)
{
	ville_delete();
	
	char type_quartier;
	
	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0: type_quartier = 'h'; break;
		case 1: type_quartier = 't'; break;
		case 2: type_quartier = 'p'; break;
		}
		
		ville_v.lecteur_n(fichier,type_quartier);		
	}
	
	ville_v.check_error(E01);
	
	ville_v.lecture_l (fichier);
	
	ville_v.check_error(E08);
	ville_v.check_error(E04);
	ville_v.check_error(E03);
	ville_v.check_error(E06);
	ville_v.check_error(E05);
	
	if (get_init_error())
	{
		ville_v.ville_reset();
		return;
	}

	cout << error::success();
}


void filter_in(ifstream& fichier, unsigned int & var)
{
	while (fichier.fail())
	{
		fichier.clear();
		fichier.ignore(numeric_limits<streamsize>::max(), '\n');
		fichier >> var;
	}
}

