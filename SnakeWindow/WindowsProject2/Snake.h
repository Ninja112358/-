#pragma once

#include <vector>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "common.h"


class Snake {
public:
    PointList point;    //�ߵ�����
    Position position;  //�ߵ�ǰ�ķ���
    int width;          //�ߵĿ��
public:
    Snake();
    Snake(const Snake& snake);
    ~Snake();

    void drawSnake(HDC hdc);                //����
    void pushSnake();                       //�����β
    void moveSnake();                       //�ƶ���
    void keyDown();                         //�������
    bool headHit(Point point);              //�ж���ͷ������ʲô
    bool hitWall();                         //�ж����Ƿ�ײǽ
    void nextPosition(Position position);   //��������һ���ķ���
};




