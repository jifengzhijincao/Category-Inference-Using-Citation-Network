/*
  This file defines the basic elements composing the graph.
  The vertice represent the paper.
  The edge_cit represent the citation relation.
  The edge_aut represent the 
*/
#include <cstddef>
#include <stdlib.h>
class Edge_cit {
public:
	int citer;               // start
	int citee;               // end
	double factor;           // factor
	Edge_cit* prev;          // linked list
	Edge_cit* next;          // linked list
	Edge_cit() {
		citer = citee = -1;
		factor = 0;
		prev = next = NULL;
	}
};

class Edge_aut {
public:
	int id1;                 // paper 1
	int id2;				 // paper 2
	double factor;           // factor
	Edge_aut* prev;          // linked list prev
	Edge_aut* next;          // linked list next
	Edge_aut() {
		id1 = id2 = -1;
		factor = 0;
		prev = next = NULL;
	}
};

class Vertice {
public:
	int id;                    // paper id
	int label;                 // paper label, -1 represents unknown yet(different from 0)
	int author;                // paper author
	double prob[10];           // probability for each label
	Edge_cit *edge_citer_base; // base address of citation edge start from this vertice
	Edge_cit *edge_citer_end;  // end address of citation edge start from this vertice
	Edge_cit *edge_citee_base; // base address of citation edge end at this vertice
	Edge_cit *edge_citee_end;  // end address of citation edge end at this vertice
	Edge_aut *edge_aut_base;   // base address of same-author edge including this vertice
	Edge_aut *edge_aut_end;    // base address of same-author edge including this vertice
	

	Vertice() {
		id = 0; label = -1;
		edge_aut_base = NULL;
		edge_citee_base = edge_citer_base = NULL;
		edge_citee_end = edge_citer_end = NULL;
		for (int i = 0; i < 10; i++) 
			prob[i] = 0;
	}

	Vertice(int i, int l) {
		id = i; label = l;
		edge_aut_base = NULL;
		edge_citee_base = edge_citee_base = NULL;
		for (int j = 0; j < 10; j++)
			prob[j] = 0;
	}	

	// Add citation edge as a citer
	void add_edge_citer(int citee) {
		Edge_cit *ptr = new Edge_cit;
		ptr->citer = this->id; 
		ptr->citee = citee;
		ptr->prev = edge_citer_end;
		if (edge_citer_end != NULL) 
			edge_citer_end->next = ptr;
		edge_citer_end = ptr;
		if (edge_citer_base == NULL)
			edge_citer_base = edge_citer_end;
		return;
	}
	// Add citation edge as a citee
	void add_edge_citee(int citer) {
		Edge_cit *ptr = (Edge_cit*)malloc(sizeof(Edge_cit));
		ptr->citee = this->id; ptr->citer = citer;
		ptr->prev = edge_citer_end; 
		if (edge_citee_end != NULL)
			edge_citee_end->next = ptr;
		edge_citee_end = ptr;
		if (edge_citee_base == NULL)
			edge_citee_base = edge_citee_end;
		return;
	}
	/*
	void add_edge_aut(int aut) {
		Edge_aut *ptr = new Edge_aut;
		ptr->id1 = this->id; ptr->id2 = aut;
		ptr->prev = edge_aut_end;
		if (edge_aut_end != NULL) 
			edge_aut_end->next = ptr;
		if (edge_aut_base == NULL)
			edge_aut_base = edge_aut_end;
		return;
	}
	*/
};
