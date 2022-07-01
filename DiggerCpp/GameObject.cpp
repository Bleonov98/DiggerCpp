#include "GameObject.h"

int GameObject::GetX()
{
    return _x;
}

int GameObject::GetY()
{
    return _y;
}

void GameObject::SetX(int x)
{
    _x = x;
}

void GameObject::SetY(int y)
{
    _y = y;
}

void GameObject::EraseObject()
{
    wData->vBuf[_y][_x] = u' ';
}

bool GameObject::IsObjectDelete()
{
    return _deleteObject;
}

void GameObject::DeleteObject()
{
    _deleteObject = true;
}


// -------------------- Enemy --------------- 

void Enemies::CheckNextStep()
{
    if (
        (_direction == UP) && (wData->vBuf[_y - 1][_x] == (u'#' | (WALL_COLOR << 8)) || _y - 1 == 1)
        ) {
        _direction = STOP;
    }
    else if (
        (_direction == RIGHT) && (wData->vBuf[_y][_x + 1] == (u'#' | (WALL_COLOR << 8)) || _x + 1 == COLS - 1)
        ) {
        _direction = STOP;
    }
    else if (_direction == DOWN && (wData->vBuf[_y + 1][_x] == (u'#' | (WALL_COLOR << 8)) || _y + 1 == ROWS)) {
        _direction = STOP;
    }
    else if (_direction == LEFT && (wData->vBuf[_y][_x - 1] == (u'#' | (WALL_COLOR << 8)) || _x - 1 == 1)) {
        _direction = STOP;
    }
}

void Enemies::MoveTo(int x, int y)
{
    pair<int, int> targetPos;
    targetPos.first = x;
    targetPos.second = y;

    if (targetPos.first >= COLS || targetPos.second >= ROWS || targetPos.first <= 1 || targetPos.second <= 1) {
        return;
    }

    if (targetPos.first == _x && targetPos.second == _y) {
        return;
    }

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (wData->vBuf[i][j] == (u'#' | (WALL_COLOR << 8)) || i <= 1 || j <= 1 || j >= COLS - 1 || i >= ROWS) {
                wData->grid[i][j] = -99;
            }
            else wData->grid[i][j] = -1;
        }
    }
    // erase previous grid state

    pathToGoal.clear();

    pair<int, int> startPos = make_pair(GetX(), GetY());

    int d = 0; // steps from start to goal

    bool stop = false;

    vector <pair<int, int>> wave;
    vector <pair<int, int>> prevWave;

    wData->grid[startPos.second][startPos.first] = d;
    prevWave.push_back(make_pair(startPos.first, startPos.second));

    while (!stop) {

        d++;

        for (int i = 0; i < prevWave.size(); i++)
        {

            if (wData->grid[prevWave[i].second][prevWave[i].first + 1] == -1) {
                wData->grid[prevWave[i].second][prevWave[i].first + 1] = d;
                wave.push_back(make_pair(prevWave[i].first + 1, prevWave[i].second));
            }
            if (wData->grid[prevWave[i].second][prevWave[i].first - 1] == -1) {
                wData->grid[prevWave[i].second][prevWave[i].first - 1] = d;
                wave.push_back(make_pair(prevWave[i].first - 1, prevWave[i].second));
            }
            if (wData->grid[prevWave[i].second + 1][prevWave[i].first] == -1) {
                wData->grid[prevWave[i].second + 1][prevWave[i].first] = d;
                wave.push_back(make_pair(prevWave[i].first, prevWave[i].second + 1));
            }
            if (wData->grid[prevWave[i].second - 1][prevWave[i].first] == -1) {
                wData->grid[prevWave[i].second - 1][prevWave[i].first] = d;
                wave.push_back(make_pair(prevWave[i].first, prevWave[i].second - 1));
            }
        }

        for (int i = 0; i < wave.size(); i++)
        {
            if (wave[i].first == targetPos.first && wave[i].second == targetPos.second) {
                stop = true;
                break;
            }
        }

        prevWave.clear();
        prevWave.swap(wave);
    }

    pair<int, int> curPos = make_pair(targetPos.first, targetPos.second);
    pathToGoal.push_back(make_pair(targetPos.first, targetPos.second));

    while (d > 0) {
        if (wData->grid[curPos.second][curPos.first + 1] == d) {
            pathToGoal.push_back(make_pair(curPos.first + 1, curPos.second));
            curPos.first++;
        }
        else if (wData->grid[curPos.second][curPos.first - 1] == d) {
            pathToGoal.push_back(make_pair(curPos.first - 1, curPos.second));
            curPos.first--;
        }
        else if (wData->grid[curPos.second + 1][curPos.first] == d) {
            pathToGoal.push_back(make_pair(curPos.first, curPos.second + 1));
            curPos.second++;
        }
        else if (wData->grid[curPos.second - 1][curPos.first] == d) {
            pathToGoal.push_back(make_pair(curPos.first, curPos.second - 1));
            curPos.second--;
        }

        d--;
    }

    _algMove = true;
}

void Enemies::DrawObject()
{
    wData->vBuf[_y][_x] = u'@' | (_color << 8);
}

void Enemies::ChangeDirection() 
{
    if (_direction == STOP) {
        _direction = rand() % 4;
    }
}

void Enemies::MoveObject()
{
    EraseObject();

    if (pathToGoal.empty()) {
        _algMove = false;
    }

    if (!_algMove) {

        ChangeDirection();

        CheckNextStep();

        if (_direction == UP) {
            SetY(_y -= _speed);
        }
        else if (_direction == RIGHT) {
            SetX(_x += _speed);
        }
        else if (_direction == DOWN) {
            SetY(_y += _speed);
        }
        else if (_direction == LEFT) {
            SetX(_x -= _speed);
        }
    }
    else if (_algMove) {
        if (!pathToGoal.empty()) {
            SetX(pathToGoal.back().first);
            SetY(pathToGoal.back().second);

            pathToGoal.pop_back();
        }
    }
}

// -------------------- Player --------------- 

void Player::ChangeDirection()
{
    if (GetAsyncKeyState(VK_UP) && (_y - 1 != 1)) {
        _direction = UP;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && (_x + 1 != COLS - 1)) {
        _direction = RIGHT;
    }
    else if (GetAsyncKeyState(VK_DOWN) && (_y + 1 != ROWS)) {
        _direction = DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT) && (_x - 1 != 1)) {
        _direction = LEFT;
    }
}

void Player::Death(bool& worldIsRun)
{
    EraseObject();

    if (lifes == 1) {
        worldIsRun = false;
    }
    else lifes--;
}

int Player::GetLifes()
{
    return lifes;
}

void Player::MoveObject()
{
    _direction = STOP;

    ChangeDirection();

    if (_direction != STOP) {

        EraseObject();

        if (_direction == UP) {
            _y -= _speed;
        }
        else if (_direction == RIGHT) {
            _x += _speed;
        }
        else if (_direction == DOWN) {
            _y += _speed;
        }
        else if (_direction == LEFT) {
            _x -= _speed;
        }

        if (_playerAnimation == 0) {
            _playerAnimation++;
        }
        else _playerAnimation = 0;

    }
}

void Player::DrawObject()
{
    wData->vBuf[_y][_x] = sprite[_direction][0][0] | (_color << 8);
}