#pragma once
#include "Tools.h"

#define SPRITE_HEIGHT 1
#define SPRITE_WIDTH 4
#define WALL_HEIGHT 10
#define WALL_WIDTH 21
#define WALL_COLOR White


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

	virtual void EraseObject();

protected:

	wd* wData;

	int _x, _y, _speed, _color;

	bool _deleteObject = false;

	virtual ~GameObject() {
		delete this;
	}

};

class DynamicObject : public GameObject
{
public:

	DynamicObject(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};

	virtual void MoveObject() = 0;

	int GetDirection();

	void UnderMoneyBag();

protected:

	virtual void ChangeDirection() = 0;

	void BagCollision();

	bool _isFalling = false;

	int _direction = STOP;
};

class Player : public DynamicObject
{
public:
	Player(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void MoveObject() override;

	void Death(bool& worldIsRun);

	void Immortal(bool &immortal);

	int GetBulletDirection();

	int GetLifes();

private:
	
	static const int DIRECTION = 5;

	int _playerAnimation = 0, lifes = 3, _lastDir = STOP;

	void ChangeDirection() override;

	char16_t playerSprite[DIRECTION][SPRITE_HEIGHT][SPRITE_WIDTH]
	{
		{
			u"V-V"
		},
		{
			u"<-<",
		},
		{
			u"^-^"
		},
		{
			u">->"
		},
		{
			u"==="
		}
	};

};

class Enemies : public DynamicObject
{
public:
	Enemies(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {
		MoveTo(2, 2);
	};

	void DrawObject() override;

	void MoveObject() override;

	void IsInVisArea(Player* player);

private:

	char16_t enemySprite[SPRITE_HEIGHT][SPRITE_WIDTH]
	{
		u"0-0"
	};

	const int VISIBLE_RADIUS = 30;

	vector <pair<int, int>> pathToGoal;
	vector <pair<int, int>> visibleArea;

	bool _algMove = false;

	void CheckNextStep();

	void RefreshVisibleArea();

	void MoveTo(int x, int y);

	void ChangeDirection() override;

};

class MoneyBag : public GameObject
{
public:

	MoneyBag(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {};

	void DrawObject() override;

	void Drop();

	void CheckFall();

	bool BagIsOpen();

	bool BagIsFalling();

	bool BagIsReady();

private:

	char16_t moneyBagSprite[SPRITE_HEIGHT][SPRITE_WIDTH]{
		u"($)"
	};

	char16_t moneyBagSpriteOpen[SPRITE_HEIGHT][SPRITE_WIDTH]{
		u"###"
	};


	bool _isOpen = false, _isFalling = false;

	int _fall = 0;
};

class Diamond : public GameObject 
{
public:
	Diamond(wd* wData, int x, int y, int speed, int color) : GameObject(wData, x, y, speed, color) {};
private:

	void DrawObject() override;

	char16_t spriteDiamond[SPRITE_HEIGHT][SPRITE_WIDTH]{
		u"(V)"
	};

};

class Bonus: public GameObject
{
public:
	Bonus(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {};

	void DrawObject() override;

private:

	char16_t spriteBonus[SPRITE_HEIGHT][SPRITE_WIDTH]{
		u"(+)"
	};

};

class Wall: public GameObject 
{
public:

	Wall(wd* wData, int x, int y, int speed, int color) :GameObject(wData, x, y, speed, color) {};

	void DrawObject();

private:

	char16_t wallSprite[WALL_HEIGHT][WALL_WIDTH]
	{
		u"####################",
		u"####################",
		u"####################",
		u"####################",
		u"####################",
		u"####################",
		u"####################",
		u"####################",
		u"####################",
		u"####################",
	};

};

class Bullet: public DynamicObject
{
public:

	Bullet(wd* wData, int x, int y, int speed, int color) :DynamicObject(wData, x, y, speed, color) {};

	void Shot(int direction);

	void MoveObject() override;

	void DrawObject() override;

	void EraseObject() override;

private:

	void ChangeDirection() override;
};