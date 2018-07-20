/*
  This file is for testing the correctness of the graph
  You can modify the arguments of output and check them 
*/ 
#include <cstdio>
#include "make_graph.cpp"
void output(int id, Vertice *v) {
	Vertice *tmp = v + id;
	printf("%d %d\n", tmp->id, tmp->label);
	Edge_cit* ptr = tmp->edge_citer_base;
	while (ptr != NULL) {
		printf("%d cites %d\n", ptr->citer, ptr->citee);
		ptr = ptr->next;
	}	
}

int main()
{
	Vertice *v;
	char label_file[40] = "..\\Preprocessing\\paper_labels.txt";
	char cit_file[40] = "..\\Cora\\PP.txt";
	v = make_graph_label(v, label_file);
	make_graph_cit(v, cit_file);
	//output(12411, v);
	return 0;
}
