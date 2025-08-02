#pragma once
#include <windows.h>
#include "GameManager.h"
#include <iostream>
GameManager gameManager;
//��ʼ������
void Start(HWND hWnd) {
	gameManager.setHWND(hWnd);
	gameManager.Start();
}
//���º���(ÿ֡����һ��)
bool Update() {
	return gameManager.Update();
}
//�滭����
void Draw(HDC hdc) {
	gameManager.Draw(hdc);
}
//��������
void KeyDown() {
	if (PLAY_MODE == 0) 
		gameManager.snake.keyDown();
}
