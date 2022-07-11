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
    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        for (int j = 0; j < SPRITE_WIDTH - 1; j++)
        {
            wData->vBuf[_y + i][_x + j] = u' ';
        }
    }
}

bool GameObject::IsObjectDelete()
{
    return _deleteObject;
}

void GameObject::DeleteObject()
{
    EraseObject();
    _deleteObject = true;
}

// ------------------------------------------

int DynamicObject::GetDirection()
{
    return _direction;
}

void DynamicObject::UnderMoneyBag()
{
    _isFalling = true;
}

void DynamicObject::BagCollision()
{
    for (int i = 0; i < SPRITE_WIDTH - 1; i++)
    {
        if ((_direction == UP && wData->vBuf[_y - 1][_x + i] == (u'(' | (Green << 8))) ||
            (_direction == UP && wData->vBuf[_y - 1][_x + i] == (u'(' | (DropBag << 8)))) {
            _direction = STOP;
        }
        else if ((_direction == UP && wData->vBuf[_y - 1][_x + i] == (u'$' | (Green << 8))) ||
                 (_direction == UP && wData->vBuf[_y - 1][_x + i] == (u'$' | (DropBag << 8))) ) {
            _direction = STOP;
        }
        else if ((_direction == UP && wData->vBuf[_y - 1][_x + i] == (u')' | (Green << 8))) ||
            (_direction == UP && wData->vBuf[_y - 1][_x + i] == (u')' | (DropBag << 8)))) {
            _direction = STOP;
        }

        if ((_direction == DOWN && wData->vBuf[_y + 1][_x + i] == (u'(' | (Green << 8))) ||
            (_direction == DOWN && wData->vBuf[_y + 1][_x + i] == (u'(' | (DropBag << 8)))) {
            _direction = STOP;
        }
        else if ((_direction == DOWN && wData->vBuf[_y + 1][_x + i] == (u'$' | (Green << 8))) ||
            (_direction == DOWN && wData->vBuf[_y + 1][_x + i] == (u'$' | (DropBag << 8)))) {
            _direction = STOP;
        }
        else if ((_direction == DOWN && wData->vBuf[_y + 1][_x + i] == (u')' | (Green << 8)) ) ||
            (_direction == DOWN && wData->vBuf[_y + 1][_x + i] == (u')' | (DropBag << 8))) ) {
            _direction = STOP;
        }
    }
}

// -------------------- Enemy --------------- 

void Enemies::CheckNextStep()
{
    
    if (_direction == UP){
        for (int i = 0; i < SPRITE_HEIGHT; i++)
        {
            for (int j = 0; j < SPRITE_WIDTH - 1; j++)
            {
                if (wData->vBuf[_y - 1][_x + j] == (u'#' | (WALL_COLOR << 8)) || _y - 1 == 1) {
                    _direction = STOP;
                    return;
                }
            }
        }
    }
    else if (_direction == RIGHT) {
        for (int i = 0; i < SPRITE_HEIGHT; i++)
        {
            for (int j = 0; j < SPRITE_WIDTH; j++)
            {
                if (wData->vBuf[_y][_x + j] == (u'#' | (WALL_COLOR << 8)) || _x + SPRITE_WIDTH == COLS) {
                    _direction = STOP;
                    return;
                }
            }
        }
    }
    else if (_direction == DOWN) {
        for (int i = 0; i < SPRITE_HEIGHT; i++)
        {
            for (int j = 0; j < SPRITE_WIDTH - 1; j++)
            {
                if (wData->vBuf[_y + 1][_x + j] == (u'#' | (WALL_COLOR << 8)) || _y + 1 == ROWS) {
                    _direction = STOP;
                    return;
                }
            }
        }
    }
    else if (_direction == LEFT) {
        for (int i = 0; i < SPRITE_HEIGHT; i++)
        {
            for (int j = 0; j < SPRITE_WIDTH - 1; j++)
            {
                if (wData->vBuf[_y][_x - 1] == (u'#' | (WALL_COLOR << 8)) || _x - 1 == 1) {
                    _direction = STOP;
                    return;
                }
            }
        }
    }

    BagCollision();
}

