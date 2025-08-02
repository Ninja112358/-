#pragma once
#include <windows.h>
#include "GameManager.h"
#include <iostream>
GameManager gameManager;
//初始化函数
void Start(HWND hWnd) {
	gameManager.setHWND(hWnd);
	gameManager.Start();
}
//更新函数(每帧调用一次)
bool Update() {
	return gameManager.Update();
}
//绘画函数
void Draw(HDC hdc) {
	gameManager.Draw(hdc);
}
//按键函数
void KeyDown() {
	if (PLAY_MODE == 0) 
		gameManager.snake.keyDown();
}
