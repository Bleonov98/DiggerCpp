#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <wchar.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

#include "resource.h"

#pragma comment(lib, "winmm.lib")

#define ESC "\x1b"
#define CSI "\x1b["
#define ROWS 55
#define COLS 90

using namespace std;
using namespace chrono;