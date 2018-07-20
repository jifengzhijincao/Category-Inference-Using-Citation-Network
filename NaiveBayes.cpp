/*
	1.paper_dict\term_dict\author_dict 并没什么用，所以并未读入处理。
	2.朴素贝叶斯中的没有标签的样本不做训练和测试。
	编译时：
		1.用TEST：得到测试集占数据集20%-40%的准确率
			结果存于当前目录下的"NaiveBayes_score.txt"
			格式：index(比例) + "\t" + 准确率
		2.用GETPROB：针对生成的训练集和测试集得到相应的paper的标签以及标签的概率
			结果存于当前目录下的"paper_real_label_prob.txt"
			格式：paper + "\t" + true_label + "\t" + calcu_label + "\t" + prob
*/

#include<fstream>
#include<iostream>
#include<cstring>
#include<string>
#include<set>
#include<map>
#include<ctime>
#include<cmath>
#include<iterator>
#include<assert.h>
using namespace std;

#define DATANUM 19396

set<int> testSet;
set<int> trainSet;

/*
	数据集
*/
struct Datasets
{
	//  key: paper  val: label
	map<int, int> papers_and_labels;

	//  key: paper  val: terms
	map<int, set<int> >  papers_and_terms;

	//  key: term  val: paper
	map<int, set<int> > terms_and_papers;

	map<int, int> labelSet;

	//dictionary
	set<int> termSet;

	void openfile(string fileName, ifstream& readin)
	{
		readin = ifstream(fileName);
		assert(readin.is_open());
	}

	void closefile(ifstream& readin)
	{
		readin.close();
	}

	//get data(papers, terms, labels)
	Datasets(string PL_filePath,string PT_filePath)
	{
		ifstream readin;
		int id1, id2;

		cout << "get Dataset..." << endl;

		openfile(PL_filePath, readin);
		while (readin >> id1 >> id2)
		{
			papers_and_labels.insert(make_pair(id1, id2));
			if (id2 != 0)
			{
				labelSet[id2] = labelSet[id2] + 1;
				continue;
			}
		}
		closefile(readin);
		cout << "papers_and_labels got!\n";

		openfile(PT_filePath, readin);
		while (readin >> id1 >> id2)
		{
			papers_and_terms[id1].insert(id2);
			terms_and_papers[id2].insert(id1);
			termSet.insert(id2);
			readin >> id1;
		}
		closefile(readin);
		cout << "papers_and_terms got!\n";

	}

};

struct Bayes
{
	//id1：label id2：word  double：prob
	// beta(id1,id2) = double
	map<int, map<int, double> > beta;

	//label 出现的概率
	map<int, double> pai;

	//统计词频
	map<int, map<int, int> > wordINum_in_labelJ;
	map<int, int> wordsNum_in_labelJ;

	Datasets* dataset;

	//构造函数中统计词频
	Bayes(Datasets& dataset)
	{
		this->dataset = &dataset;
		set<int>::iterator trainIter = trainSet.begin();
		while (trainIter != trainSet.end())
		{
			int label = dataset.papers_and_labels[*trainIter];
			if (label == 0)
			{
				trainIter++;
				continue;
			}
			set<int>::iterator wordIter = dataset.papers_and_terms[*trainIter].begin();
			if (wordsNum_in_labelJ.find(label) == wordsNum_in_labelJ.end())
				wordsNum_in_labelJ[label] = 0;
			while (wordIter != dataset.papers_and_terms[*trainIter].end())
			{
				wordsNum_in_labelJ[label]++;
				if (wordINum_in_labelJ.find(label) != wordINum_in_labelJ.end())
					wordINum_in_labelJ[label][*wordIter]++;
				else
					wordINum_in_labelJ[label][*wordIter] = 0;
				wordIter++;
			}
			trainIter++;
		}
		cout << "NaiveBayes data got!\n";
	}

	//训练：各label出现的概率(pai[label]),各label中各个term出现的概率(beta[label][term])
	void train()
	{
		cout << "NaiveBayes training..." << endl;
		map<int, int>::iterator labelIter = dataset->labelSet.begin();
		int labelNums = 0;
		while (labelIter != dataset->labelSet.end())
		{
			labelNums += labelIter->second;
			set<int>::iterator termIter = dataset->termSet.begin();
			while (termIter != dataset->termSet.end())
			{
				beta[labelIter->first][*termIter]
					= (wordINum_in_labelJ[labelIter->first][*termIter] + 1)
					/ (double)(wordsNum_in_labelJ[labelIter->first] + dataset->termSet.size());
				termIter++;
			}
			labelIter++;
		}
		labelIter = dataset->labelSet.begin();
		while (labelIter != dataset->labelSet.end())
		{
			pai[labelIter->first] = labelIter->second / (double)labelNums;
			labelIter++;
		}
	}

