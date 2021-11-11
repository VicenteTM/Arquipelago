//noeud.cc
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#include "noeuds.h"


using namespace std;


static bool init_error(0);


Node::Node(unsigned int uid, double x, double y, unsigned int capacity)
		  : uid(uid), capacity(capacity), current_node(false),
		    parent_node_t(false), parent_node_p(false), parent_current_node(false)
															 
{
	node_centre = {x,y};
}


unsigned int Node::get_cap() const
{
	return capacity;
}


void Node::backup_cap()
{
	capacity = capacity_backup;
}

Point Node::get_node_centre() const
{
	return node_centre;
}


void Node::backup_centre()
{
	node_centre = centre_backup;
}


unsigned int Node::get_uid() const
{
	return uid;
}


Info_D Node::get_d_info () const
{
	return d_info;
}


bool Node::get_current_node() const
{
	return current_node;
}


void Node::set_current_node(bool state)
{
	current_node = state;
}


void Node::set_parent_node(bool state, char d_type)
{
	if (d_type == 'P')
	{
		parent_node_p = state;
	}
	else if(d_type == 'T')
	{
		parent_node_t = state;
	}
	else
	{
		parent_node_p = state;
		parent_node_t = state;
	}
}


void Node::set_parent_current_node(bool state)
{
	parent_current_node = state;
}


