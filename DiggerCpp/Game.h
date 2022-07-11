#pragma once
#include "GameObject.h"

class Game
{
private:

    wd wData;

    char16_t prevBuf[ROWS][COLS];
    char coord[50];

    bool worldIsRun = true, win = false, gunReload = true, immortal = false;

    int score = 0;

    HINSTANCE hInstance;

    class VirtualTerminal {
    public:

        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        CONSOLE_SCREEN_BUFFER_INFO csbi;

        HWND hWindowConsole = GetForegroundWindow();

        bool Terminal() {

            // Set output mode to handle virtual terminal sequences
            if (hOut == INVALID_HANDLE_VALUE)
            {
                return false;
            }
            HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
            if (hIn == INVALID_HANDLE_VALUE)
            {
                return false;
            }

            DWORD dwOriginalOutMode = 0;
            DWORD dwOriginalInMode = 0;
            if (!GetConsoleMode(hOut, &dwOriginalOutMode))
            {
                return false;
            }
            if (!GetConsoleMode(hIn, &dwOriginalInMode))
            {
                return false;
            }

            DWORD dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
            DWORD dwRequestedInModes = ENABLE_VIRTUAL_TERMINAL_INPUT;

            DWORD dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
            if (!SetConsoleMode(hOut, dwOutMode))
            {
                // we failed to set both modes, try to step down mode gracefully.
                dwRequestedOutModes = ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                dwOutMode = dwOriginalOutMode | dwRequestedOutModes;
                if (!SetConsoleMode(hOut, dwOutMode))
                {
                    // Failed to set any VT mode, can't do anything here.
                    return -1;
                }
            }

            DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
            if (!SetConsoleMode(hIn, dwInMode))
            {
                // Failed to set VT input mode, can't do anything here.
                return -1;
            }

            return 0;
        }

        void SetScreenSize() {
            int Width = 120, Height = 56, err = 30;

            COORD monitorSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
            COORD bufferSize = { Width, Height };

            _SMALL_RECT Rect;
            Rect.Top = 0;
            Rect.Left = 0;
            Rect.Bottom = Height - 1;
            Rect.Right = Width - 1;

            SetConsoleScreenBufferSize(hOut, bufferSize);

            SetWindowPos(hWindowConsole, HWND_NOTOPMOST,
                (monitorSize.X / 2 - Width * 4 - err * 4), (monitorSize.Y / 2 - Height * 8 - err), 0, 0,
                SWP_NOZORDER | SWP_NOREPOSITION | SWP_NOREDRAW);

            SetConsoleWindowInfo(hOut, TRUE, &Rect);

            SetWindowLong(hWindowConsole, GWL_STYLE, GetWindowLong(hWindowConsole, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

            SetConsoleTitleA("Space Invaders C++");
        }

        void SetConsoleFont() {

            CONSOLE_FONT_INFOEX cfi;

            cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
            cfi.nFont = 6;
            cfi.dwFontSize.X = 30;
            cfi.dwFontSize.Y = 13;
            cfi.FontFamily = 54;
            cfi.FontWeight = 1000;

            wcscpy_s(cfi.FaceName, L"Lucida Console");
            SetCurrentConsoleFontEx(hOut, 0, &cfi);
        }
    };

    VirtualTerminal term; // console setting

    Enemies* enemy;
    Wall* wall;
    MoneyBag* moneyBag;
    Diamond* diamond;
    Bullet* bullet;
    Bonus* bonus;

    vector <GameObject*> allObjectList;
    vector <Enemies*> enemyList;
    vector <MoneyBag*> moneyBagList;
    vector <Diamond*> diamondList;
    vector <Bullet*> bulletList;
    vector <Bonus*> bonusList;


protected:

    void SetPos(int x, int y);

    void DrawArea();

    void CreateWorld();

    void DrawToMem();

    void HotKeys(bool& pause);

    void DrawEndInfo(bool& restart);

    void DrawInfo(Player* player);

    void DrawChanges();

    void SetWall(int x, int y);

    void SetDiamond(int x, int y);

    void SetMoneyBag(int x, int y);

    void SetBonus(int x, int y);

    void SpawnEnemy();

    void ReloadGun();

    void DrawLevel();

    void Collision(Player* player);

public:

    void RunWorld(bool& restart);

};

