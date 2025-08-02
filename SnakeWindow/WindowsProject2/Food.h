#pragma once
#include "common.h"
class Food
{
public:
    Point point;
    bool flag;
public:
    Food() {
        point = { 0,0 };
        flag = false;
    }
    ~Food() {

    }
    bool isExist() {
        return flag;
    }
    void drawFood(HDC hdc) {
        if (isExist()) {
            //setfillcolor(FOOD_COLOR);
            HBRUSH hBrush = CreateSolidBrush(FOOD_COLOR);
            SelectObject(hdc, hBrush);
            Rectangle(hdc,point.x, point.y, point.x + POINT_WIDTH, point.y + POINT_WIDTH);
            DeleteObject(hBrush);
        }
    }
    void hide() {
        flag = false;
    }


};

