#include "Snake.h"

Snake::Snake() {

	width = POINT_WIDTH;
	for (size_t i = 0; i < SNAKE_INIT_SIZE; i++) {
		if (point.empty())
			point.push_back({ WINDOW_WIDTH / 2,WINDOW_HEIGHT / 2 });
		else {
			auto it = point.end() - 1;
			point.push_back({ (*it).x + POINT_WIDTH,(*it).y });
		}
	}
	position = left;
}
Snake::Snake(const Snake& snake) {
	position = snake.position;
	width = snake.width;
	size_t len = snake.point.size();
	for (int i = 0; i < len; i++)
		point.push_back(snake.point[i]);
}
Snake::~Snake() {
}

void Snake::drawSnake(HDC hdc) {
	//绘制填充矩形
	size_t len = point.size();
	for (int i = 0; i < len - 1; i++) {
		HBRUSH hBrush;
		if (i == 0)
			hBrush = CreateSolidBrush(SNAKE_COLOR_HEAD);
		else if (i == len - 2)
			hBrush = CreateSolidBrush(SNAKE_COLOR_TAIL);
		else
			hBrush = CreateSolidBrush(SNAKE_COLOR_BODY);
		SelectObject(hdc, hBrush);
		Rectangle(hdc,point[i].x, point[i].y, point[i].x + width, point[i].y + width);

		DeleteObject(hBrush);
	}
}

void Snake::pushSnake() {
	point.push_back(*(point.end() - 1));
}

void Snake::moveSnake() {
	size_t len = point.size();
	for (int i = len - 1; i > 0; i--)
		point[i] = point[i - 1];
	switch (position) {
	case up:
		point[0].y -= width;
		break;
	case down:
		point[0].y += width;
		break;
	case left:
		point[0].x -= width;
		break;
	case right:
		point[0].x += width;
		break;
	}
}

void Snake::keyDown() {
	//异步获取键盘的值
	if (GetAsyncKeyState('A') || GetAsyncKeyState(VK_LEFT)) {
		nextPosition(left);
	}
	else if (GetAsyncKeyState('D') || GetAsyncKeyState(VK_RIGHT)) {
		nextPosition(right);
	}
	else if (GetAsyncKeyState('W') || GetAsyncKeyState(VK_UP)) {
		nextPosition(up);
	}
	else if (GetAsyncKeyState('S') || GetAsyncKeyState(VK_DOWN)) {
		nextPosition(down);
	}
	////同步获取键盘的值
	//char ch = _getch();
	//if (ch == 'A' || ch == 'a' || ch == 75) {
	//	nextPosition(left);
	//}
	//else if (ch == 'D' || ch == 'd' || ch == 77) {
	//	nextPosition(right);
	//}
	//else if (ch == 'W' || ch == 'w' || ch == 72) {
	//	nextPosition(up);
	//}
	//else if (ch == 'S' || ch == 's' || ch == 80) {
	//	nextPosition(down);
	//}
}

bool Snake::headHit(Point point) {
	return this->point[0] == point;
}

bool Snake::hitWall() {
	//判断是否撞到自身
	size_t len = point.size();
	for (int i = 1; i < len - 1; i++)
		if (headHit(point[i]))
			return true;

	//判断是否撞到边界
	//return point[0].x > WINDOW_WIDTH || point[0].x < -POINT_WIDTH || point[0].y > WINDOW_HEIGHT || point[0].y < -POINT_WIDTH;
	return point[0].x >= WINDOW_WIDTH || point[0].x < 0 || point[0].y >= WINDOW_HEIGHT || point[0].y < 0;
}

void Snake::nextPosition(Position position) {
	switch (position)
	{
	case up:
		if (this->position != down)
			this->position = up;
		break;
	case down:
		if (this->position != up)
			this->position = down;
		break;
	case left:
		if (this->position != right)
			this->position = left;
		break;
	case right:
		if (this->position != left)
			this->position = right;
		break;
	}
}
