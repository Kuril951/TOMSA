#include <TOMSA.h>
#include <PointInPolygonAlgorithm.h>
#include <ODsay.h>
#include <math.h>
#include <utility>
#include <algorithm>
#include <ConcaveToConvex.h>
#include <UntwistLine.h>
#include <random> // ù ������ ��ǥ������ ���ϱ� ����
#include <time.h> //srand(time(NULL));
#include <iostream> //cout �� ��ȣ�ϴٰ� ���� ����
#include <codecvt> // utf-8 -> �ѱ� ��ȯ
#include <math.h>

#define END_GENERATION 5 // �� ������� ���� �� ���ΰ�
#define POPULATION 10 // �� ���� �����ڸ� ������ ���ΰ�.

TOMSA::TOMSA(vector<Position> points)
	:users(points)
{
	// �����ִ� �ٰ��� Ǯ��
	UntwistLine::untwistLine(points);
	// user ��ġ�� ��õ ���� ����
	boudaryPoints = ConcaveToConvex::convert(points);
}


/* �Էµ� ������ ��ǥ�� �̵��ð��� ����Ͽ� �߰����� ã�� �˰���
* return Position : ������ �߰� ��ǥ ��
*/
Position TOMSA::start()
{
	const bool NOT_INSIDE = false;
	double latVector = 0, lonVector = 0;		// �߰����� ���� ���͵��� �� ����
	int consideredUserCnt = users.size();	// ����ؾ� �� ���� ��
	int generation = 1; //����� üũ
	Position gene[POPULATION]; //���� ������ ������ ������ �迭
	int greatSolution = 0; // �������� �ε���
	double avgLat = 0.0, avgLng = 0.0;

	PointInPolygonAlgorithm PIPAlgorithm;

	/* �߰������� ���, ���ϰų� ���� �������ʴ� �� �ȿ��� �������� �߻�*/
	Position center = getCenterOfGravity();

	srand(time(NULL));
	
	for (int i = 0; i < POPULATION; i++) {
		gene[i].setLatitude(center.getLatitude() + ((rand() << 8) / 1000000.0));
		gene[i].setLatitude(gene[i].getLatitude() - ((rand() << 8) / 1000000.0));
		gene[i].setLongitude(center.getLongitude() + ((rand() << 8) / 1000000.0));
		gene[i].setLongitude(gene[i].getLongitude() - ((rand() << 8) / 1000000.0));

		// ���� �ۿ� ������� Ȯ��
		if (PIPAlgorithm.isInside(gene[i], boudaryPoints) == NOT_INSIDE) {
			// �ٽ� ���ϱ�
			i -= 1;
		}
	}
	
	Position nextGene[POPULATION]; // ��������� ������ �迭

	//�� ������ ���� ������ ������ �ð��� ������ 2���� �����迭 ����
	int **time;
	time = new int*[consideredUserCnt];

	for (int i = 0; i < consideredUserCnt; i++) {
		*(time + i) = new int[POPULATION];
	}


	while (true) 
	{
		int usercount = 0;

		// END_GENERATION�� �ش��ϴ� ������� ����.
		if (generation == END_GENERATION) {
			cout << END_GENERATION << "���뿡�� �����մϴ�" << endl;
			cout << "< " << END_GENERATION << "���� �����ڵ��� �������� �ɸ��� �ð��� >" << endl;
			for (Position point : users) {
				for (int i = 0; i < POPULATION; i++) {
					cout << time[usercount][i] << "��  ";
				}
				cout << endl;
				usercount++;
			}
			exit(1); //����
		}

		cout << generation << "��° ���� �����ڵ�" << endl;
		for (int i = 0; i < POPULATION; i++) {
			cout.precision(6);
			cout << fixed << "{ \"latitude\": " << gene[i].getLatitude()
				<< ", \"longitude\": " << gene[i].getLongitude() << " }" << endl;
		}

		//�ð� �ʱ�ȭ
		**time = { 0, };
		
		// �� ����ڵ��� ��ġ���� �� �����ڵ� ��ǥ���� ���µ� �ɸ��� �ð� ���ϱ�
		for (Position point : users) {

			for (int i = 0; i < POPULATION; i++) {
				time[usercount][i] = getPathTime(point, gene[i]);
				cout << (usercount + 1) << "��° ������ " << i+1 << "��° ������ ��ǥ���� ���µ� �ɸ� �ð� : " << time[usercount][i] << endl;
			}

			usercount++;
		}

		//�̹����뿡 �����ذ� �����ϴ��� Ȯ�� -> �����ϸ� ��ȯ�ϰ� ����
		if (isOptimizedResult(time, consideredUserCnt, &greatSolution)) {
			/* ��ο� ���� ���� string����..
			usercount = 0;
			cout << "< ���� ��� >" << endl;
			for (Position point : users) {
				
				//??????
				std::wstring_convert < std::codecvt_utf8_utf16<wchar_t>> convert;
				std::wstring path = convert.from_bytes(convert);
				

				//�ѱ� ���� ���� �߻� 
				cout << usercount + 1 << "��° ����ڰ� ������ ���� ���� ���" << endl << getPath(point, gene[greatSolution]) << endl;
				usercount++;
			}
			*/
			return gene[greatSolution];
		}

		// ������ ���� �ٲٱ�
		for (int i = 0; i < POPULATION; i++) {
			if (i == (rand() % POPULATION)) {
				nextGene[i] = gene[i]; // ���� ����� ���⸦ ���ù��� ���
			}
			else { // �������� ����
				if (rand() % 2) {
					nextGene[i].setPosition(gene[i].getLatitude() + ((rand() % 10000) / 200000.0), gene[i].getLongitude() + ((rand() % 10000) / 200000.0) );
				}
				else {
					nextGene[i].setPosition(gene[i].getLatitude() - ((rand() % 10000) / 200000.0), gene[i].getLongitude() - ((rand() % 10000) / 200000.0));
				} //if-else in else
			}// if-else
		}//for i

		// ���� �ۿ� ������� Ȯ��
		// TODO : ������� ��ǥ�� ���� �ű��� �����ؾߵ�
		for (int i = 0; i < POPULATION; i++) {
			if (PIPAlgorithm.isInside(nextGene[i], boudaryPoints) == NOT_INSIDE) {
				// ��� ����������� ��ǥ�� ������ �ٽñ��ϱ�.
				if (rand() % 2) {
					nextGene[i].setPosition(gene[i].getLatitude() + ((rand() % 10000) / 200000.0), gene[i].getLongitude() + ((rand() % 10000) / 200000.0));
				}
				else {
					nextGene[i].setPosition(gene[i].getLatitude() - ((rand() % 10000) / 200000.0), gene[i].getLongitude() - ((rand() % 10000) / 200000.0));
				} //if-else in else
			}
		}

		/*
		else if (isOnWater()) {
			// �� ����� ��ǥ ���� ����?
		}
		*/

		//���� ����� ����
		for (int i = 0; i < POPULATION; i++) {
			gene[i] = nextGene[i];
		}

		generation++; // ���� ����

	}//while

	//�Ҵ� ����
	for (int i = consideredUserCnt - 1; i <= 0; i--) {
		delete[] * (time + i);
	}

	delete[] time;
}

