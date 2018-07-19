/*
  This is for transfering labels in paper_label.txt to top labels
  Compare the labels in paper_labels.txt with hierarchy of Cora,
  Modify and write into paper_labels.txt
*/
#include <iostream>
#include <cstdio>
using namespace std;
int dict[10][20] = 
{
    {1, 2, 8, 17, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {4, 16, 39, 53, 55, 57, 71, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {7, 28, 40, 66, 67, 69, 70, 9, 14, 15, 18, 21, 33, 45, 46, 59, 63, 65, 0, 0},
    {11, 22, 37, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {13, 38, 56, 73, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {20, 23, 34, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {25, 41, 52, 61, 78, 79, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {27, 29, 32, 43, 44, 47, 72, 74, 77, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {31, 48, 49, 60, 62, 64, 68, 75, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {36, 50, 51, 54, 58, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
int length_for_search[10] = {4, 7, 19, 3, 4, 4, 7, 9, 9, 5};
int main()
{
	FILE *fread = fopen("Cora\\paper_label.txt", "r");
	FILE *fwrite = fopen("paper_labels.txt", "w");
	
	char *topic = new char[100];
	int label;
	int count = 1;
	// There are 19396 lines in total
	while (fscanf(fread, "%s\t%d\n", topic, &label) && count <= 19396) {
		// Find the top catagory for the paper
		if (label > 0) {
			int flag = 0;
			for (int i = 0; i < 10; i++) {
				if (flag == 1)
					break;
				for (int j = 0; j < length_for_search[i]; j++) 
					if (dict[i][j] == label) {
						label = i + 1;
						flag = 1;
						break;
					}
			}
		}
		fprintf(fwrite, "%s %d\n", topic, label);
		count++;
	}
	fclose(fread);
	fclose(fwrite);
	return 0;
}
