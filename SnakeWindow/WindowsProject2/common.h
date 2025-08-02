#pragma once
#include <vector>
#include <queue>
#include <windows.h>
#include <algorithm>
//基础配置项
#define PLAY_MODE			1					//游玩模式(0:手动游玩,1:自动寻路,2:手动寻路(需要按回车))
#define FIND_WAY_ALGORITHM	1					//寻路算法(0:BFS,1:A*)
//DEBUG配置
//#define SHOW_CONSOLE							//是否显示控制台
//#define SHOW_STEPNUM							//是否显示当前步数(步数在标题显示,开启后会导致蛇的移动速度变慢)
//窗口配置
#define WINDOW_WIDTH		400                 //窗口宽度
#define WINDOW_HEIGHT		400                 //窗口高度
#define POINT_WIDTH			40					//一个点的宽度(像素)
//游戏配置
#define SCORE_INCREASE		10                  //分数涨幅
#define DELAY				200					//手动游玩的延时
#define AUTO_DELAY			0					//自动寻路的延时
#define HAND_DELAY			10					//手动寻路的延时(如果手动的贪吃蛇巨快，建议调一下这个设置)

#define SNAKE_INIT_SIZE		4                   //蛇身初始化长度
#define SNAKE_COLOR_HEAD	RGB(255,0,0)		//蛇头颜色
#define SNAKE_COLOR_BODY	RGB(0,0,255)		//蛇身颜色
#define SNAKE_COLOR_TAIL	RGB(255,0,255)      //蛇身尾颜色
#define FOOD_COLOR			RGB(255,255,0)      //食物颜色
#define BACKGROUND_COLOR	RGB(255,255,255)	//背景颜色
//算法用的
#define MAP_WIDTH			WINDOW_WIDTH / POINT_WIDTH + 10		//虚拟地图宽度大小
#define MAP_HEIGHT			WINDOW_HEIGHT / POINT_WIDTH + 10	//虚拟地图高度大小

//通用的点类:由于工程量太小了，所以就把逻辑写这里了
struct Point {
	int x = 0, y = 0, dist = 0;
	bool operator==(Point p) {
		return x == p.x && y == p.y;
	}
	Point& operator=(const Point& p) {
		x = p.x;
		y = p.y;
		return *this;
	}
	Point& operator+=(const Point& p) {
		x += p.x;
		y += p.y;
		return *this;
	}
	Point& operator-=(const Point& p) {
		x -= p.x;
		y -= p.y;
		return *this;
	}
	Point& operator/=(int num) {
		x /= num;
		y /= num;
		return *this;
	}
	Point& operator*=(int num) {
		x *= num;
		y *= num;
		return *this;
	}
	Point operator+(Point p) {
		return { x + p.x , y + p.y };
	}
	Point operator-(Point p) {
		return { x - p.x , y - p.y };
	}
	Point operator/(int num) {
		return { x / num , y / num };
	}
	Point operator*(int num) {
		return { x * num , y * num };
	}
};
struct PointDistCmp {
	bool operator()(Point a, Point b) const {
		return a.dist > b.dist;
	}
};

using PointList = std::vector<Point>;
using PointQueue = std::queue<Point>;
using PointProiriyQueue = std::priority_queue<Point, PointList, PointDistCmp>;
#define Swap(a,b) std::swap(a,b)

enum Position { up, down, left, right };