void Enemies::MoveTo(int x, int y)
{
    pathToGoal.clear();
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
            else if (wData->vBuf[i][j] == (u'(' | (Green << 8)) || wData->vBuf[i][j] == (u')' | (Green << 8)) || wData->vBuf[i][j] == (u'$' | (Green << 8))) {
                wData->grid[i][j] = -99;
            }
            else if (wData->vBuf[i][j] == (u'(' | (DropBag << 8)) || wData->vBuf[i][j] == (u')' | (DropBag << 8)) || wData->vBuf[i][j] == (u'$' | (DropBag << 8))) {
                wData->grid[i][j] = -99;
            }
            else wData->grid[i][j] = -1;
        }
    }
    // erase previous grid state

    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        for (int j = 0; j < SPRITE_WIDTH - 1; j++)
        {
            if (wData->grid[y][x + j] == -99) {
                return;
            }
            if (wData->grid[_y][_x + j] == -99) {
                return;
            }
        }
    }
    
    pair<int, int> startPos = make_pair(GetX(), GetY());

    int d = 0; // steps from start to goal

    bool stop = false;

    vector <pair<int, int>> wave;
    vector <pair<int, int>> prevWave;

    wData->grid[startPos.second][startPos.first] = d;
    prevWave.push_back(make_pair(startPos.first, startPos.second));

    while (!stop) {

        d++;

        if (d >= (ROWS - 2) * (COLS - 2)) {
            return;
        }

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

    for (int size = 0; size < pathToGoal.size(); size++)
    {
        for (int i = 0; i < SPRITE_HEIGHT; i++)
        {
            for (int j = 0; j < SPRITE_WIDTH - 1; j++)
            {
                if (wData->grid[pathToGoal[size].second + i][pathToGoal[size].first + j] == -99) {
                    pathToGoal.clear();
                    _algMove = false;
                    return;
                }
            }
        }
    }
    
}

void Enemies::DrawObject()
{
    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        for (int j = 0; j < SPRITE_WIDTH - 1; j++)
        {
            wData->vBuf[_y + i][_x + j] = enemySprite[i][j] | (_color << 8);
        }
    }
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

    ChangeDirection();

    CheckNextStep();

    if (pathToGoal.empty()) {
        _algMove = false;
    }

    if (!_algMove && !_isFalling) {

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
    else if (_algMove && !_isFalling) {
        if (!pathToGoal.empty()) {
            SetX(pathToGoal.back().first);
            SetY(pathToGoal.back().second);

            pathToGoal.pop_back();
        }
    }

    RefreshVisibleArea();
}

void Enemies::IsInVisArea(Player* player)
{
    if (_algMove == false) {
        for (int i = 0; i < visibleArea.size(); i++)
        {
            if ((player->GetX() == visibleArea[i].first) && (player->GetY() == visibleArea[i].second)) {
                _algMove = true;
                MoveTo(player->GetX(), player->GetY());
                break;
            }
        }
    }
}

void Enemies::RefreshVisibleArea()
{
    visibleArea.clear();

    for (int R = 1; R < VISIBLE_RADIUS; R++)
    {
        int x = 0;
        int y = R;

        int delta = 1 - 2 * R;
        int err = 0;

        while (y >= x) {
            visibleArea.push_back(make_pair(_x + x, _y + y));
            visibleArea.push_back(make_pair(_x + x, _y - y));
            visibleArea.push_back(make_pair(_x - x, _y + y));
            visibleArea.push_back(make_pair(_x - x, _y - y));
            visibleArea.push_back(make_pair(_x + y, _y + x));
            visibleArea.push_back(make_pair(_x + y, _y - x));
            visibleArea.push_back(make_pair(_x - y, _y + x));
            visibleArea.push_back(make_pair(_x - y, _y - x));

            err = 2 * (delta + y) - 1;

            if ((delta < 0) && (err <= 0)) {
                delta += 2 * ++x + 1;
                continue;
            }
            if ((delta > 0) && (err > 0)) {
                delta -= 2 * --y + 1;
                continue;
            }

            delta += 2 * (++x - --y);
        }
    }
}


// -------------------- Player --------------- 

void Player::ChangeDirection()
{
    if (GetAsyncKeyState(VK_UP) && (_y - 1 != 1)) {
        _direction = UP;
    }
    else if (GetAsyncKeyState(VK_RIGHT) && (_x + SPRITE_WIDTH != COLS)) {
        _direction = RIGHT;
    }
    else if (GetAsyncKeyState(VK_DOWN) && (_y + 1 != ROWS)) {
        _direction = DOWN;
    }
    else if (GetAsyncKeyState(VK_LEFT) && (_x - 1 != 1)) {
        _direction = LEFT;
    }

    BagCollision();
}

void Player::Death(bool& worldIsRun)
{
    Sleep(1000);

    EraseObject();

    if (lifes == 1) {
        worldIsRun = false;
    }
    else lifes--;

    _isFalling = false;
}

void Player::MoveObject()
{
    _direction = STOP;

    ChangeDirection();

    if (_direction != STOP && !_isFalling) {

        EraseObject();

        if (_direction == UP) {
            _y -= _speed;
            _lastDir = UP;
        }
        else if (_direction == RIGHT) {
            _x += _speed;
            _lastDir = RIGHT;
        }
        else if (_direction == DOWN) {
            _y += _speed;
            _lastDir = DOWN;
        }
        else if (_direction == LEFT) {
            _x -= _speed;
            _lastDir = LEFT;
        }

        if (_playerAnimation == 0) {
            _playerAnimation++;
        }
        else _playerAnimation = 0;

    }
}

