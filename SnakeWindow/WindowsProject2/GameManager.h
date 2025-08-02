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

	void Start();					//游戏初始化
	bool Update();					//更新游戏,每帧调用一次
	void Draw(HDC hdc);					//绘画
	void setHWND(HWND hWnd);		//设置窗口句柄
	void showScore();				//显示分数
	void initFood();				//初始化食物(根据map)
private:
	void updateMap(Snake& snake);
	void autoPlay();



	bool findWay(Point start, Point aim, Position& position, PointList& path);

	Position findPosition(Point p);		//找
	bool snakeSafe(PointList& path);
	void printMap();
	void fuckRule();
	Point findHead();


	bool findWayBfs(Point start, Point aim, PointList& path);	//bfs实现寻路算法
	bool findWayAStar(Point start, Point aim, PointList& path);	//A*实现寻路算法
	bool isTrue(int index,Point t);										//判断能不能走
};

int findManhattanDist(Point& a, Point& b);
