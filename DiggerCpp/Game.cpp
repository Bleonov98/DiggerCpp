#include "Game.h"

void Game::SetPos(int x, int y)
{
	sprintf_s(coord, "%s%d;%dH", CSI, y, x);
	printf(coord);
}

void Game::HotKeys(bool& pause)
{
	while (worldIsRun) {
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			worldIsRun = false;
		}
		else if (GetAsyncKeyState(0x50) & 0x8000) {
			pause = !pause;

			this_thread::sleep_for(milliseconds(1000));
		}
	}
}

void Game::DrawArea()
{
	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	HRSRC hResource = FindResource(hInstance, MAKEINTRESOURCE(IDR_TEXT1), L"TEXT");

	if (hResource)
	{
		HGLOBAL hLoadedResource = LoadResource(hInstance, hResource);

		if (hLoadedResource)
		{
			LPCSTR area = (LPCSTR)LockResource(hLoadedResource);

			if (area)
			{
				DWORD dwResourceSize = SizeofResource(hInstance, hResource);

				if (0 != dwResourceSize)
				{
					for (int i = 0; i < strnlen(area, 9000); i++) {
						cout << area[i];
					}
				}
			}
		}
	}

	setvbuf(stdout, NULL, _IONBF, 0);
}

void Game::CreateWorld() {

	term.Terminal();  // Set virtual terminal settings
	term.SetScreenSize();
	term.SetConsoleFont();

	printf(CSI "?1049h"); // enable alt buffer
	printf(CSI "?25l"); // hide cursor blinking

	DrawArea();
	DrawLevel();
}

void Game::DrawEndInfo(bool& restart)
{
	if (win) {
		SetPos(COLS + 4, 20);
		cout << "CONGRATULATION! YOU WIN!";
	}
	else {
		SetPos(COLS + 11, 20);
		cout << "GAME OVER!";
	}

	SetPos(COLS + 5, 22);
	cout << "PRESS ENTER TO RESTART";
	SetPos(COLS + 7, 23);
	cout << "PRESS ESC TO EXIT";

	bool pressed = false;
	restart = false;

	while (!pressed) {
		if (GetAsyncKeyState(VK_RETURN)) {
			restart = true;
			pressed = true;
		}
		else if (GetAsyncKeyState(VK_ESCAPE)) {
			restart = false;
			pressed = true;
		}
	}
}

void Game::DrawInfo(Player* player)
{
	SetPos(COLS + 10, 2);
	cout << score;
	SetPos(COLS + 10, 4);
	cout << player->GetLifes();
}

void Game::DrawChanges()
{
	for (int y = 0; y < ROWS; y++)
	{
		for (int x = 0; x < COLS; x++)
		{
			if (prevBuf[y][x] == wData.vBuf[y][x]) {
				continue;
			}
			else {
				prevBuf[y][x] = wData.vBuf[y][x];

				SetPos(x, y);

				if ((prevBuf[y][x] >> 8) == Red) {
					printf(CSI "22;31m");
				}
				else if ((prevBuf[y][x] >> 8) == Green) {
					printf(CSI "22;32m");
				}
				else if ((prevBuf[y][x] >> 8) == Blue) {
					printf(CSI "22;34m");
				}
				else if ((prevBuf[y][x] >> 8) == Yellow) {
					printf(CSI "22;33m");
				}
				else if ((prevBuf[y][x] >> 8) == Purple) {
					printf(CSI "22;35m");
				}
				else if ((prevBuf[y][x] >> 8) == Cyan) {
					printf(CSI "22;36m");
				}
				else if ((prevBuf[y][x] >> 8) == White) {
					printf(CSI "47;37m");
				}
				else printf(CSI "22; 44m");

				cout << char(prevBuf[y][x]);

				printf(CSI "1;0m");
			}
		}
	}
}

void Game::DrawToMem()
{
	for (int i = 0; i < allObjectList.size(); i++)
	{
		allObjectList[i]->DrawObject();
	}
}

void Game::SetWall(int x, int y)
{
	wall = new Wall(&wData, x, y, 0, White);
	wall->DrawObject();
}

void Game::SetDiamond(int x, int y)
{
	diamond = new Diamond(&wData, x, y, 0, Purple);
	allObjectList.push_back(diamond);
	diamondList.push_back(diamond);
}

void Game::SetMoneyBag(int x, int y)
{
	moneyBag = new MoneyBag(&wData, x, y, 0, Green);
	allObjectList.push_back(moneyBag);
	moneyBagList.push_back(moneyBag);
}

