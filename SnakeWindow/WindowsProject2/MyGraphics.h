#pragma once
#include <windows.h>
#include "GameManager.h"
#include <iostream>
GameManager gameManager;
void Start(HWND hWnd) {
	gameManager.setHWND(hWnd);
	gameManager.Start();
}
bool Update() {
	return gameManager.Update();
}
void Draw(HDC hdc) {
	gameManager.Draw(hdc);
}
void KeyDown() {
	if (PLAY_MODE == 0) 
		gameManager.snake.keyDown();
}
