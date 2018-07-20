/*
  This is for omit the paper with unobserved label
  We will considere those papers when doing relational classification
  But for Naive Bayes and Logistic Regression, we donot need them
*/
#include <iostream>
#include <cstdio>
using namespace std;
int main()
{
	FILE *fread = fopen("D:\\Paper Network\\Preprocessing\\paper_labels.txt", "r");
	FILE *fwrite = fopen("paper_label_nonzero.txt", "w");
	
	char *topic = new char[100];
	int label;
	int count = 1;
	// There are 19396 lines in total
	while (fscanf(fread, "%s\t%d\n", topic, &label) && count <= 19396) {
		if (label > 0) 
			fprintf(fwrite, "%s %d\n", topic, label);
		count++;
	}
	fclose(fread);
	fclose(fwrite);
	return 0;
}
