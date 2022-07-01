#pragma once
#include "Tools.h"

class GameObject
{
public:
	GameObject(wd* wData, int x, int y, int speed, int color) {
		_x = x, _y = y, _speed = speed, _color = color;
		this->wData = wData;
	};

	int GetX();

	int GetY();

	void SetX(int x);

	void SetY(int y);

	virtual void DrawObject() = 0;

	bool IsObjectDelete();

	void DeleteObject();

protected:

	wd* wData;

	int _x, _y, _speed, _color;

	bool _deleteObject = false;

	void EraseObject();

	virtual ~GameObject() {
		delete this;
	}

};

class DynamicObject : public GameObject
{
public:

	DynamicObject(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};

	virtual void MoveObject() = 0;

protected:

	virtual void CheckNextStep() = 0;

	virtual void ChangeDirection() = 0;

	enum dir {
		UP,
		RIGHT,
		DOWN,
		LEFT,
		STOP
	};

	int _direction = STOP;
};

class Player : public DynamicObject
{
public:
	Player(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void MoveObject() override;

	void Death(bool& worldIsRun);

	int GetLifes();

protected:

	int _playerAnimation = 0, lifes = 3;

	static const int DIRECTION = 5;

	void CheckNextStep() override;

	void ChangeDirection() override;

	char16_t sprite[DIRECTION][1][2]
	{
		{
			u"V"
		},
		{
			u"<"
		},
		{
			u"^"
		},
		{
			u">"
		},
		{
			u"@"
		}
	};

};