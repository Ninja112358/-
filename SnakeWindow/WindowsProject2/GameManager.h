#pragma once
#include "Snake.h"
#include "Food.h"


class GameManager
{
private:
	HWND hWnd;
	int score = 0;
	int dx[4] = { 0,0,1,-1 };
	int dy[4] = { 1,-1,0,0 };
public:
	Snake snake;
	Food food;
	PointList ans;
	Point mapFood;
	PointList mapSnakePoint;
	bool isSaveFlag = false;
	int mapWidth = WINDOW_WIDTH / POINT_WIDTH;
	int mapHeight = WINDOW_HEIGHT / POINT_WIDTH;
	int map[MAP_WIDTH][MAP_HEIGHT] = { 0 };
public:
	GameManager();
	~GameManager();

	void Start();					//��Ϸ��ʼ��
	bool Update();					//������Ϸ,ÿ֡����һ��
	void Draw(HDC hdc);					//�滭
	void setHWND(HWND hWnd);		//���ô��ھ��
	void showScore();				//��ʾ����
	void initFood();				//��ʼ��ʳ��(����map)
private:
	void updateMap(Snake& snake);		//���µ�ͼ
	void autoPlay();					//�Զ�����

	bool findWay(Point start, Point aim, Position& position, PointList& path);	//�Զ�Ѱ·
	Position findPosition(Point p);		//ͨ������Point���ض�Ӧ�ķ���
	bool snakeSafe(PointList& path);	//����·���ж��������߹�����·��֮���Ƿ�ȫ
	void printMap();					//��ӡ��ͼ
	void fuckRule();					//һ���Ż�·���Ĺ���(���ٿն�����)
	Point findHead();					//��ȡͷ��


	bool findWayBfs(Point start, Point aim, PointList& path);	//bfsʵ��Ѱ·�㷨
	bool findWayAStar(Point start, Point aim, PointList& path);	//A*ʵ��Ѱ·�㷨
	bool isTrue(int index,Point t);								//�ж��ܲ�����
};

int findManhattanDist(Point& a, Point& b);		//����������������پ���
int findDiagonalDistance(Point& a, Point& b);	//����������ĶԽ��߾���
