#include "GameManager.h"
#include <iostream>

GameManager::GameManager() {
	hWnd = NULL;
	score = 0;
}

GameManager::~GameManager() {
}

void GameManager::Start() {
	MessageBox(hWnd, L"游戏即将开始,请做好准备", L"提示", MB_ICONINFORMATION);
}
size_t nb = 0;
wchar_t str[20];
bool GameManager::Update() {
	if (snake.hitWall()) {
		wsprintf(str, L"最终步数为:%lu", nb);
		SetWindowText(hWnd, str);
		return false;
	}
	nb++;
#ifdef SHOW_STEPNUM
	wsprintf(str, L"当前步数为:%lu", nb);
	SetWindowText(hWnd, str);
#endif
	//初始化并绘制基本的东西
	snake.moveSnake();
	if (!food.isExist())
		initFood();
	//updateMap(snake);
	//printMap();
	//蛇吃到了食物
	if (snake.headHit(food.point)) {
		snake.pushSnake();
		food.hide();
		initFood();
		
		score += SCORE_INCREASE;
	}
	
	showScore();
	//这里判断是玩家游玩还是自动寻路
	if (PLAY_MODE != 0)
		autoPlay();				//自动寻路游玩逻辑

	return true;
}

void GameManager::Draw(HDC hdc){
	food.drawFood(hdc);
	snake.drawSnake(hdc);
}

void GameManager::setHWND(HWND hWnd) {
	this->hWnd = hWnd;
}

void GameManager::showScore() {
	//wchar_t text[20] = L"";
	//wsprintf(text, L"score: %d", score);
	//settextcolor(LIGHTGREEN);
	//settextstyle(20, 0, L"楷体");
	//outtextxy((WINDOW_WIDTH - 100) / 2, 10, text);
}
void GameManager::initFood() {
	updateMap(snake);
	if (food.isExist())
		return;
	PointList points;
	Point tail;
	int width = WINDOW_WIDTH / POINT_WIDTH;
	int height = WINDOW_HEIGHT / POINT_WIDTH;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (map[x][y] == 0)
				points.push_back({ x,y });
			else if (map[x][y] == 3)
				tail = { x,y };
		}
	}
	if (!points.empty()) {
		int randIndex = rand() % points.size();
		food.point = points[randIndex] * POINT_WIDTH;
		food.flag = true;
	}
	else {
		food.point = tail * POINT_WIDTH;
		food.flag = true;
	}
}
void GameManager::updateMap(Snake& snake) {
	memset(map, 0, sizeof(map));
	mapFood = food.point / POINT_WIDTH;
	size_t len = snake.point.size();
	mapSnakePoint.resize(len);
	for (int i = 0; i < len; i++)
		mapSnakePoint[i] = snake.point[i] / POINT_WIDTH;
	//生成食物
	map[mapFood.x][mapFood.y] = 2;
	//生成蛇的身体
	for (int i = 1; i < len; i++)
		map[mapSnakePoint[i].x][mapSnakePoint[i].y] = 1;
	//蛇后面的虚拟尾巴
	map[mapSnakePoint[len - 1].x][mapSnakePoint[len - 1].y] = 3;
	//蛇的真实尾巴
	map[mapSnakePoint[len - 2].x][mapSnakePoint[len - 2].y] = 3;
	//蛇头
	map[mapSnakePoint[0].x][mapSnakePoint[0].y] = 4;
}

void GameManager::autoPlay() {
	//为了方便写算法，每一次循环这里都重新生成一次地图的信息(0:空地,1:墙,2:食物,3蛇尾,4:蛇头)
	updateMap(snake);
	Position position = snake.position;
	size_t tail = snake.point.size() - 1;
	PointList path;
	PointList newPath;
	if (isSaveFlag && !ans.empty()) {
		//如果虚拟蛇吃完食物之后是安全的,这里直接按照绝对安全的方式走就可以了
		size_t len = ans.size();
		snake.nextPosition(findPosition(ans[len - 1] - mapSnakePoint[0]));
		ans.pop_back();
		if (ans.empty())
			isSaveFlag = false;
		return;
	}
	//判断是否能通往食物
	if (findWay(snake.point[0], food.point, position, path)) {
		//如果找到了通往食物的路还要判断吃完食物后是否能找到自己的尾巴
		if (snakeSafe(path)) {
			updateMap(snake);
			ans = path;
			isSaveFlag = true;
			size_t len = ans.size();
			snake.nextPosition(findPosition(ans[len - 1] - mapSnakePoint[0]));
			ans.pop_back();
			//snake.nextPosition(position);
		}
		else {
			Position newPos;
			PointList newPath;
			std::cout << "吃了食物之后不安全" << std::endl;
			updateMap(snake);

			if (findWay(snake.point[0], snake.point[tail], newPos, newPath)) {
				fuckRule();
			}
			else {
				//std::cout << "吃了食物之后不安全?????????????" << std::endl;
				fuckRule();
			}
		}
	}
	else if (findWay(snake.point[0], snake.point[tail], position, newPath)) {
		//std::cout << "吃不到食物所以只能追尾巴" << std::endl;
		fuckRule();
	}
	else {
		std::cout << "?????????????" << std::endl;
		fuckRule();
	}

}
//求从start到aim的最短路,position表示从start到aim最短路的下一步的方向,ans表示最短路径
bool GameManager::findWay(Point start, Point aim, Position& position, PointList& ans) {
	if (start == aim)
		return true;
	start /= POINT_WIDTH;
	aim /= POINT_WIDTH;
	bool flag;
	if (FIND_WAY_ALGORITHM == 0)
		flag = findWayBfs(start, aim, ans);
	else
		flag = findWayAStar(start, aim, ans);
	size_t len = ans.size();
	if (len > 0)
		position = findPosition(ans[len - 1] - start);
	return flag;
}
Position GameManager::findPosition(Point p) {
	//int dx[4] = { 0,0,1,-1 };
	//int dy[4] = { 1,-1,0,0 };
	int index = 0;
	for (int i = 0; i < 4; i++)
		if (dx[i] == p.x && dy[i] == p.y)
			index = i;
	if (index == 0)
		return down;
	else if (index == 1)
		return up;
	else if (index == 2)
		return right;
	else if (index == 3)
		return left;
}
//假设一个虚拟蛇去吃,判断蛇吃完食物后是否安全
bool GameManager::snakeSafe(PointList& res) {
	Position p;
	PointList t;
	Snake newSnake(snake);
	int len = (int)res.size();
	for (int i = len - 1; i >= 0; i--) {
		newSnake.nextPosition(findPosition(res[i] - newSnake.point[0] / POINT_WIDTH));
		newSnake.moveSnake();
		if (newSnake.headHit(food.point)) {
			newSnake.pushSnake();
		}
		updateMap(newSnake);
	}
	return findWay(newSnake.point[0], newSnake.point[newSnake.point.size() - 1], p, t);
}

void GameManager::printMap() {
	char ch[10] = { '~','#','$','-','*' };
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			std::cout << ch[map[x][y]];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int findManhattanDist(Point& a, Point& b) {
	return abs(a.x - b.x) + abs(a.y - b.y);
}

void GameManager::fuckRule() {
	updateMap(snake);
	//printMap();
	//让蛇在四周选择一个安全的位置，移动一步
	//要求: 1.移动之后必须看见蛇尾,使自己保持在安全状态
	//      2.如果周边有多个这样的位置,找离蛇尾的曼哈顿距离最远的位置
	PointList nb = { };
	Point Max;
	for (int i = 0; i < 4; i++) {
		updateMap(snake);
		Point head = findHead();
		Point next = { head.x + dx[i],head.y + dy[i] };
		if (next.x < 0 || next.x >= mapWidth || next.y < 0 || next.y >= mapHeight)
			continue;
		if (map[next.x][next.y] == 1)
			continue;
		PointList ans = { next };
		if (snakeSafe(ans)) {
			nb.push_back(next);
		}
	}
	size_t len = nb.size();
	int maxLen = 0;

	static Point foodTemp, tailTemp;
	foodTemp = mapFood;
	tailTemp = mapSnakePoint[mapSnakePoint.size() - 2];
	std::sort(nb.begin(), nb.end(), [&](Point a, Point b) {
		return findManhattanDist(a, tailTemp) > findManhattanDist(b, tailTemp);
		});
	if (nb.size() > 0)
		Max = nb[0];
	updateMap(snake);
	snake.nextPosition(findPosition(Max - mapSnakePoint[0]));
}
Point GameManager::findHead() {
	Point ans;
	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			if (map[x][y] == 4)
				ans = { x,y };
		}
	}
	return ans;
}

bool isWalked[MAP_WIDTH][MAP_HEIGHT] = { 0 };
bool GameManager::findWayBfs(Point start, Point aim, PointList& ans) {
	memset(isWalked, 0, sizeof(isWalked));
	Point path[MAP_WIDTH][MAP_HEIGHT];
	PointQueue q;
	q.push(start);
	isWalked[start.x][start.y] = true;
	bool flag = false;
	while (!q.empty()) {
		auto t = q.front();
		q.pop();
		if (t == aim) {
			flag = true;
			break;
		}
		for (int i = 0; i < 4; i++) {
			if (!isTrue(i, t))
				continue;
			Point next = { t.x + dx[i],t.y + dy[i] };
			if (next.x < 0 || next.x >= mapWidth || next.y < 0 || next.y >= mapHeight)
				continue;
			if (isWalked[next.x][next.y] || map[next.x][next.y] == 1)
				continue;
			isWalked[next.x][next.y] = true;
			path[next.x][next.y] = t;
			q.push(next);
		}
	}
	if (!flag) {
		//MessageBox(hWnd, L"", L"", MB_OK);
		return false;
	}
	int x = aim.x, y = aim.y;
	while (1) {
		ans.push_back({ x,y });
		Point t = path[x][y];
		if (t == start)
			break;
		x = t.x, y = t.y;
	}
	return true;
}
int distance[MAP_WIDTH][MAP_HEIGHT] = { 0 };
//求从start到aim的最短路,ans表示最短路径
bool GameManager::findWayAStar(Point start, Point aim, PointList& ans) {
	memset(distance, 0x3f, sizeof(distance));
	memset(isWalked, 0, sizeof(isWalked));
	distance[start.x][start.y] = 1;
	PointProiriyQueue heap;
	Point path[MAP_WIDTH][MAP_HEIGHT];
	bool flag = false;
	heap.push({ start.x,start.y,1 + findManhattanDist(start, aim) });
	while (!heap.empty()) {
		auto t = heap.top();
		heap.pop();
		if (isWalked[t.x][t.y])
			continue;
		isWalked[t.x][t.y];
		if (t == aim) {
			flag = true;
			break;
		}
		for (int i = 0; i < 4; i++) {
			if (!isTrue(i, t))
				continue;
			Point next = { t.x + dx[i],t.y + dy[i] };
			if (next.x < 0 || next.x >= mapWidth || next.y < 0 || next.y >= mapHeight)
				continue;
			if (isWalked[next.x][next.y] || map[next.x][next.y] == 1)
				continue;
			if (distance[next.x][next.y] > distance[t.x][t.y] + 1) {
				distance[next.x][next.y] = distance[t.x][t.y] + 1;
				path[next.x][next.y] = t;
				heap.push({ next.x,next.y,distance[t.x][t.y] + 1 + findManhattanDist(next,aim) });
			}
		}
	}
	if (!flag) {
		//MessageBox(hWnd, L"", L"", MB_OK);
		return false;
	}
	int x = aim.x, y = aim.y;
	while (1) {
		ans.push_back({ x,y });
		Point t = path[x][y];
		if (t == start)
			break;
		x = t.x, y = t.y;
	}
	return true;
}
//限制寻路路径
bool GameManager::isTrue(int index,Point t){
	int row = t.y;
	int col = t.x;
	Position pos = findPosition({ dx[index] ,dy[index] });
	if (col % 2 == 1 && pos == up) 
		return true;
	if (col % 2 == 0 && pos == down) 
		return true;
	if (row % 2 == 1 && pos == right) 
		return true;
	if(row % 2 == 0 && pos == left) 
		return true;
	return false;
}
