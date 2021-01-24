// Beginning Game Programming, Second Edition
// Chapter 10
// Paddle_Game program header file


#ifndef _GAME_H
#define _GAME_H 1

//windows/directx headers
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr9.h>
#include <dsound.h>
#include <dinput.h>
#include <windows.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

//framework-specific headers
#include "dxgraphics.h"
#include "dxaudio.h"
#include "dxinput.h"

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dxerr9.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "winmm.lib")

//application title
#define APPTITLE "Galactis2D"

//screen setup
#define FULLSCREEN 0       //0 = windowed, 1 = fullscreen
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

//function prototypes
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);

//struct for misc position and movement
typedef struct
{
	int x, y;
	int moveY;
}SPRITE;

//enum to handle game states
enum GAMESTATE {MENU, STAGE1, STAGE2, WINNER, LOSER};

#endif

