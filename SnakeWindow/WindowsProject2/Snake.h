#pragma once

#include <vector>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include "common.h"


class Snake {
public:
    PointList point;
    Position position;
    int width;
public:
    Snake();
    Snake(const Snake& snake);
    ~Snake();

    void drawSnake(HDC hdc);
    void pushSnake();
    void moveSnake();
    void keyDown();
    bool headHit(Point point);
    bool hitWall();
    void nextPosition(Position position);
};




