#include "GameManager.h"
#include <iostream>

GameManager::GameManager() {
	hWnd = NULL;
	score = 0;
}

GameManager::~GameManager() {
}

void GameManager::Start() {
	MessageBox(hWnd, L"��Ϸ������ʼ,������׼��", L"��ʾ", MB_ICONINFORMATION);
}
size_t nb = 0;
wchar_t str[20];
bool GameManager::Update() {
	if (snake.hitWall()) {
		wsprintf(str, L"���ղ���Ϊ:%lu", nb);
		SetWindowText(hWnd, str);
		return false;
	}
	nb++;
#ifdef SHOW_STEPNUM
	wsprintf(str, L"��ǰ����Ϊ:%lu", nb);
	SetWindowText(hWnd, str);
#endif
	//��ʼ�������ƻ����Ķ���
	snake.moveSnake();
	if (!food.isExist())
		initFood();
	//updateMap(snake);
	//printMap();
	//�߳Ե���ʳ��
	if (snake.headHit(food.point)) {
		snake.pushSnake();
		food.hide();
		initFood();
		
		score += SCORE_INCREASE;
	}
	
	showScore();
	//�����ж���������滹���Զ�Ѱ·
	if (PLAY_MODE != 0)
		autoPlay();				//�Զ�Ѱ·�����߼�

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
	//settextstyle(20, 0, L"����");
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
	//����ʳ��
	map[mapFood.x][mapFood.y] = 2;
	//�����ߵ�����
	for (int i = 1; i < len; i++)
		map[mapSnakePoint[i].x][mapSnakePoint[i].y] = 1;
	//�ߺ��������β��
	map[mapSnakePoint[len - 1].x][mapSnakePoint[len - 1].y] = 3;
	//�ߵ���ʵβ��
	map[mapSnakePoint[len - 2].x][mapSnakePoint[len - 2].y] = 3;
	//��ͷ
	map[mapSnakePoint[0].x][mapSnakePoint[0].y] = 4;
}

void GameManager::autoPlay() {
	//Ϊ�˷���д�㷨��ÿһ��ѭ�����ﶼ��������һ�ε�ͼ����Ϣ(0:�յ�,1:ǽ,2:ʳ��,3��β,4:��ͷ)
	updateMap(snake);
	Position position = snake.position;
	size_t tail = snake.point.size() - 1;
	PointList path;
	PointList newPath;
	if (isSaveFlag && !ans.empty()) {
		//��������߳���ʳ��֮���ǰ�ȫ��,����ֱ�Ӱ��վ��԰�ȫ�ķ�ʽ�߾Ϳ�����
		size_t len = ans.size();
		snake.nextPosition(findPosition(ans[len - 1] - mapSnakePoint[0]));
		ans.pop_back();
		if (ans.empty())
			isSaveFlag = false;
		return;
	}
	//�ж��Ƿ���ͨ��ʳ��
	if (findWay(snake.point[0], food.point, position, path)) {
		//����ҵ���ͨ��ʳ���·��Ҫ�жϳ���ʳ����Ƿ����ҵ��Լ���β��
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
			std::cout << "����ʳ��֮�󲻰�ȫ" << std::endl;
			updateMap(snake);

			if (findWay(snake.point[0], snake.point[tail], newPos, newPath)) {
				fuckRule();
			}
			else {
				//std::cout << "����ʳ��֮�󲻰�ȫ?????????????" << std::endl;
				fuckRule();
			}
		}
	}
	else if (findWay(snake.point[0], snake.point[tail], position, newPath)) {
		//std::cout << "�Բ���ʳ������ֻ��׷β��" << std::endl;
		fuckRule();
	}
	else {
		std::cout << "?????????????" << std::endl;
		fuckRule();
	}

}
//���start��aim�����·,position��ʾ��start��aim���·����һ���ķ���,ans��ʾ���·��
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
//����һ��������ȥ��,�ж��߳���ʳ����Ƿ�ȫ
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
	//����������ѡ��һ����ȫ��λ�ã��ƶ�һ��
	//Ҫ��: 1.�ƶ�֮����뿴����β,ʹ�Լ������ڰ�ȫ״̬
	//      2.����ܱ��ж��������λ��,������β�������پ�����Զ��λ��
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
//���start��aim�����·,ans��ʾ���·��
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
//����Ѱ··��
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
