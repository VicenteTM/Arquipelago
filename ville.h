//ville.h
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1


#ifndef VILLE_HEADER_H
#define VILLE_HEADER_H


#include "noeuds.h"

enum Error {E01, E03, E04, E05, E06, E08};

class Ville
{
public:
	Ville():ville_alive(false){};
	
	void lecteur_n (ifstream& fichier, char type_quartier);
	void lecture_l(ifstream& fichier);
	
	void ville_reset();
	
	void check_error(Error erreur);
	
	void no_parent();
	void sp();
	void while_sp(unsigned int parent_p,char d_type);
	
	double startDji();
	double startENJ();
	double startCI();
	
	void draw_all();
	
	void sauvegarder_ville(string nom_sauvegarde);
	
	unsigned int new_uid();
	void add_node(Point p, char a);
	Node* p_in_ville(Point p);
	void modify_current_node(Point p, bool edit_link, bool shortest_path);
	void set_ville_dead();
	bool get_ville_alive() const;
	void new_coordinates(Point p, bool shortest_path);
	void new_capacity(Segment seg);
	Node* current_node();
	
	void delete_node(Node* dead_node);
	
	bool newLinkNodeSpp(Node* node_1, Node* node_2);
	bool newNodeLinkSpp(Point p);
	bool newNodeSpp(Point p);
	bool linksnodesspp();
private:
	vector<Node*> quartiers;
	bool ville_alive;
};

bool ville_new_nodespp(Point p={0,0});

void ville_new_capacity(Segment seg);
void ville_new_coordinates(Point p, bool shortest_path);
void ville_modify_current_node(Point p, bool edit_link, bool shortest_path);
void ville_v_dead();
bool ville_v_alive();
bool ville_p_in(Point p);
void ville_add_node(Point p, char a);
void ville_no_parent();
void ville_draw_all();
void ville_delete();
void ville_sp();

double villeENJ();
double villeCI();
double villeMTA();

void ville_sauvegarde(string nom_sauvegarde);
void sauvegarde_noeud(ofstream& out, const vector<Node*>& tab_n, char nodeType);
void sauvegarde_lien(ofstream& out, const vector<Node*>& tab_n);

void initialisation(ifstream& fichier);
void filter_in(ifstream& fichier, unsigned int & var);

#endif // VILLE_HEADER_H
