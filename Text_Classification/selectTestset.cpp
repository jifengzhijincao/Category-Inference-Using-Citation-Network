/*
	选取的是测试集和训练集
	读入：当前目录的 paper_labels.txt
	训练集：当前目录的 train.txt
	测试集：当前目录的 test.txt
	要求输入：数据集用作测试集的数据的百分比
*/
#include<fstream>
#include<iostream>
#include<ctime>
#include<string>
#include<map>
#include<set>
#include<iterator>
#include<assert.h>
using namespace std;

#define DATANUM 19396
map<int, int> dataset;
set<int> testSet;
set<int> trainSet;

//随机选取(%index)的数据作为测试集(testSet)，剩下的为训练集(trainSet)
void getRandom(int index)
{
	srand(time(NULL));
	testSet.clear();
	trainSet.clear();
	int testNum = DATANUM*index / 100.0;
	for (int i = 0; i < testNum; ++i)
	{
		while (1)
		{
			int id = (rand() % 19396) + 1;
			if (testSet.find(id) == testSet.end())
				if (dataset[id] != 0)
				{
					testSet.insert(id);
					break;
				}
		}
	}
	for (int i = 1; i <= DATANUM; ++i)
	{
		if (testSet.find(i) == testSet.end())
			trainSet.insert(i);
	}
	cout << "train_set\\testset selected!\n";
}

void getdata(string file)
{
	ifstream in = ifstream(file);
	assert(in.is_open());
	int paper, label;
	while (in >> paper >> label)
	{
		dataset.insert(make_pair(paper, label));
	}
	in.close();
}

int main()
{
	string infile = "paper_labels.txt";
	int index;
	//cout << "input file: ";
	//cin >> infile;
	cout << "test num(%index): ";
	cin >> index;
	getdata(infile);
	string trainSetFile = "train.txt";
	string testSetFile = "test.txt";
	getRandom(index);
	ofstream out = ofstream(trainSetFile);
	set<int>::iterator pos = trainSet.begin();
	while (pos != trainSet.end())
	{
		out << *pos << endl;
		pos++;
	}
	out.close();
	out = ofstream(testSetFile);
	pos = testSet.begin();
	while (pos != testSet.end())
	{
		out << *pos << endl;
		pos++;
	}
	out.close();
}