bool Node::operator==(char const& type_test) const
{
	if (node_type == type_test)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator!=(char const& type_test) const
{
	if (node_type != type_test)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator==(unsigned int const& uid_test) const
{
	if (uid == uid_test)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator!=(unsigned int const& uid_test) const
{
	if (uid != uid_test)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator<(Node const& node_test) const
{
	if (uid < node_test.uid)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator==(Node const& noeud_test) const
{
	if (uid == noeud_test.uid)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator!=(Node const& noeud_test) const
{
	if (uid != noeud_test.uid)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator==(Node* node_test) const
{
	if (this == node_test)
	{
		return true;
	}
			
	return false;	
}


bool Node::operator!=(Node* node_test) const
{
	if (this != node_test)
	{
		return true;
	}
			
	return false;	
}



void Node::add_voisins(Node* voisin)
{
	tab_voisins.push_back(voisin);
}


void Node::delete_voisin(Node* voisin)
{
	for (size_t i = 0; i < tab_voisins.size(); i++)
	{		
		if (tab_voisins[i] == voisin)
		{
			tab_voisins[i] = tab_voisins[tab_voisins.size()-1];
			tab_voisins.pop_back();
		}
	}
}


bool Node::check_voisin(Node* voisin)
{
	if (this == voisin)
	{
		return false;
	}
	
	for (size_t i = 0; i < tab_voisins.size(); i++)
	{
		if (tab_voisins[i] == voisin)
		{
			return true;
		}
	}
	return false;
}


bool Node::check_green_voisin(unsigned int i)
{
	return (parent_node_t and tab_voisins[i]->parent_node_t) or 
		   (parent_node_p and tab_voisins[i]->parent_node_p) or 
		   (current_node and tab_voisins[i]->parent_current_node) or
		   (tab_voisins[i]->current_node and parent_current_node);
}


unsigned int Node::get_nbVoisin() const
{
	return tab_voisins.size();
}


Node* Node::get_voisin(unsigned int i) const
{
	return tab_voisins[i];
}


void Node::d_init(char d_type)
{
	d_info.in = true;
	
	if (d_type =='T' and node_type=='P')
	{
		d_info.in = false;
	}

	d_info.access = infinite_time;
	d_info.parent = no_link;
}


void Node::set_d_in (bool new_in)
{
	d_info.in = new_in;
}


void Node::set_d_access (double new_access)
{
	d_info.access = new_access;
}


void Node::set_d_parent (unsigned int new_parent)
{
	d_info.parent = new_parent;
}


void Node::set_new_coordinates(Point p)
{
	centre_backup = node_centre;
	node_centre = p;
}


void Node::make_new_capacity(Segment seg)
{
	Segment tmp_1 = {node_centre, seg.p1};
	Segment tmp_2 = {node_centre, seg.p2};
	
	double dist_fix = tmp_1.longueur() - sqrt(capacity);
	double nouveau_rayon =  tmp_2.longueur() - dist_fix;
	
	capacity_backup = capacity;
	
	capacity = pow(nouveau_rayon,2);
	
	if (nouveau_rayon < sqrt(min_capacity))
	{
		capacity = min_capacity;
	}
	else if (nouveau_rayon > sqrt(max_capacity))
	{
		capacity = max_capacity;
	}
}


Cercle Node::show_cercle()
{
	Cercle cercle;
	cercle.centre = node_centre;
	cercle.rayon = sqrt(capacity);
	return cercle;
}


Segment Node::show_segment(Node const& node_v)
{
	Segment segment {node_centre, node_v.node_centre};
	return segment;
}


void Node::draw_link()
{
	for (unsigned int i = 0; i < tab_voisins.size(); i++)
	{
		if (tab_voisins[i]->uid < uid)
		{
			continue;
		}
		
		Segment lien_tmp = this->show_segment(*tab_voisins[i]);
		if (check_green_voisin(i))
		{
			lien_tmp.draw_segment(GREEN);
			continue;
		}
		lien_tmp.draw_segment(BLACK);
	}
}


bool Node::p_in_node(Point p)
{
	return this->show_cercle().in_cercle(p);
}


void Housing::draw()
{
	if (current_node)
	{
		show_cercle().draw_cercle(RED);
		return;
	}
	else if(parent_node_p or parent_node_t)
	{
		show_cercle().draw_cercle(GREEN);
	}
	else
	{
		show_cercle().draw_cercle(BLACK);
	}
}


bool Housing::check_max_link()
{
	if (tab_voisins.size() > max_link)
	{
		cout << error::max_link(uid);
		return true;
	}
	
	return false;
}


void Transport::draw()
{
	if (current_node)
	{
		show_cercle().draw_cercle(RED);
		show_cercle().draw_etoile(RED);
		return;
	}
	else if(parent_node_p or parent_node_t)
	{
		show_cercle().draw_cercle(GREEN);
		show_cercle().draw_etoile(GREEN);
	}
	else
	{
		show_cercle().draw_cercle(BLACK);
		show_cercle().draw_etoile(BLACK);
	}
}

void Production::draw()
{
	if (current_node)
	{
		show_cercle().draw_cercle(RED);
		show_cercle().draw_rectangle(RED);
		return;
	}
		else if(parent_node_p or parent_node_t)
	{
		show_cercle().draw_cercle(GREEN);
		show_cercle().draw_rectangle(GREEN);
	}
	else
	{
		show_cercle().draw_cercle(BLACK);
		show_cercle().draw_rectangle(BLACK);
	}
}


ofstream& operator<<(ofstream& out, const Node& node_out)
{
	out << "\t" << node_out.get_uid() << " " 
		<< node_out.get_node_centre().x << " "
		<< node_out.get_node_centre().y << " "
		<< node_out.get_cap() << "\n";
	
	return out;
}




//FONCTIONS LECTURE
// ====================================================================================
Node* create_node(unsigned int uid, unsigned int capacity, double x, double y, 
														   char type_quartier)
{
	e07 (uid);
	e09 (capacity);
	e10 (capacity);
	
	switch (type_quartier)
	{
		case 'h':
		{
			Housing* new_housing = new Housing(uid,x,y,capacity);
			return new_housing;
		}
		case 't':
		{
			Transport* new_transport = new Transport(uid,x,y,capacity);
			return new_transport;
		}
		case 'p':
		{
			Production* new_production = new Production(uid,x,y,capacity);
			return new_production;
		}
	}
	return nullptr;
}


void create_link(vector<Node*>& tab_n, unsigned int uid1, unsigned int uid2)
{
	if (uid_to_node(uid1, tab_n) == nullptr or uid_to_node(uid2, tab_n) == nullptr)
	{
		return;
	}
		
	uid_to_node(uid1, tab_n) -> add_voisins(uid_to_node(uid2, tab_n));
	uid_to_node(uid2, tab_n) -> add_voisins(uid_to_node(uid1, tab_n));
}
// ====================================================================================

Node* uid_to_node(unsigned int uid,const vector<Node*>& tab_n)
{
	for (size_t i = 0; i < tab_n.size(); i++)
	{
		if (*tab_n[i] == uid)
		{
			return tab_n[i];
		}
	}
	
	e02(uid);
	
	return nullptr;
}

//~ FONCTIONS ERROR
// ====================================================================================
void e01 (const vector<Node*>& tab_n)
{
	if (init_error)
		return;
	
	for (size_t i = 0; i < tab_n.size(); i++)
	{
		for (size_t j = 0; j < tab_n.size(); j++)
		{
			if ( (i!=j) and *tab_n[i] == *tab_n[j])
			{
				cout << error::identical_uid(tab_n[i]->get_uid());
				init_error = 1;
				return;
			}
		}
	}
}


void e02 (const unsigned int & uid)
{
	if (init_error)
	{
		return;
	}
	
	cout << error::link_vacuum(uid);
	init_error = 1;
}

void e03 (const vector<Node*>& tab_n)
{
	if (init_error)
	{
		return;
	}
	
	for (size_t i = 0; i < tab_n.size() ; i++)
	{
		if (tab_n[i]->check_max_link())
		{
			init_error = 1;
			return;
		}
	}
	
}

void e04 (const vector<Node*>& tab_n)
{
	if (init_error)
	{
		return;
	}
	
	for (size_t i = 0; i < tab_n.size(); i++)
	{	
		for (size_t j = 0; j < tab_n[i]->get_nbVoisin(); j++)
		{
			for (size_t k = j+1; k < tab_n[i]->get_nbVoisin(); k++)
			{
				if (tab_n[i]->get_voisin(j) == tab_n[i]->get_voisin(k))
				{
					cout << error::multiple_same_link(tab_n[i]->get_uid(), 
									 tab_n[i]->get_voisin(j)->get_uid());
					init_error = 1;
					return;
				}
			}
		}
	}
}


void e05 (const vector<Node*>& tab_n)
{
	if (init_error)
	{
		return;
	}
	
	for (size_t i = 0; i < tab_n.size(); i++)
	{
		for (size_t j = 0; j < tab_n[i]->get_nbVoisin(); j++)
		{
			for (size_t k = 0; k < tab_n.size(); k++)
			{
				if (k!=i and *tab_n[k]!= *(tab_n[i]->get_voisin(j)))
				{	
					if (nodeLinkSuperposition(tab_n[i]->show_cercle(),
						tab_n[i]->get_voisin(j)->show_cercle(),
						tab_n[k]->show_cercle()))
					{
						cout << error::node_link_superposition(tab_n[k]->get_uid());
						init_error = 1;
						return;
					}
				}
			}
		}
	}
}


void e06 (const vector<Node*>& tab_n)
{
	if (init_error)
	{
		return;
	}
	
	if (tab_n.size() == 0)
	{
		return;
	}
		
		
	for (size_t i = 0; i < tab_n.size()-1; i++)
	{
		for (size_t j = i+1; j < tab_n.size(); j++)
		{
			if (nodeSuperposition(tab_n[i]->show_cercle(),tab_n[j]->show_cercle()))
			{
				cout << error::node_node_superposition(tab_n[i]->get_uid(),
													   tab_n[j]->get_uid());
				init_error = 1;
				return;
			}
		}
	}
	
	return;
}


void e07 (const unsigned int& uid)
{	
	if (init_error)
	{
		return;
	}
	
	if (uid == no_link)
	{
		cout << error::reserved_uid();
		init_error = 1;
		return;
	}
}


void e08 (const vector<Node*>& tab_n)
{
	if (init_error)
	{
		return;
	}
	
	for (size_t i = 0; i < tab_n.size(); i++)
	{
		for (size_t j = 0; j < tab_n[i]->get_nbVoisin(); j++)
		{
			if (*tab_n[i] == *(tab_n[i]->get_voisin(j)))
			{
				cout << error::self_link_node(tab_n[i]->get_uid());
				init_error = 1;
				return;
			}
		}
	}
}


void e09 (const unsigned int & capacity)
{
	if (init_error)
	{
		return;
	}
	
	if (capacity < min_capacity)
	{
		cout << error::too_little_capacity(capacity);
		init_error = 1;
		return;
	}
}


void e10 (const unsigned int & capacity)
{
	if (init_error)
	{
		return;
	}
	
	if (capacity > max_capacity)
	{
		cout << error::too_much_capacity(capacity);
		init_error = 1;
		return;
	}
}


bool get_init_error()
{
	return init_error;
}

// ====================================================================================
unsigned int dijkstra(unsigned int indice_depart, char d_type, vector<Node*>& tab_n)
{
	
	for (size_t i = 0; i < tab_n.size(); i++)
	{
		tab_n[i]->d_init(d_type);
	}

	tab_n[indice_depart]->set_d_access(0);
	vector <unsigned int> tA;
	
	
	for (size_t i = 0; i < tab_n.size(); i++)
	{
		tA.push_back(i);
	}

	tri_insertion_tA(tA,tab_n);
	unsigned int n;
	double alt;
	bool test_t;
	
	do
	{
		test_t = false;	
		for (size_t i = 0; i < tab_n.size(); i++)
		{
			if (tab_n[i]->get_d_info().in)
			{
				test_t = true;
			}
		}
		
		n = find_min_access(tA,tab_n);
		
		if (n == no_link)
		{
			return no_link;
		}
		
		if (*tab_n[n] == d_type)
		{
			return n;
		}
		
		tab_n[n]->set_d_in(false);
		
		for (size_t lv = 0; lv < tab_n[n]->get_nbVoisin(); lv++)
		{
			if (tab_n[n]->get_voisin(lv)->get_d_info().in)
			{
				alt = tab_n[n]->get_d_info().access + compute_access(n,lv,tab_n);
				
				if (tab_n[n]->get_voisin(lv)->get_d_info().access > alt)
				{
					tab_n[n]->get_voisin(lv)->set_d_access(alt);
					tab_n[n]->get_voisin(lv)->set_d_parent(n);
					tri_insertion_tA(tA,tab_n);
				}
			}
		}
	} 
	while (test_t);
	
	return no_link;
}


void tri_insertion_tA (vector <unsigned int>& tab, vector<Node*>& tab_n)
{
	for (size_t i=1 ; i <= tab.size()-1; i++) 
	{
		unsigned int j, tmp;
		j = i;
		
		while(j > 0 and 
			 tab_n[tab[j-1]]->get_d_info().access > tab_n[tab[j]]->get_d_info().access) 
		{
		  tmp = tab[j];
		  tab[j] = tab[j-1];
		  tab[j-1] = tmp;

		  j--;
		}
	}
}


unsigned int find_min_access(const vector <unsigned int>& tA, vector<Node*>& tab_n)
{
	unsigned int i = 0;
	
	while (not tab_n[tA[i]]->get_d_info().in)
	{
		++i;
		
		if (i == tab_n.size())
		{
			return no_link;
		}
	}
	return tA[i];
}


double compute_access(unsigned int n, unsigned int lv, vector<Node*>& tab_n)
{		
	Segment lien = tab_n[n]->show_segment(*tab_n[n]->get_voisin(lv));
	
	if (*tab_n[n] == 'T' and *tab_n[n]->get_voisin(lv) == 'T')
	{
			return ((lien.longueur())/fast_speed);
	}
		
	return ((lien.longueur())/default_speed);
}


void reset_init_error()
{
	init_error = false;
}