	//根据得到的pai和beta给出一片论文类别的判断
	//返回label + prob
	pair<int,double> checkLabelForOne(int paper)
	{
		map<int, int>::iterator labelIter = dataset->labelSet.begin();
		int bestlabel = -1;
		double maxprob = 1 << 31;
		double allprob = 0;
		while (labelIter != dataset->labelSet.end())
		{
			int label = labelIter->first;
			set<int>::iterator paperTermIter = (dataset->papers_and_terms)[paper].begin();
			double prob = pai[label];
			while (paperTermIter != (dataset->papers_and_terms)[paper].end())
			{
				prob *= beta[label][*paperTermIter];
				paperTermIter++;
			}
			allprob += prob;
			if (prob > maxprob)
			{
				maxprob = prob;
				bestlabel = label;
			}
			labelIter++;

		}
		assert(bestlabel > 0);
		return make_pair(bestlabel, maxprob/allprob);
	}

	//针对测试集：得到所应用的朴素贝叶斯的准确率
	double test()
	{
		set<int>::iterator testSetIter = testSet.begin();
		int trueNum = 0;
		int allNum = 0;
		while (testSetIter != testSet.end())
		{
			allNum++;
			int paper = *testSetIter;
			int testLabel = checkLabelForOne(paper).first;
			int realLabel = dataset->papers_and_labels[paper];
			if (testLabel == realLabel)
				trueNum++;
			testSetIter++;
		}
		double score = trueNum / (double)allNum;
		return score;
	}

	void storeLabelAndProb(string filename)
	{
		map<int, int>::iterator paLaIter = dataset->papers_and_labels.begin();
		ofstream out = ofstream(filename);
		assert(out.is_open());
		while (paLaIter != dataset->papers_and_labels.end())
		{
			pair<int, double>ans = checkLabelForOne(paLaIter->first);
			out << paLaIter->first << "\t" << paLaIter->second
				<< "\t" << ans.first << "\t" << ans.second << "\n";
			paLaIter++;
		}
	}
};

//随机选取(%index)的数据作为测试集(testSet)，剩下的为训练集(trainSet)
void getRandom(Datasets& dataset, int index)
{
	srand(time(NULL));
	testSet.clear();
	trainSet.clear();
	int testNum = DATANUM*index / 100.0;
	for (int i = 0; i < testNum; ++i)
	{
		while (1)
		{
			int id = rand() % 19396 + 1;
			if (testSet.find(id) == testSet.end())
				if (dataset.papers_and_labels[id] != 0)
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

int main()
{
	string filePath1 = "../Preprocessing/paper_labels.txt";
	string filePath2 = "../Cora/PT.txt";
	Datasets dataset = Datasets(filePath1, filePath2);
	string outfile = "";
#ifdef TEST
	double score[25] = { 0 };
	outfile = "NaiveBayes_score.txt"
	for (int i = 20; i <= 40; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			getRandom(dataset, i);
			Bayes bayes = Bayes(dataset);
			bayes.train();
			score[i - 20] += bayes.test();
		}
		cout << "test: " << i << ' ' << "score: " << score[i - 20] / 5 << endl;
	}
	ofstream out = ofstream(outfile);
	for (int i = 0; i <= 20; ++i)
	{
		out << i + 20 << "\t" << score[i]\5 << endl;
	}
	out.close();
#endif

#ifdef GETPROB
	string trainFile = "train.txt";
	string testFile = "test.txt";
	outfile = "paper_real_label_prob.txt"
	ifstream in = ifstream(trainFile);
	assert(in.is_open());
	int id;
	while (in >> id)
	{
		trainSet.insert(id);
	}
	in = ifstream(testFile);
	assert(in.is_open());
	while (in >> id)
	{
		testSet.insert(id);
	}
	Bayes bayes = Bayes(dataset);
	bayes.train();
	bayes.storeLabelAndProb(outfile);
#endif
}
