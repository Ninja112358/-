#pragma once
#include <vector>
#include <queue>
#include <windows.h>
#include <algorithm>
//����������
#define PLAY_MODE			1					//����ģʽ(0:�ֶ�����,1:�Զ�Ѱ·,2:�ֶ�Ѱ·(��Ҫ���س�))
#define FIND_WAY_ALGORITHM	1					//Ѱ·�㷨(0:BFS,1:A*)
//DEBUG����
//#define SHOW_CONSOLE							//�Ƿ���ʾ����̨
//#define SHOW_STEPNUM							//�Ƿ���ʾ��ǰ����(�����ڱ�����ʾ,������ᵼ���ߵ��ƶ��ٶȱ���)
//��������
#define WINDOW_WIDTH		640                 //���ڿ��
#define WINDOW_HEIGHT		640                 //���ڸ߶�
#define POINT_WIDTH			40					//һ����Ŀ��(����)
//��Ϸ����
#define SCORE_INCREASE		10                  //�����Ƿ�
#define DELAY				200					//�ֶ��������ʱ
#define AUTO_DELAY			0					//�Զ�Ѱ·����ʱ
#define HAND_DELAY			10					//�ֶ�Ѱ·����ʱ(����ֶ���̰���߾޿죬�����һ���������)

#define SNAKE_INIT_SIZE		4                   //�����ʼ������
#define SNAKE_COLOR_HEAD	RGB(255,0,0)		//��ͷ��ɫ
#define SNAKE_COLOR_BODY	RGB(0,0,255)		//������ɫ
#define SNAKE_COLOR_TAIL	RGB(255,0,255)      //����β��ɫ
#define FOOD_COLOR			RGB(255,255,0)      //ʳ����ɫ
#define BACKGROUND_COLOR	RGB(255,255,255)	//������ɫ
//�㷨�õ�
#define MAP_WIDTH			WINDOW_WIDTH / POINT_WIDTH + 10		//�����ͼ��ȴ�С
#define MAP_HEIGHT			WINDOW_HEIGHT / POINT_WIDTH + 10	//�����ͼ�߶ȴ�С

//ͨ�õĵ���:���ڹ�����̫С�ˣ����ԾͰ��߼�д������
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
		return a.dist < b.dist;
	}
};

using PointList = std::vector<Point>;
using PointQueue = std::queue<Point>;
using PointProiriyQueue = std::priority_queue<Point, PointList, PointDistCmp>;
#define Swap(a,b) std::swap(a,b)

enum Position { up, down, left, right };