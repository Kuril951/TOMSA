#include <ConvexHull.h>
#include <UntwistLine.h>
#include <stack>

vector<Position> ConvexHull::getConvexHull(vector<Position>& points)
{
	UntwistLine::untwistLine(points);

	stack<int> convexHullIdx;

	convexHullIdx.push(0);
	convexHullIdx.push(1);

	int nextPointIdx = 2;
	
	// ��� �� Ž��
	while (nextPointIdx < points.size()) {
		while (convexHullIdx.size() >= 2) {
			int A, B;
			A = convexHullIdx.top();
			convexHullIdx.pop();
			B = convexHullIdx.top();
			// �ֻ�� �� 2���� �������� ���谡 ccw�϶����� pop
			if (ccw(points[A], points[B], points[nextPointIdx]) < 0) {
				convexHullIdx.push(A);
				break;
			}
		}
		convexHullIdx.push(nextPointIdx++);
	}

	// convex hull�� �����ϴ� ���� ����
	vector<Position> convexHull;

	while (!convexHullIdx.empty()) {
		convexHull.push_back(points[convexHullIdx.top()]);
		convexHullIdx.pop();
	}

	return convexHull;
}

// 3���� ���⼺�� �˷���
// ���� AB �� ���� AC�� CCW
// ��ȯ�� ret > 0 : �ݽð� ����, ret = 0 : ����, ret < 0 �ð����
long long ConvexHull::ccw(const Position & A, const Position & B, const Position & C)
{
	const long long e = 1e6;
	long long ax = A.getLongitude() * e, ay = A.getLatitude() * e;
	long long bx = B.getLongitude() * e, by = B.getLatitude() * e;
	long long cx = C.getLongitude() * e, cy = C.getLatitude() * e;

	return (bx - ax) *(cy - ay) - (by - ay)*(cx - ax);
}
