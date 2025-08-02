#pragma once

#include <vector>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "common.h"


class Snake {
public:
    PointList point;    //蛇的坐标
    Position position;  //蛇当前的方向
    int width;          //蛇的宽度
public:
    Snake();
    Snake(const Snake& snake);
    ~Snake();

    void drawSnake(HDC hdc);                //画蛇
    void pushSnake();                       //添加蛇尾
    void moveSnake();                       //移动蛇
    void keyDown();                         //按键检测
    bool headHit(Point point);              //判断蛇头碰到了什么
    bool hitWall();                         //判断蛇是否撞墙
    void nextPosition(Position position);   //设置蛇下一步的方向
};