void Player::Immortal(bool immortal)
{
    Sleep(8000);

    immortal = false;
}

void Player::DrawObject()
{
    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        for (int j = 0; j < SPRITE_WIDTH - 1; j++)
        {
            wData->vBuf[_y + i][_x + j] = playerSprite[_direction][i][j] | (_color << 8);
        }
    }
}

int Player::GetBulletDirection()
{
    return _lastDir;
}

int Player::GetLifes()
{
    return lifes;
}

// -------------------- Bullet ---------------

void Bullet::Shot(int direction)
{
    _direction = direction;
}

void Bullet::MoveObject()
{
    EraseObject();

    ChangeDirection();

    if (!_deleteObject) {
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
}

void Bullet::DrawObject()
{
    wData->vBuf[_y][_x] = u'*' | (_color << 8);
}

void Bullet::EraseObject()
{
    wData->vBuf[_y][_x] = u' ';
}

void Bullet::ChangeDirection()
{
    if ( (_direction == UP && wData->vBuf[_y - 1][_x] == (u'#' | (WALL_COLOR << 8))) || _y - 1 == 1) DeleteObject();

    else if ((_direction == RIGHT && wData->vBuf[_y][_x + 1] == (u'#' | (WALL_COLOR << 8))) || _x + 1 == COLS - 1) DeleteObject();

    else if ((_direction == DOWN && wData->vBuf[_y + 1][_x] == (u'#' | (WALL_COLOR << 8))) || _y + 1 == ROWS) DeleteObject();

    else if ((_direction == LEFT && wData->vBuf[_y][_x - 1] == (u'#' | (WALL_COLOR << 8))) || _x - 1 == 1) DeleteObject();
        
    else if (_direction == STOP) DeleteObject();
}


// -------------------- Bonuses --------------

void MoneyBag::Drop()
{
    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        for (int j = 0; j < SPRITE_WIDTH - 1; j++)
        {
            if (wData->vBuf[_y + 1][_x + j] == (u'#' | (WALL_COLOR << 8)) || _y == ROWS - 1) {
                return;
            }
        }
    }

    _isFalling = true;
    _color = DropBag;

    EraseObject();
    _y++;
    _fall++;

    for (int i = 0; i < SPRITE_WIDTH - 1; i++)
    {
        if (wData->vBuf[_y + 1][_x + i] == (u'#' | (WALL_COLOR << 8)) || _y + 1 == ROWS) {

            _isFalling = false;

            if (_fall >= 2) {
                _isOpen = true;
                PlaySound(MAKEINTRESOURCE(IDR_WAVE4), NULL, SND_RESOURCE | SND_ASYNC);
            }
            else if (_fall <= 1) {
                _fall = 0;
            }
        }
    }
}

bool MoneyBag::BagIsOpen()
{
    return _isOpen;
}

bool MoneyBag::BagIsFalling()
{
    return _isFalling;
}

void MoneyBag::DrawObject() {

    if (_isOpen) {
        for (int i = 0; i < SPRITE_HEIGHT; i++)
        {
            for (int j = 0; j < SPRITE_WIDTH - 1; j++)
            {
                wData->vBuf[_y + i][_x + j] = moneyBagSpriteOpen[i][j] | (Yellow << 8);
            }
        }
    }
    else {
        for (int i = 0; i < SPRITE_HEIGHT; i++)
        {
            for (int j = 0; j < SPRITE_WIDTH - 1; j++)
            {
                wData->vBuf[_y + i][_x + j] = moneyBagSprite[i][j] | (_color << 8);
            }
        }
    }
    

}

void Diamond::DrawObject()
{
    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        for (int j = 0; j < SPRITE_WIDTH - 1; j++)
        {
            wData->vBuf[_y + i][_x + j] = spriteDiamond[i][j] | (_color << 8);
        }
    }
}

void Bonus::DrawObject()
{
    for (int i = 0; i < SPRITE_HEIGHT; i++)
    {
        for (int j = 0; j < SPRITE_WIDTH - 1; j++)
        {
            wData->vBuf[_y + i][_x + j] = spriteBonus[i][j] | (_color << 8);
        }
    }
}


// -------------------- Wall ----------------

void Wall::DrawObject()
{
    for (int i = 0; i < WALL_HEIGHT; i++)
    {
        for (int j = 0; j < WALL_WIDTH - 1; j++)
        {
            if (wData->vBuf[_y + i][_x + j] == 0 || wData->vBuf[_y + i][_x + j] == u' ') {
                wData->vBuf[_y + i][_x + j] = wallSprite[i][j] | (_color << 8);
            }
        }
    }
}