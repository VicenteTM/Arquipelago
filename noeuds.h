//noeuds.h
//Vicente Carbon
//Dimitri Jacquemont
//Rendu 3
//Architecture: Fig 11 b1

#ifndef NOEUD_HEADER_H
#define NOEUD_HEADER_H

#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
#include "error.h"
#include "tools.h"


struct Info_D
{
	bool in;
	double access;
	unsigned int parent;
};


class Node
{
public:
	Node (unsigned int uid, double x, double y, unsigned int capacity);
	virtual ~Node(){};
			
	unsigned int get_cap() const;
	void backup_cap();
	Point get_node_centre() const;
	void backup_centre();
	unsigned int get_uid() const;
	Info_D get_d_info () const;
	
	bool get_current_node() const; 
	void set_current_node(bool state);
	void set_parent_node(bool state,char d_type = 'h');
	void set_parent_current_node(bool state);
	
	bool operator==(char const& type_test) const;
	bool operator!=(char const& type_test) const;
	
	bool operator==(unsigned int const& uid_test) const;
	bool operator!=(unsigned int const& uid_test) const;
	
	bool operator<(Node const& node_test) const;
	bool operator==(Node const& noeud_test) const;	
	bool operator!=(Node const& noeud_test) const;
	
	bool operator==(Node* node_test) const;
	bool operator!=(Node* node_test) const;
	
	void add_voisins(Node* voisin);
	void delete_voisin(Node* voisin);
	bool check_voisin(Node* voisin);
	bool check_green_voisin(unsigned int i);
	unsigned int get_nbVoisin() const;
	Node* get_voisin(unsigned int i) const;
	
	void d_init(char d_type);
	void set_d_in(bool new_in);
	void set_d_access(double new_access);
	void set_d_parent(unsigned int new_parent);
	void set_new_coordinates(Point p);
	void make_new_capacity(Segment seg);
	
	Cercle show_cercle();
	Segment show_segment(Node const& node_v);
	
	virtual void draw() = 0;
	void draw_link();
	
	bool p_in_node(Point p);
	
	virtual bool check_max_link() {return false;};

protected:
	unsigned int uid;
	Point node_centre;
	Point centre_backup;
	unsigned int capacity;
	unsigned int capacity_backup;
	vector<Node*> tab_voisins;
	char node_type;
	
	Info_D d_info;
	
	bool current_node;
	bool parent_node_t;
	bool parent_node_p;
	bool parent_current_node;
	
};


class Housing: public Node
{
public:
	Housing (unsigned int uid, double x, double y, unsigned int capacity)
		: Node(uid, x, y, capacity) {node_type = 'H';}
		
	~Housing(){}
	
	bool check_max_link() override;
		
	void draw() override;
};


class Transport: public Node
{
public:
	Transport (unsigned int uid, double x, double y, unsigned int capacity)
		: Node(uid, x, y, capacity) {node_type = 'T';}
		
	~Transport(){}
	
	void draw() override;
};


class Production: public Node
{
public:
	Production (unsigned int uid, double x, double y, unsigned int capacity)
		: Node(uid, x, y, capacity) {node_type = 'P';}
		
	~Production(){}
	
	void draw() override;
};

ofstream& operator<<(ofstream& out, const Node& node_out);



Node* create_node(unsigned int uid, unsigned int capacity, 
				  double x, double y, char type_quartier);
void create_link(vector<Node*>& tab_n, unsigned int uid1,
									   unsigned int uid2);
									   
Node* uid_to_node(unsigned int uid, const vector<Node*>& tab_n);

void e01 (const vector<Node*>& tab_n);
void e02 (const unsigned int & uid);
void e03 (const vector<Node*>& tab_n);
void e04 (const vector<Node*>& tab_n);
void e05 (const vector<Node*>& tab_n);
void e06 (const vector<Node*>& tab_n);
void e07 (const unsigned int & uid);
void e08 (const vector<Node*>& tab_n);
void e09 (const unsigned int & capacity);
void e10 (const unsigned int & capacity);

bool get_init_error();

unsigned int dijkstra(unsigned int indice_depart, char d_type, vector<Node*>& tab_n);
void tri_insertion_tA(vector <unsigned int>& tab, vector<Node*>& tab_n);
unsigned int find_min_access(const vector <unsigned int>& tA, vector<Node*>& tab_n);
double compute_access(unsigned int n, unsigned int lv, vector<Node*>& tab_n);

void reset_init_error();

#endif // NOEUD_HEADER_H