/* �����߽� ��ǥ ���ϱ�
 * return Position : �����߽� ��ǥ
 */
Position TOMSA::getCenterOfGravity()
{
	double latitude = 0, longitude = 0;
	for (Position point : users) {
		latitude += point.getLatitude();
		longitude += point.getLongitude();
	}

	latitude /= users.size();
	longitude /= users.size();

	return Position(latitude, longitude);
}

/* �ּ� �̵� ��� �ð��� ã�� ��ȯ
 * parameters -> src : ���� ��ǥ, dest : ������ ��ǥ
 * return int : �ּҽð�
 */
int TOMSA::getPathTime(const Position &src, const Position &dest)
{
	return odsay.getPathMinTime(src, dest);
}

/* ��������� ������ ���� ���� ��� ��ȯ */
string TOMSA::getPath(const Position &src, const Position &dest) {
	return odsay.getPathInfo(src, dest);
}


/* ã�� �߰� ���� ����ȭ�� ������� �Ǻ��ϴ� �޼ҵ�
 * MIN_TIME_INTERVAL ���ݿ� ���ϴ� �������� �׷�ȭ �Ͽ�
 * Ư�� �ο��� �׷쿡 �����ִٸ� ����ȭ �Ǿ��� �Ǵ�
 * parameters -> times : �������� �̵��ð�, userCnt : �����ؾߵ� ���� ��
 * return true/false 
 */
bool TOMSA::isOptimizedResult(vector<int> times, int userCnt)
{
	// �׷�ȭ�� �������� �̵��ð��� ����
	const int MIN_TIME_INTERVAL = 10;

	sort(times.begin(), times.end());

	int minTimeOfGroup = -1;	// �ش� �׷���� ������ �ּ� �̵��ð�
	int userCntInGroup = 0;		// �� �׷����� �����ִ� ���� ��

	for (int time : times) {
		if (userCntInGroup == 0) {
			minTimeOfGroup = time;
			userCntInGroup++;
		}
		else if (time <= minTimeOfGroup + MIN_TIME_INTERVAL)
			userCntInGroup++;
		else
			userCntInGroup = 0;

		if (userCntInGroup >= userCnt)
			break;
	}

	if (userCntInGroup >= userCnt)
		return true;
	return false;
}

bool TOMSA::isOptimizedResult(int **time, int userCnt, int *greatSolution)
{
	// �׷�ȭ�� �������� �̵��ð��� ����
	const int MIN_TIME_INTERVAL = 5;

	//int minTimeOfGroup = -1;	// �ش� �׷���� ������ �ּ� �̵��ð�
	int *minTimeOfGroup = new int[POPULATION];
	//int userCntInGroup = 0;		// �� �׷����� �����ִ� ���� ��
	int *maxTimeOfGroup = new int[POPULATION];

	for (int i = 0; i < POPULATION; i++) {
		*(minTimeOfGroup + i) = 999;
		*(maxTimeOfGroup + i) = 0;
	}
	
	for (int j = 0; j < POPULATION; j++) {
		for (int i = 0; i < userCnt; i++) {

			//�ּҽð� ����
			if (time[i][j] < minTimeOfGroup[j])
				minTimeOfGroup[j] = time[i][j];

			//�ִ�ð� ����
			if (time[i][j] > maxTimeOfGroup[j])
				maxTimeOfGroup[j] = time[i][j];

		} //for i
	}//for j

	for (int i = 0; i < POPULATION; i++) {
		//�ִ�ð� - �ּҽð� = 3�� ���ϸ� �������� -> ����
		if ((maxTimeOfGroup[i] - minTimeOfGroup[i]) <= MIN_TIME_INTERVAL) {
			*greatSolution = i;
			return true;
		}
	}

	delete[] maxTimeOfGroup;
	delete[] minTimeOfGroup;

	return false;
}