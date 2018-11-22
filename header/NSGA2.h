#ifndef __NSGA2_H__
#define __NSGA2_H__

#include <vector>
#include <Position.h>
#include<set>

using namespace std;

class NSGA2
{
private:
	vector<Position> users;

public:
	// times: n���� �����Լ�(user)���� �ķ��� ���� ��ǥ�� ���� ��ŭ�� ������
	vector<set<int>> fastNondominatedSort(vector<vector<int>> &times);
	vector<vector<int>> crowdingDistanceAssignment(vector<vector<int>> &times, vector<set<int>> &front);
	bool dominates(vector<int> &p, vector<int> &q);

public:
	NSGA2(vector<Position> &users);
	~NSGA2();
};


#endif