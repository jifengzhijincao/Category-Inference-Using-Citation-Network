/*
  This file defines the basic elements composing the graph.
  The vertice represent the paper.
  The edge_cit represent the citation relation.
  The edge_aut represent the 
*/
class Edge_cit {
public:
	int citer;               // start
	int citee;               // end
	double factor;           // factor
	Edge_cit() {
		citer = citee = -1;
		factor = 0;
	}
};

class Edge_aut {
public:
	int id1;                 // paper 1
	int id2;				 // paper 2
	double factor;           // factor
	Edge_aut() {
		id1 = id2 = -1;
		factor = 0;
	}
};

class Vertice {
public:
	int id;                  // paper id
	int label;               // paper label, -1 represents unknown yet(different from 0)
	int author;              // paper author
	double prob[10];         // probability for each label
	Edge_cit *edge_cit_base; // base address of citation edge start from this vertice
	Edge_aut *edge_aut_base; // base address of same-author edge including this vertice
	

	Vertice() {
		id = 0; label = -1;
		for (int i = 0; i < 10; i++) 
			prob[i] = 0;
	}

	Vertice(int i, int l) {
		id = i; label = l;
		for (int j = 0; j < 10; j++)
			prob[j] = 0;
	}	
};
