#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include<algorithm>
#include <list>
#pragma once
using namespace std;


//创建一个结点集，存储字符对应的权值（频率）
//需要有一个函数，将文件中的字符的权值全部读入到结点集nodes当中，同时将各个不同字符添加到LeafChars里面，用一个结构体。 
//！！注意考虑回车 、 空格！！ 

struct HuffmanTreeNode
{
	int weight;
	unsigned int parent=0, leftChild=0, rightChild=0;
};

template<class CharType, class WeightType>
class HuffmanTree
{
private:
	ofstream outFile;
	ifstream inFile;
	string inFileName;
	string outFileName;
	HuffmanTreeNode * nodes;                //存储结点信息
	char* LeafChars;                        //叶结点字符信息 
	string* LeafCharCodes;                  //叶结点字符编码信息 
	int curPos;                             //译码时从根节点到叶节点路径的当前结点
	int num;                                //叶节点个数

public:
	void Init(char ch[], int w[], int n);
	HuffmanTree();                          //构造函数
	string EnCode(char ch);                 //编码 
	char DeCode(string strCode);            //译码
	void Select(int pos, int& r1, int& r2);     //用于找出最小的两个权值 
	void Run();

};

template<class CharType, class WeightType>
void HuffmanTree<CharType, WeightType>::Select(int pos, int& r1, int& r2)
{
	//int i;                           //flag用于找出权值最小的，便于之后找第二小的 
	//int a[100];
	//for (i = 0; i <= pos - 1; i++)
	//{
	//	a[i] = nodes[i + 1].weight;
	//}
	//sort(a, a + pos);
	//for (i = 0; i <= pos - 1; i++)
	//{
	//	if (nodes[i + 1].weight == a[0] && nodes[i + 1].parent == 0)
	//	{
	//		r1 = i + 1;
	//		break;
	//	}
	//		
	//}
	//for (i = 0; i <= pos - 1; i++)
	//{
	//	if (i+1==r1)
	//		continue;
	//	else
	//	{
	//		if (nodes[i + 1].weight == a[1] && nodes[i + 1].parent == 0)
	//		{
	//			r2 = i + 1;
	//			break;
	//		}
	//	}
	//}

	int i, flag1, flag2=0;
	int min = 10000;
	for (i = 1; i <= pos; i++)
	{
		if (nodes[i].parent == 0 && nodes[i].weight < min)
		{
			min = nodes[i].weight;
			flag1 = i;
		}
	}
	r1 = flag1;
	min = 10000;                           //重置min的值 
	for (i = 1; i <=pos; i++)
	{
		if (i == r1)
			continue;
		else
		{
			if(nodes[i].parent == 0 && nodes[i].weight < min)
			{
				min = nodes[i].weight;
				flag2 = i;
			}
		}
	}
	r2 = flag2;
}

template<class CharType, class WeightType>
HuffmanTree<CharType, WeightType>::HuffmanTree()
{
}

template<class CharType, class WeightType>
void HuffmanTree<CharType, WeightType>::Init(char ch[], int w[], int n)
{
	num = n;
	int m = 2 * n - 1, pos;

	nodes = new HuffmanTreeNode[m + 1];          //nodes[0]未用
	LeafChars = new char[n + 1];
	LeafCharCodes = new string[n + 1];

	for (pos = 1; pos <= n; pos++)
	{//存储叶结点信息 
		nodes[pos].weight = w[pos - 1];
		LeafChars[pos] = ch[pos - 1];
	}

	for (pos = n + 1; pos <= m; pos++)
	{//建立哈夫曼树 
		int r1, r2;
		Select(pos - 1, r1, r2);                              //从nodes[1~pos-1]中选择双亲为0，权值最小的两个结点r1,r2

		nodes[r1].parent = nodes[r2].parent = pos;
		nodes[pos].leftChild = r1;
		nodes[pos].rightChild = r2;
		nodes[pos].weight = nodes[r1].weight + nodes[r2].weight;
	}

	for (pos = 1; pos <= n; pos++)
	{
		string charCode = "\0";
		for (unsigned int child = pos, parent = nodes[child].parent; parent != 0; child = parent, parent = nodes[child].parent)
		{
			if (nodes[parent].leftChild == child)
				charCode = charCode + "0";
			else
				charCode = charCode + "1";
		}
		LeafCharCodes[pos] = charCode;
	}
	curPos = m;
}


template<class CharType, class WeightType>
string HuffmanTree<CharType, WeightType>::EnCode(char ch)     //编码 
{
	for (int pos = 1; pos <= num; pos++)
	{
		if (LeafChars[pos] == ch)
			return LeafCharCodes[pos];
	}
}

