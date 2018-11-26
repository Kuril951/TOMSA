#ifndef __NSGA2_H__
#define __NSGA2_H__

#include <vector>
#include <Position.h>
#include <set>
#include <Population.h>

using namespace std;

class NSGA2
{
private:
	vector<Population> pop;		// ������ ����(g) * �����Լ��� �� ����(f) : (g*f) �迭
	const int maxGeneration = 50;	// �ִ� ���� Ƚ��
	const int geneCnt = 10;		//������ ��

public:
	vector<vector<int>> fastNondominatedSort(vector<Population> &pop);	// ��ȯ�� front
	vector<Population> calcCrowdingDistance(vector<Population> &pop, vector<vector<int>> &front);
	vector<Population> sortPopulation(vector<Population> &pop);
	bool dominates(Population &p, Population &q);		// p�� q�� �����ϴ��� �Ǵ�
	vector<vector<int>> nsga2();

public:
	NSGA2(vector<Population> &pop);
	~NSGA2();
};


#endif