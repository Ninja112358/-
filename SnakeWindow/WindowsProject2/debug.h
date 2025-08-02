#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <consoleapi.h>

//调出控制台进行调试用的方法
void InitConsole() {
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
}