template<class CharType, class WeightType>
char HuffmanTree<CharType, WeightType>::DeCode(string strCode)   //译码 
{
	for (int pos = 1; pos <= num; pos++)
	{
		if (LeafCharCodes[pos] == strCode)
			return LeafChars[pos];
	}
	/*string charList;
	for (int pos = 0; pos < strCode.size(); pos++)
	{
		if (strCode[pos] == '0')
			curPos = nodes[curPos].leftChild;
		else
			curPos = nodes[curPos].rightChild;

		if (nodes[curPos].leftChild == 0 && nodes[curPos].rightChild == 0)
		{
			charList += LeafChars[curPos];
			curPos = 2 * num - 1;
		}
	}
	return charList;*/
}

template<class CharType, class WeightType>
void HuffmanTree<CharType, WeightType>::Run()
{
	int flag = 1, i;
	char ch1;
	char opr;
	int charNum = 0;                   //不同字符个数 
	char str[100];                      //用于临时存放取出的字符 
	int weight[100];                 //用于临时存放字符对应的权值 

	memset(str, 0, sizeof(str));
	memset(weight, 0, sizeof(weight));

	cout << "请输入您要存放字符的文件的名称：" << endl;
	cin >> inFileName;
	cout << "请输入您要存放编码的文件的名称：" << endl; 
	cin >> outFileName;

	inFile.open(inFileName);           //打开输入文件 
	//实现把文件中的字符和权重弄出来,并分别放到 * LeafChars和 HuffmanTreeNode.weight中  *** 
	while (!inFile.eof())                         //***此处存在问题，最后一个字符会多输出一次 
	{
		inFile >> ch1;
		if (strchr(str, ch1) == NULL)                           //ch没出现过
		{
			str[charNum] = ch1;
			weight[charNum]++;
			charNum++;
		}
		else                                                     //ch出现过
		{
			for (i = 0; i < charNum; i++)
			{
				if (str[i] == ch1)
				{
					weight[i]++;
					break;
				}
			}
		}
	}
	weight[i]--;                                        //用于处理最后一位字符会重复一次的问题 
	inFile.close();
	//构建Huffman树
	Init(str, weight, charNum);

	while (flag)
	{
		/*memset(str, 0, sizeof(str));
		memset(weight, 0, sizeof(weight));*/
		charNum = 0;    //初始化 
		cout << "1.编码" << endl;
		cout << "2.译码" << endl;
		cout << "3.退出程序" << endl;
		cout << "请选择您要进行的操作：" << endl;
		cin >> opr;
		if (opr == '1' || opr == '2')
		{
			//cout << "请输入您要存放字符的文件的名称：" << endl;
			//cin >> inFileName;
			//cout << "请输入您要存放编码的文件的名称：" << endl;  //***这里为什么会调试出错？？？ 
			//cin >> outFileName;

			//inFile.open(inFileName);           //打开输入文件 
			////实现把文件中的字符和权重弄出来,并分别放到 * LeafChars和 HuffmanTreeNode.weight中  *** 
			//while (!inFile.eof())                         //***此处存在问题，最后一个字符会多输出一次 
			//{
			//	inFile >> ch1;
			//	if (strchr(str, ch1) == NULL)                           //ch没在FileContent中出现过
			//	{
			//		str[charNum] = ch1;
			//		weight[charNum]++;
			//		charNum++;
			//	}
			//	else                                                     //ch在FileContent中出现过
			//	{
			//		for (i = 0; i < charNum; i++)
			//		{
			//			if (str[i] == ch1)
			//			{
			//				weight[i]++;
			//				break;
			//			}
			//		}
			//	}
			//}
			//weight[i]--;                                        //用于处理最后一位字符会重复一次的问题 
			//inFile.close();
			////构建Huffman树
			//Init(str, weight, charNum);

			//打开输出输入文件
			
			//进行编码操作 
			if (opr == '1')
			{
				inFile.open(inFileName);
				outFile.open(outFileName);                      //打开字符文件输入到编码文件
				char ch2;
				while (inFile >> ch2)                           //使用这样的条件判断，有效避免了最后一位字符被重复调用一次
				{
					outFile << EnCode(ch2);
					outFile << '\n';
				}
				cout << "文件编码成功！" << endl;
				inFile.close();
				outFile.close();                                //关闭文件 
			}
			//进行译码操作 
			else
			{
				inFile.open(outFileName);
				outFile.open(inFileName);                      //打开编码文件输入到字符文件
				string str;                                    //！！此处注意，这种情况下输入文件中的每个编码应该是由回车分隔的，不然可能一次性读太多 
				while (inFile >> str)
				{
					outFile << DeCode(str);
				}
				cout << "文件译码成功！" <<endl;
				inFile.close();
				outFile.close();                                //关闭文件 
			}
		}
		else if (opr == '3')
		{
			cout << "感谢您的使用！" << endl;
			flag = 0;
		}
		else
		{
			cout << "指令输入错误，请重新输入！" << endl;
			continue;
		}

	}
}