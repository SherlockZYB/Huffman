#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>
#include <string.h>
#include<algorithm>
#include <list>
#pragma once
using namespace std;


//����һ����㼯���洢�ַ���Ӧ��Ȩֵ��Ƶ�ʣ�
//��Ҫ��һ�����������ļ��е��ַ���Ȩֵȫ�����뵽��㼯nodes���У�ͬʱ��������ͬ�ַ���ӵ�LeafChars���棬��һ���ṹ�塣 
//����ע�⿼�ǻس� �� �ո񣡣� 

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
	HuffmanTreeNode * nodes;                //�洢�����Ϣ
	char* LeafChars;                        //Ҷ����ַ���Ϣ 
	string* LeafCharCodes;                  //Ҷ����ַ�������Ϣ 
	int curPos;                             //����ʱ�Ӹ��ڵ㵽Ҷ�ڵ�·���ĵ�ǰ���
	int num;                                //Ҷ�ڵ����

public:
	void Init(char ch[], int w[], int n);
	HuffmanTree();                          //���캯��
	string EnCode(char ch);                 //���� 
	char DeCode(string strCode);            //����
	void Select(int pos, int& r1, int& r2);     //�����ҳ���С������Ȩֵ 
	void Run();

};

template<class CharType, class WeightType>
void HuffmanTree<CharType, WeightType>::Select(int pos, int& r1, int& r2)
{
	//int i;                           //flag�����ҳ�Ȩֵ��С�ģ�����֮���ҵڶ�С�� 
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
	min = 10000;                           //����min��ֵ 
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

	nodes = new HuffmanTreeNode[m + 1];          //nodes[0]δ��
	LeafChars = new char[n + 1];
	LeafCharCodes = new string[n + 1];

	for (pos = 1; pos <= n; pos++)
	{//�洢Ҷ�����Ϣ 
		nodes[pos].weight = w[pos - 1];
		LeafChars[pos] = ch[pos - 1];
	}

	for (pos = n + 1; pos <= m; pos++)
	{//������������ 
		int r1, r2;
		Select(pos - 1, r1, r2);                              //��nodes[1~pos-1]��ѡ��˫��Ϊ0��Ȩֵ��С���������r1,r2

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
string HuffmanTree<CharType, WeightType>::EnCode(char ch)     //���� 
{
	for (int pos = 1; pos <= num; pos++)
	{
		if (LeafChars[pos] == ch)
			return LeafCharCodes[pos];
	}
}

template<class CharType, class WeightType>
char HuffmanTree<CharType, WeightType>::DeCode(string strCode)   //���� 
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
	int charNum = 0;                   //��ͬ�ַ����� 
	char str[100];                      //������ʱ���ȡ�����ַ� 
	int weight[100];                 //������ʱ����ַ���Ӧ��Ȩֵ 

	memset(str, 0, sizeof(str));
	memset(weight, 0, sizeof(weight));

	cout << "��������Ҫ����ַ����ļ������ƣ�" << endl;
	cin >> inFileName;
	cout << "��������Ҫ��ű�����ļ������ƣ�" << endl; 
	cin >> outFileName;

	inFile.open(inFileName);           //�������ļ� 
	//ʵ�ְ��ļ��е��ַ���Ȩ��Ū����,���ֱ�ŵ� * LeafChars�� HuffmanTreeNode.weight��  *** 
	while (!inFile.eof())                         //***�˴��������⣬���һ���ַ�������һ�� 
	{
		inFile >> ch1;
		if (strchr(str, ch1) == NULL)                           //chû���ֹ�
		{
			str[charNum] = ch1;
			weight[charNum]++;
			charNum++;
		}
		else                                                     //ch���ֹ�
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
	weight[i]--;                                        //���ڴ������һλ�ַ����ظ�һ�ε����� 
	inFile.close();
	//����Huffman��
	Init(str, weight, charNum);

	while (flag)
	{
		/*memset(str, 0, sizeof(str));
		memset(weight, 0, sizeof(weight));*/
		charNum = 0;    //��ʼ�� 
		cout << "1.����" << endl;
		cout << "2.����" << endl;
		cout << "3.�˳�����" << endl;
		cout << "��ѡ����Ҫ���еĲ�����" << endl;
		cin >> opr;
		if (opr == '1' || opr == '2')
		{
			//cout << "��������Ҫ����ַ����ļ������ƣ�" << endl;
			//cin >> inFileName;
			//cout << "��������Ҫ��ű�����ļ������ƣ�" << endl;  //***����Ϊʲô����Գ������� 
			//cin >> outFileName;

			//inFile.open(inFileName);           //�������ļ� 
			////ʵ�ְ��ļ��е��ַ���Ȩ��Ū����,���ֱ�ŵ� * LeafChars�� HuffmanTreeNode.weight��  *** 
			//while (!inFile.eof())                         //***�˴��������⣬���һ���ַ�������һ�� 
			//{
			//	inFile >> ch1;
			//	if (strchr(str, ch1) == NULL)                           //chû��FileContent�г��ֹ�
			//	{
			//		str[charNum] = ch1;
			//		weight[charNum]++;
			//		charNum++;
			//	}
			//	else                                                     //ch��FileContent�г��ֹ�
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
			//weight[i]--;                                        //���ڴ������һλ�ַ����ظ�һ�ε����� 
			//inFile.close();
			////����Huffman��
			//Init(str, weight, charNum);

			//����������ļ�
			
			//���б������ 
			if (opr == '1')
			{
				inFile.open(inFileName);
				outFile.open(outFileName);                      //���ַ��ļ����뵽�����ļ�
				char ch2;
				while (inFile >> ch2)                           //ʹ�������������жϣ���Ч���������һλ�ַ����ظ�����һ��
				{
					outFile << EnCode(ch2);
					outFile << '\n';
				}
				cout << "�ļ�����ɹ���" << endl;
				inFile.close();
				outFile.close();                                //�ر��ļ� 
			}
			//����������� 
			else
			{
				inFile.open(outFileName);
				outFile.open(inFileName);                      //�򿪱����ļ����뵽�ַ��ļ�
				string str;                                    //�����˴�ע�⣬��������������ļ��е�ÿ������Ӧ�����ɻس��ָ��ģ���Ȼ����һ���Զ�̫�� 
				while (inFile >> str)
				{
					outFile << DeCode(str);
				}
				cout << "�ļ�����ɹ���" <<endl;
				inFile.close();
				outFile.close();                                //�ر��ļ� 
			}
		}
		else if (opr == '3')
		{
			cout << "��л����ʹ�ã�" << endl;
			flag = 0;
		}
		else
		{
			cout << "ָ������������������룡" << endl;
			continue;
		}

	}
}