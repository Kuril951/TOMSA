#include <iostream>
#include <string>
#include <Position.h>
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <ConcaveToConvex.h>
#include <PointInPolygonAlgorithm.h>
#include <TOMSA.h>
#include <ODsay.h>
#include <Geolocation.h>
#include <UntwistLine.h>
#include <algorithm>

#include <Windows.h>		// todo : ����



using namespace std;


void timeMeasurement(vector<Position> &positions)
{
	LARGE_INTEGER liCounter1, liCounter2, liFrequency;

	// ���ļ�(1�ʴ� �����Ǵ� ī��Ʈ��)�� ���Ѵ�.
	QueryPerformanceFrequency(&liFrequency);
	QueryPerformanceCounter(&liCounter1);

	ODsay odsay;
	odsay.getPathMinTime(positions[0], positions[1]);
	/*TOMSA tomsa(positions);
	Position midPoint = tomsa.start();*/

	QueryPerformanceCounter(&liCounter2);
	printf("����ð� : %lf ��\n", (double)(liCounter2.QuadPart - liCounter1.QuadPart) / (double)liFrequency.QuadPart);
}

void initPositions(vector<Position> &positions, char *jsonString)
{
    rapidjson::Document document;
    document.Parse(jsonString);

    assert(document["userArr"].IsArray());
    const rapidjson::Value &userArr = document["userArr"];
    assert(userArr.IsArray());

    for(auto &user : userArr.GetArray()){
        assert(user["latitude"].IsDouble());
        assert(user["longitude"].IsDouble());
        positions.push_back(Position(user["latitude"].GetDouble(),
            user["longitude"].GetDouble()));
    }
}

int main(int argc, char *argv[])
{
    /*if(argc < 2){
        cout << "argument is few" << endl;
        exit(1);
    }*/
	/*
	// ������ �Ǵ� �α�
	char tmp[] = "{\"userArr\":[{\"latitude\":37.550277,\"longitude\":127.073053},\
	{\"latitude\":37.545036,\"longitude\":127.054245},\
	{\"latitude\":37.535413,\"longitude\":127.062388},\
	{\"latitude\":37.531359,\"longitude\":127.083799}]}";
	*/
	// ������
	/*char tmp[] = "{\"userArr\":[{\"latitude\":37.545036,\"longitude\":127.054245},\
	{\"latitude\":37.550277,\"longitude\":127.073053},\
	{\"latitude\":37.535413,\"longitude\":127.062388},\
	{\"latitude\":37.531359,\"longitude\":127.083799}]}";*/

	
	// ���� ���� ���� ����
	char tmp[] = "{\"userArr\":[{\"latitude\":37.565364,\"longitude\":126.985554},\
	{\"latitude\":37.544172,\"longitude\":127.067588},\
	{\"latitude\":37.487466,\"longitude\":127.026868},\
	{\"latitude\":37.508402,\"longitude\":126.941398}]}";
	

    vector<Position> positions;
    initPositions(positions, tmp);

	//timeMeasurement(positions);

	TOMSA tomsa(positions);
    Position midPoint = tomsa.start();
	
	cout << " < ������ ������(��ǥ) >" << endl;
	cout.precision(6);
    cout << fixed << "{ \"latitude\": " << midPoint.getLatitude()
        << ", \"longitude\": " << midPoint.getLongitude() << " }" << endl;

    return 0;
}