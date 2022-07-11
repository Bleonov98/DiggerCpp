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
					printf(CSI "47;32m");
				}
				else if ((prevBuf[y][x] >> 8) == Blue) {
					printf(CSI "47;34m");
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
				else if ((prevBuf[y][x] >> 8) == BrRed) {
					printf(CSI "22;91m");
				}
				else if ((prevBuf[y][x] >> 8) == BrGreen) {
					printf(CSI "22;92m");
				}
				else if ((prevBuf[y][x] >> 8) == BrBlue) {
					printf(CSI "22;94m");
				}
				else if ((prevBuf[y][x] >> 8) == BrCyan) {
					printf(CSI "22;96m");
				}
				else if ((prevBuf[y][x] >> 8) == BrPurple) {
					printf(CSI "22;95m");
				}
				else if ((prevBuf[y][x] >> 8) == BrYellow) {
					printf(CSI "22;93m");
				}
				else if ((prevBuf[y][x] >> 8) == White) {
					printf(CSI "47;37m");
				}
				else if ((prevBuf[y][x] >> 8) == DropBag) {
					printf(CSI "22;32m");
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
	for (int i = 0; i < enemyList.size(); i++)
	{
		if (enemyList[i]->IsObjectDelete()) {
			enemyList.erase(enemyList.begin() + i);
		}
	}

	for (int i = 0; i < moneyBagList.size(); i++)
	{
		if (moneyBagList[i]->IsObjectDelete()) {
			moneyBagList.erase(moneyBagList.begin() + i);
		}
	}

	for (int i = 0; i < diamondList.size(); i++)
	{
		if (diamondList[i]->IsObjectDelete()) {
			diamondList.erase(diamondList.begin() + i);
		}
	}

	for (int i = 0; i < bonusList.size(); i++)
	{
		if (bonusList[i]->IsObjectDelete()) {
			bonusList.erase(bonusList.begin() + i);
		}
	}
	
	for (int i = 0; i < bulletList.size(); i++)
	{
		if (bulletList[i]->IsObjectDelete()) {
			bulletList.erase(bulletList.begin() + i);
		}
	}

	for (int i = 0; i < allObjectList.size(); i++)
	{
		if (allObjectList[i]->IsObjectDelete()) {
			wData.vBuf[allObjectList[i]->GetY()][allObjectList[i]->GetX()] = u' ';
			allObjectList.erase(allObjectList.begin() + i);
		}
	}

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
	diamond = new Diamond(&wData, x, y, 0, Blue);
	allObjectList.push_back(diamond);
	diamondList.push_back(diamond);
}

void Game::SetMoneyBag(int x, int y)
{
	moneyBag = new MoneyBag(&wData, x, y, 0, Green);
	allObjectList.push_back(moneyBag);
	moneyBagList.push_back(moneyBag);
}

void Game::SetBonus(int x, int y)
{
	bonus = new Bonus(&wData, x, y, 0, BrPurple);
	bonusList.push_back(bonus);
	allObjectList.push_back(bonus);
}

void Game::SpawnEnemy()
{
	enemy = new Enemies(&wData, COLS - 5, 2, 1, Red);
	allObjectList.push_back(enemy);
	enemyList.push_back(enemy);
}

void Game::ReloadGun()
{
	Sleep(10000);
	gunReload = true;
}

void Game::DrawLevel()
{
	// --------------------- WALL ---------------------
	
	SetWall(2, 3);
	SetWall(WALL_WIDTH + 4, 2);
	SetWall(WALL_WIDTH * 2 + 3, 2);
	SetWall(WALL_WIDTH * 3 + 6, 3);

	SetWall(2, WALL_HEIGHT + 2);
	SetWall(WALL_WIDTH + 4, WALL_HEIGHT + 2);
	SetWall(WALL_WIDTH * 2 + 7, WALL_HEIGHT + 4);
	SetWall(WALL_WIDTH * 3 + 6, WALL_HEIGHT + 2);

	SetWall(2, WALL_HEIGHT * 2 + 2);
	SetWall(WALL_WIDTH + 4, WALL_HEIGHT * 2 + 2);
	SetWall(WALL_WIDTH * 2 + 7, WALL_HEIGHT * 2 + 2);
	SetWall(WALL_WIDTH * 3 + 6, WALL_HEIGHT * 2 + 2);

	SetWall(2, WALL_HEIGHT * 3 + 2);
	SetWall(WALL_WIDTH + 4, WALL_HEIGHT * 3 + 2);
	SetWall(WALL_WIDTH * 2 + 7, WALL_HEIGHT * 3 + 2);
	SetWall(WALL_WIDTH * 3 + 6, WALL_HEIGHT * 3 + 2);

	SetWall(2, WALL_HEIGHT * 4 + 2);
	SetWall(WALL_WIDTH + 4, WALL_HEIGHT * 4 + 2);
	SetWall(WALL_WIDTH * 2 + 7, WALL_HEIGHT * 4 + 2);
	SetWall(WALL_WIDTH * 3 + 6, WALL_HEIGHT * 4 + 2);

	SetWall(2, WALL_HEIGHT * 5);
	SetWall(WALL_WIDTH * 2 + 7, WALL_HEIGHT * 5);
	SetWall(WALL_WIDTH * 3 + 6, WALL_HEIGHT * 5);

	// ---- DiamondBlock_1 -----

	SetDiamond(12, 15);
	SetDiamond(12 + SPRITE_WIDTH, 15);
	SetDiamond(12 - SPRITE_WIDTH, 15);

	SetDiamond(14, 17);
	SetDiamond(14 - SPRITE_WIDTH, 17);

	SetDiamond(12, 19);
	
	// ---- DiamondBlock_2 -----

	SetDiamond(52 + SPRITE_WIDTH, 25);
	SetDiamond(52 + SPRITE_WIDTH * 2, 25);
	SetDiamond(52 + SPRITE_WIDTH * 3, 25);

	SetDiamond(52 + SPRITE_WIDTH, 27);
	SetDiamond(52 + SPRITE_WIDTH * 2, 27);
	SetDiamond(52 + SPRITE_WIDTH * 3, 27);

	SetDiamond(52 + SPRITE_WIDTH, 29);
	SetDiamond(52 + SPRITE_WIDTH * 2, 29);
	SetDiamond(52 + SPRITE_WIDTH * 3, 29);

	// ---- DiamondBlock_3 -----

	SetDiamond(66, 39);
	SetDiamond(66 + SPRITE_WIDTH, 39);
	SetDiamond(66 + SPRITE_WIDTH * 2, 39);
	SetDiamond(66 + SPRITE_WIDTH * 3, 39);
	SetDiamond(66 + SPRITE_WIDTH * 4, 39);

	SetDiamond(66, 41);
	SetDiamond(66 + SPRITE_WIDTH * 4, 41);

	SetDiamond(66, 43);
	SetDiamond(66 + SPRITE_WIDTH, 43);
	SetDiamond(66 + SPRITE_WIDTH * 2, 43);
	SetDiamond(66 + SPRITE_WIDTH * 3, 43);
	SetDiamond(66 + SPRITE_WIDTH * 4, 43);

	// ---- DiamondBlock_4 -----

	SetDiamond(30, 5);
	SetDiamond(30 + SPRITE_WIDTH, 5);
	SetDiamond(30 + SPRITE_WIDTH * 2, 5);
	SetDiamond(30 + SPRITE_WIDTH * 3, 5);
	SetDiamond(30 + SPRITE_WIDTH * 4, 5);

	SetDiamond(30, 7);
	SetDiamond(30 + SPRITE_WIDTH, 7);
	SetDiamond(30 + SPRITE_WIDTH * 2, 7);
	SetDiamond(30 + SPRITE_WIDTH * 3, 7);
	SetDiamond(30 + SPRITE_WIDTH * 4, 7);

	// ---- DiamondBlock_5 -----

	SetDiamond(30, 25);
	SetDiamond(30 + SPRITE_WIDTH, 25);
	SetDiamond(30 + SPRITE_WIDTH * 2, 25);

	SetDiamond(30, 45);
	SetDiamond(30 + SPRITE_WIDTH, 45);

	// ---- DiamondBlock_6 -----

	SetDiamond(5, 34);
	SetDiamond(5 + SPRITE_WIDTH, 34);
	SetDiamond(5 + SPRITE_WIDTH * 2, 34);

	SetDiamond(5, 36);
	SetDiamond(5 + SPRITE_WIDTH, 36);
	SetDiamond(5 + SPRITE_WIDTH * 2, 36);

	SetDiamond(5, 38);
	SetDiamond(5 + SPRITE_WIDTH, 38);
	SetDiamond(5 + SPRITE_WIDTH * 2, 38);

	SetDiamond(5, 40);
	SetDiamond(5 + SPRITE_WIDTH, 40);
	SetDiamond(5 + SPRITE_WIDTH * 2, 40);

	SetDiamond(5, 42);
	SetDiamond(5 + SPRITE_WIDTH, 42);
	SetDiamond(5 + SPRITE_WIDTH * 2, 42);

	// ------------ MONEY BAG --------- 

	SetMoneyBag(15, 10);

	SetMoneyBag(28, 15);
	SetMoneyBag(28 + SPRITE_WIDTH, 15);
	SetMoneyBag(28 + SPRITE_WIDTH * 2, 15);

	SetMoneyBag(30, 30);
	SetMoneyBag(50, 30);

	SetMoneyBag(60, 10);

	SetMoneyBag(80, 15);
	SetMoneyBag(73, 15);
}

void Game::Collision(Player* player)
{
	// die settings
	// player die
	for (int size = 0; size < enemyList.size(); size++)
	{
		for (int i = 0; i < SPRITE_WIDTH - 1; i++)
		{
			for (int j = 0; j < SPRITE_WIDTH - 1; j++)
			{
				if ( (player->GetX() + i == enemyList[size]->GetX() + j) && (player->GetY() == enemyList[size]->GetY()) && (!immortal) ) {

					player->Death(worldIsRun);

					player->SetX(2);
					player->SetY(2);

					for (int enm = 0; enm < enemyList.size(); enm++)
					{
						enemyList[enm]->EraseObject();
						enemyList[enm]->SetX(COLS - 5);
						enemyList[enm]->SetY(2);
					}

					Sleep(200);

					return;
				}
				else if ((player->GetX() + i == enemyList[size]->GetX() + j) && (player->GetY() == enemyList[size]->GetY()) && (immortal)) {
					enemyList[size]->DeleteObject();
					enemyList[size]->EraseObject();

					score += 500;

					return;
				}
			}
		}
	}
	// enemy die
	for (int size = 0; size < enemyList.size(); size++)
	{
		for (int bullet = 0; bullet < bulletList.size(); bullet++)
		{
			for (int i = 0; i < SPRITE_WIDTH - 1; i++)
			{
				if ((enemyList[size]->GetX() + i == bulletList[bullet]->GetX() && enemyList[size]->GetY() == bulletList[bullet]->GetY()) || 
					(enemyList[size]->GetX() + i == bulletList[bullet]->GetX() && enemyList[size]->GetY() + 1 == bulletList[bullet]->GetY()) ||
					(enemyList[size]->GetX() + i == bulletList[bullet]->GetX() && enemyList[size]->GetY() - 1 == bulletList[bullet]->GetY()))  {

					enemyList[size]->DeleteObject();
					bulletList[bullet]->DeleteObject();
					enemyList[size]->EraseObject();
					bulletList[bullet]->EraseObject();

					score += 500;

					return;
				}
			}
		}
	}
	// ------------

	for (int size = 0; size < diamondList.size(); size++)
	{
		for (int i = 0; i < SPRITE_WIDTH - 1; i++)
		{
			for (int j = 0; j < SPRITE_WIDTH - 1; j++)
			{
				if (player->GetX() + i == diamondList[size]->GetX() + j && player->GetY() + i == diamondList[size]->GetY() + i) {
					PlaySound(MAKEINTRESOURCE(IDR_WAVE1), NULL, SND_RESOURCE | SND_ASYNC);

					score += 100;
					diamondList[size]->DeleteObject();

					return;
				}
			}
		}
	}

	// moneyBag collision settings
	// ---------------------------
	for (int size = 0; size < moneyBagList.size(); size++)
	{
		for (int i = 0; i < SPRITE_WIDTH - 1; i++)
		{
			for (int j = 0; j < SPRITE_WIDTH - 1; j++)
			{

				if ( moneyBagList[size]->BagIsOpen() && 
				   ((player->GetX() + i == moneyBagList[size]->GetX() + j) && (player->GetY() == moneyBagList[size]->GetY())) )
				{

					PlaySound(MAKEINTRESOURCE(IDR_WAVE5), NULL, SND_RESOURCE | SND_ASYNC);

					score += 1000;
					moneyBagList[size]->DeleteObject();

					return;
				}
			}
		}
	}

	for (int size = 0; size < moneyBagList.size(); size++)
	{
		if ((!moneyBagList[size]->BagIsOpen()) &&
			(player->GetX() + (SPRITE_WIDTH - 2) == moneyBagList[size]->GetX()) && (player->GetY() == moneyBagList[size]->GetY())
			&& (player->GetDirection() == 1)) {

			moneyBagList[size]->SetX(moneyBagList[size]->GetX() + 1);
		}
		else if ((!moneyBagList[size]->BagIsOpen()) &&
			(player->GetX() - 1 == moneyBagList[size]->GetX() + 1) && (player->GetY() == moneyBagList[size]->GetY())
			&& (player->GetDirection() == 3)) {

			moneyBagList[size]->SetX(moneyBagList[size]->GetX() - 1);
		}
		for (int i = 0; i < enemyList.size(); i++)
		{
			if ((!moneyBagList[size]->BagIsOpen()) &&
				(enemyList[i]->GetX() + (SPRITE_WIDTH - 2) == moneyBagList[size]->GetX()) && (enemyList[i]->GetY() == moneyBagList[size]->GetY())
				&& (enemyList[i]->GetDirection() == 1)) {

				moneyBagList[size]->SetX(moneyBagList[size]->GetX() + 1);
			}
			else if ((!moneyBagList[size]->BagIsOpen()) &&
				(enemyList[i]->GetX() - 1 == moneyBagList[size]->GetX() + 1) && (enemyList[i]->GetY() == moneyBagList[size]->GetY())
				&& (enemyList[i]->GetDirection() == 3)) {

				moneyBagList[size]->SetX(moneyBagList[size]->GetX() - 1);
			}
		}
		
	}

	for (int i = 0; i < moneyBagList.size(); i++)
	{
		for (int j = 0; j < moneyBagList.size(); j++)
		{
			if (moneyBagList[i]->GetX() + (SPRITE_WIDTH - 2) == moneyBagList[j]->GetX() && moneyBagList[i]->GetY() == moneyBagList[j]->GetY() 
				&& (player->GetDirection() == RIGHT)) {
				moneyBagList[j]->SetX(moneyBagList[j]->GetX() + 1);
			}
			if (moneyBagList[i]->GetX() == moneyBagList[j]->GetX() + (SPRITE_WIDTH - 2) && moneyBagList[i]->GetY() == moneyBagList[j]->GetY()
				&& (player->GetDirection() == LEFT)) {
				moneyBagList[j]->SetX(moneyBagList[j]->GetX() - 1);
			}
		}
	}

	for (int size = 0; size < moneyBagList.size(); size++)
	{
		for (int i = 0; i < SPRITE_WIDTH - 1; i++)
		{
			for (int j = 0; j < SPRITE_WIDTH - 1; j++)
			{
				if ((moneyBagList[size]->BagIsFalling()) && (!moneyBagList[size]->BagIsOpen()) &&
					( (moneyBagList[size]->GetX() + i == player->GetX() + j && moneyBagList[size]->GetY() + 1 == player->GetY() ) ||
					  (moneyBagList[size]->GetX() + i == player->GetX() + j && moneyBagList[size]->GetY() == player->GetY()) ) )
				{
					player->UnderMoneyBag();
					player->EraseObject();
					player->SetY(player->GetY() + 1);
					

					for (int width = 0; width < SPRITE_WIDTH - 1; width++)
					{
						if (wData.vBuf[player->GetY() + 1][player->GetX() + width] == (u'#' | (WALL_COLOR << 8))
							|| player->GetY() + 1 == ROWS) {
							player->Death(worldIsRun);

							player->SetX(2);
							player->SetY(2);

							return;

						}
					}
				}
			}
		}
	}
	// death for player (BagDeath) -^

	for (int size = 0; size < moneyBagList.size(); size++)
	{
		bool died = false;
		for (int enemy = 0; enemy < enemyList.size();  enemy++)
		{
			for (int i = 0; i < SPRITE_WIDTH - 1; i++)
			{
				for (int j = 0; j < SPRITE_WIDTH - 1; j++)
				{
					if ((moneyBagList[size]->BagIsFalling()) && (!moneyBagList[size]->BagIsOpen()) &&
						((moneyBagList[size]->GetX() + i == enemyList[enemy]->GetX() + j && moneyBagList[size]->GetY() + 1 == enemyList[enemy]->GetY()) ||
						(moneyBagList[size]->GetX() + i == enemyList[enemy]->GetX() + j && moneyBagList[size]->GetY() == enemyList[enemy]->GetY())))
					{
						enemyList[enemy]->UnderMoneyBag();
						enemyList[enemy]->EraseObject();
						enemyList[enemy]->SetY(moneyBagList[size]->GetY() + 1);


						for (int width = 0; width < SPRITE_WIDTH - 1; width++)
						{
							if (wData.vBuf[enemyList[enemy]->GetY() + 1][enemyList[enemy]->GetX() + width] == (u'#' | (WALL_COLOR << 8)) ||
								enemyList[enemy]->GetY() + 1 == ROWS) {
								enemyList[enemy]->DeleteObject();

								enemyList[enemy]->EraseObject();

								score += 500;

								died = true;

								if (died) break;
							}
						}
					}
					if (died) break;
				}
				if (died) break;
			}
		}
		
	}
	// death for enemy (BagDeath)
	// 
	// --------------------------

	for (int size = 0; size < bonusList.size(); size++)
	{
		for (int i = 0; i < SPRITE_HEIGHT; i++)
		{
			for (int j = 0; j < SPRITE_WIDTH - 1; j++)
			{

				if ( (player->GetX() + j == bonusList[size]->GetX() && player->GetY() + i == bonusList[size]->GetY() + i) ||
					 (player->GetX() + j == bonusList[size]->GetX() + (SPRITE_WIDTH - 1) && player->GetY() + i == bonusList[size]->GetY() + i) ) {

					PlaySound(MAKEINTRESOURCE(IDR_WAVE2), NULL, SND_RESOURCE | SND_ASYNC);

					score += 500;
					bonusList[size]->DeleteObject();

					thread immortalGo([&] {
						immortal = true;
						player->Immortal(immortal);
					});
					immortalGo.detach();

					return;
				}

			}
		}
	}
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

	Player* player = new Player(&wData, 2, 2, 1, Cyan);
	allObjectList.push_back(player);

	int tick = 0;
	int enemyCnt = 0;

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

		if (!enemyList.empty()) {
			for (int i = 0; i < enemyList.size(); i++)
			{
				enemyList[i]->MoveObject();
				enemyList[i]->IsInVisArea(player);
			}
		}
		if (tick % 50 == 0 && enemyList.size() < 3 && enemyCnt <= 10) {
			SpawnEnemy();
			enemyCnt++;
			if (enemyCnt == 6) {
				SetBonus(COLS - SPRITE_WIDTH, 2);
			}
		}

		for (int i = 0; i < moneyBagList.size(); i++)
		{
			moneyBagList[i]->Drop();
		}

		if (bulletList.empty() && gunReload) {
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				bullet = new Bullet(&wData, player->GetX() + 1, player->GetY(), 1, Red);
				bullet->Shot(player->GetBulletDirection());
				bulletList.push_back(bullet);
				allObjectList.push_back(bullet);
				
				gunReload = false;
				thread reloadMyGun([&] {
					ReloadGun();
					});
				reloadMyGun.detach();

				PlaySound(MAKEINTRESOURCE(IDR_WAVE3), NULL, SND_RESOURCE | SND_ASYNC);
			}
		}

		for (int i = 0; i < bulletList.size(); i++)
		{
			bulletList[i]->MoveObject();
		}

		DrawToMem();

		DrawChanges();

		Collision(player);

		DrawInfo(player);

		Sleep(60);

		tick++;

		if (diamondList.empty() && moneyBagList.empty() && enemyList.empty()) {
			win = true;
			worldIsRun = false;
		}
	}

	DrawEndInfo(restart);

	hotKeys.join();

	printf(CSI "?1049l");
}