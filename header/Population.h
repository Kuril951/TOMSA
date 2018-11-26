#ifndef __POPULATION_H__
#define __POPULATION_H__

#include <Position.h>
#include <vector>

using namespace std;

class Population
{
public:
	Position pos;		// �������� ��ġ��ǥ
	int degree;
	int dominatedCnt;	// �� �����ڸ� �����ϴ� �ٸ� �������� ��
	vector<int> cost;	// �� ����ڵ�κ����� �̵��ð� ���
	vector<int> dominationSet;
	int crowdingDistance; // �������� �Ÿ�

	Population(Position pos);

	bool operator < (const Population &A);
};

#endif
