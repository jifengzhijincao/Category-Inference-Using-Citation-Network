/*
  This file makes the paper network into a graph
  Take in the Vertice array base and generate the graph
*/
/*
  The process of the algorithm:
  1. Read the label file and initialize the vertices
  2. Add citer & citee edge
  3. Organize the edges in a linked list
*/

#include "elements.h"
#include <cstdio>
#include <iostream>
#define VERTICE 19396
#include <stdlib.h>

/*
  v_base:     the base of array of vertices
  label_file: file of labels
  cit_file:   file of citations
*/
// Construct the vertices
Vertice* make_graph_label(Vertice* v_base, char *label_file) {
	//Initialize the vertice array
	v_base = (Vertice*)malloc(sizeof(Vertice) * (VERTICE+1));
	// Fill in label and id
	int id, label; int count = 1; Vertice *tmp = v_base + 1;
	FILE *in_stream_label = fopen(label_file, "r");
	while (fscanf(in_stream_label, "%d %d\n", &id, &label) && count <= VERTICE) {
		count++;
		tmp->label = label;
		tmp->id = id;
		tmp++;
	}
	fclose(in_stream_label);
	return v_base;
}

// Construct the citation edges
void make_graph_cit(Vertice* v_base, char* cit_file) {
	// Build citation network
	// Buid up the edges
	FILE *in_stream_cit = fopen(cit_file, "r");
	int citer, citee, freq;
	while (fscanf(in_stream_cit, "%d%d%d\n", &citer, &citee, &freq)) {
		if (citer == 0)
			break;
		(v_base + citer)->add_edge_citer(citee);
		(v_base + citee)->add_edge_citee(citer);
	}
	fclose(in_stream_cit);
}

//void make_graph_aut(Vertice* v_base, char* aut_file)