void Game::SpawnEnemy()
{
	enemy = new Enemies(&wData, COLS - 5, 2, 1, Red);
	allObjectList.push_back(enemy);
	enemyList.push_back(enemy);
}

void Game::DrawLevel()
{
	SetWall(7, 2);
	SetWall(WALL_WIDTH + 5, 2);
	SetWall(WALL_WIDTH * 2, 2);
	SetWall(WALL_WIDTH * 3, 2);

	SetWall(7, WALL_HEIGHT + 3);
	SetWall(WALL_WIDTH, WALL_HEIGHT + 3);
	SetWall(WALL_WIDTH * 2, WALL_HEIGHT + 3);
	SetWall(WALL_WIDTH * 3, WALL_HEIGHT + 3);

	SetWall(7, WALL_HEIGHT * 2 + 3);
	SetWall(WALL_WIDTH + 5, WALL_HEIGHT * 2 + 3);
	SetWall(WALL_WIDTH * 2, WALL_HEIGHT * 2 + 3);
	SetWall(WALL_WIDTH * 3, WALL_HEIGHT * 2 + 3);

	SetWall(7, WALL_HEIGHT * 3 + 3);
	SetWall(WALL_WIDTH + 5, WALL_HEIGHT * 3 + 3);
	SetWall(WALL_WIDTH * 2, WALL_HEIGHT * 3 + 3);
	SetWall(WALL_WIDTH * 3, WALL_HEIGHT * 3 + 3);

	SetWall(7, WALL_HEIGHT * 4);
	SetWall(WALL_WIDTH + SPRITE_WIDTH, WALL_HEIGHT * 4);
	SetWall(WALL_WIDTH * 2 + SPRITE_WIDTH, WALL_HEIGHT * 4);
	SetWall(WALL_WIDTH * 3, WALL_HEIGHT * 4);

	// ---- DiamondBlock_1 -----
	SetDiamond(12, 15);
	SetDiamond(12 + SPRITE_WIDTH, 15);
	SetDiamond(12 + SPRITE_WIDTH * 2, 15);

	SetDiamond(10 + SPRITE_WIDTH, 17);
	SetDiamond(10 + SPRITE_WIDTH * 2, 17);

	SetDiamond(12 + SPRITE_WIDTH, 19);
	
	// ---- DiamondBlock_2 -----
	SetDiamond(52, 25);
	SetDiamond(52 + SPRITE_WIDTH, 25);
	SetDiamond(52 + SPRITE_WIDTH * 2, 25);

	SetDiamond(50 + SPRITE_WIDTH, 27);
	SetDiamond(50 + SPRITE_WIDTH * 2, 27);

	SetDiamond(52 + SPRITE_WIDTH, 29);

	// ---- DiamondBlock_3 -----
	SetDiamond(32, 35);
	SetDiamond(32 + SPRITE_WIDTH, 35);
	SetDiamond(32 + SPRITE_WIDTH * 2, 35);

	SetDiamond(30 + SPRITE_WIDTH, 37);
	SetDiamond(30 + SPRITE_WIDTH * 2, 37);

	SetDiamond(32 + SPRITE_WIDTH, 39);
	// -------------------------

	SetMoneyBag(15, 10);

	SetMoneyBag(28, 15);
	SetMoneyBag(28 + SPRITE_WIDTH, 15);
	SetMoneyBag(28 + SPRITE_WIDTH * 2, 15);
	
	

}

void Game::Collision()
{
}

void Game::RunWorld(bool& restart)
{
	srand(time(NULL));
	CreateWorld();

	bool pause = false;
	score = 0;

	thread hotKeys([&]
		{ HotKeys(pause); }
	);

	Player* player = new Player(&wData, 2, 2, 1, Yellow);
	allObjectList.push_back(player);

	SpawnEnemy();

	while (worldIsRun) {

		if (pause) {

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "PAUSED";

			while (pause) {

			}

			SetPos(COLS / 2 - 2, ROWS / 2);
			cout << "      ";

		}

		player->MoveObject();

		for (int i = 0; i < enemyList.size(); i++)
		{
			enemyList[i]->MoveObject();
			enemyList[i]->IsInVisArea(player);
		}

		for (int i = 0; i < moneyBagList.size(); i++)
		{
			moneyBagList[i]->Drop();
		}

		DrawToMem();

		DrawChanges();

		DrawInfo(player);

		Sleep(60);
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}