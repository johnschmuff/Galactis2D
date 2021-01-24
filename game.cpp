//AsterBlaster Game Source file
//John Schmuff
//March 2009

#include "game.h"
#include "ship.h"
#include "weapon.h"
#include "explosion.h"

//how many columns/rows of enemies to draw
//increase for more enemies
//decrease for less enemies
#define COLUMNS 5
#define ROWS 4

//background image
LPDIRECT3DSURFACE9 back;

//sprite handler
LPD3DXSPRITE sprt;

//Images variables
LPDIRECT3DTEXTURE9 spaceImage;
LPDIRECT3DTEXTURE9 menuImage;
LPDIRECT3DTEXTURE9 winnerImage;
LPDIRECT3DTEXTURE9 loserImage;
LPDIRECT3DTEXTURE9 playerImage;
LPDIRECT3DTEXTURE9 enemyImage;
LPDIRECT3DTEXTURE9 bossImage;
LPDIRECT3DTEXTURE9 cannonImage;
LPDIRECT3DTEXTURE9 explosionImage;

//Object sprites;
SPRITE space1;
SPRITE space2;
SPRITE space3;
SPRITE menu;
SPRITE winner;
SPRITE loser;
SHIP player;
SHIP enemies1[ROWS][COLUMNS];
SHIP enemies2[ROWS][COLUMNS];
SHIP boss;
WEAPON cannon;
EXPLOSION explosion;
EXPLOSION playerExplosion;

//the wave sound
CSound *sound_menu;
CSound *sound_stage1;
CSound *sound_stage2;
CSound *sound_cannon;
CSound *sound_explosion;

//misc
long start = GetTickCount();
HRESULT result;
//vector for image positions
D3DXVECTOR3 position(0, 0, 0);
//int for the for loops
int n = 0, i = 0;

//declaration gamestate enum
GAMESTATE gameState = MENU;

//variable declaration function for all class variables and structs
void declareVar()
{
	//Space 1, 2 and 3 properties
	space1.x = 0;
	space1.y = 0;
	space1.moveY = 0;
	space2.x = 0;
	space2.y = -768;
	space2.moveY = 0;
	space3.x = 0;
	space3.y = -1536;
	space3.moveY = 0;

	//Menu, loser and winner properties
	menu.x = 46;
	menu.y = 100;
	loser.x = 46;
	loser.y = 100;
	winner.x = 46;
	winner.y = 100;

	//Player properties
	player.width = 28;
	player.height = 31;
	player.x = (SCREEN_WIDTH / 2) - (player.width / 2);
	player.y = SCREEN_HEIGHT - 100;
	player.movex = 16;
	player.movey = 16;
	player.lives = 3;
	player.enemies = 2 * (ROWS * COLUMNS);
	player.alive = true;

	//Cannon properties
	cannon.width = 9;
	cannon.height = 9;
	cannon.x = -10;
	cannon.y = 0;
	cannon.movey = 14;
	cannon.alive = false;

	for(i = 0; i < ROWS; i++)
	{
		for(n = 0; n < COLUMNS; n++)
		{
			//Enemies1 properties
			enemies1[i][n].width = 64;
			enemies1[i][n].height = 64;
			enemies1[i][n].x = 513 + enemies1[i][n].width * n;
			enemies1[i][n].y = -256 + enemies1[i][n].height * i;
			enemies1[i][n].movex = 7;
			enemies1[i][n].movey = 7;
			enemies1[i][n].alive = true;
		}
	}

	for(i = 0; i < ROWS; i++)
	{
		for(n = 0; n < COLUMNS; n++)
		{
			//Enemies2 properties
			enemies2[i][n].width = 64;
			enemies2[i][n].height = 64;
			enemies2[i][n].x = 192 + enemies1[i][n].width * n;
			enemies2[i][n].y = -256 + enemies1[i][n].height * i;
			enemies2[i][n].movex = 7;
			enemies2[i][n].movey = 7;
			enemies2[i][n].alive = true;
		}
	}


	//boss properties
	boss.width = 112;
	boss.height = 91;
	boss.x = (SCREEN_WIDTH / 2) - (boss.width / 2);
	boss.y = -100;
	boss.movex = 10;
	boss.movey = 12;
	boss.lives = 7;
	boss.alive = true;

	//declare explosion properties
	explosion.x = 0;
	explosion.y = 0;
	explosion.width = 64;
	explosion.height = 64;
	explosion.curframe = 0;
	explosion.lastframe = 7;
	explosion.animdelay = 1;
	explosion.animcount = 0;
	explosion.alive = false;

	//declare playerExplosion properties
	playerExplosion.x = 0;
	playerExplosion.y = 0;
	playerExplosion.width = 64;
	playerExplosion.height = 64;
	playerExplosion.curframe = 0;
	playerExplosion.lastframe = 7;
	playerExplosion.animdelay = 1;
	playerExplosion.animcount = 0;
	playerExplosion.alive = false;
}

//initializes the game
int Game_Init(HWND hwnd)
{
	//set random number seed
	srand((unsigned)time(NULL));

    //initialize mouse
    if (!Init_Mouse(hwnd))
    {
        MessageBox(hwnd, "Error initializing the mouse", "Error", MB_OK);
        return 0;
    }

    //initialize keyboard
    if (!Init_Keyboard(hwnd))
    {
        MessageBox(hwnd, "Error initializing the keyboard", "Error", MB_OK);
        return 0;
    }

    //create sprite handler object
    result = D3DXCreateSprite(d3ddev, &sprt);
    if (result != D3D_OK)
        return 0;

//Declare all images to load below here*************************************

    //load the background image
    back = LoadSurface("graphics/black.bmp", NULL);
    if (back == NULL)
		return 0;

    //load the spaceImage
    spaceImage = LoadTexture("graphics/space.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (spaceImage == NULL)
		return 0;

	//load the menuImage
    menuImage = LoadTexture("graphics/menu.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (menuImage == NULL)
		return 0;

	//load the winnerImage
    winnerImage = LoadTexture("graphics/winner.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (winnerImage == NULL)
		return 0;

	//load the loserImage
    loserImage = LoadTexture("graphics/loser.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (loserImage == NULL)
		return 0;

	//load the playerImage
    playerImage = LoadTexture("graphics/ship.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (playerImage == NULL)
    	return 0;

	//load the enemyImage
    enemyImage = LoadTexture("graphics/enemy.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (enemyImage == NULL)
    	return 0;

	//load the bossImage
    bossImage = LoadTexture("graphics/boss.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (bossImage == NULL)
    	return 0;

	//load the cannonImage
    cannonImage = LoadTexture("graphics/cannon.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (cannonImage == NULL)
    	return 0;

	//load the explosionImage
    explosionImage = LoadTexture("graphics/explosion.bmp", D3DCOLOR_XRGB(255, 0, 255));
    if (explosionImage == NULL)
		return 0;

//Declare all sounds to load below here*************************************

	//load menu music wave file
    sound_menu = LoadSound("sounds/menu.wav");
    if (sound_menu == NULL)
        return 0;

	//load stage1 music wave file
    sound_stage1 = LoadSound("sounds/stage1.wav");
    if (sound_stage1 == NULL)
        return 0;

	//load stage2 music wave file
    sound_stage2 = LoadSound("sounds/stage2.wav");
    if (sound_stage2 == NULL)
        return 0;

	//load cannon wave file
    sound_cannon = LoadSound("sounds/cannon.wav");
    if (sound_cannon == NULL)
        return 0;

	//load explosion wave file
    sound_explosion = LoadSound("sounds/explosion.wav");
    if (sound_explosion == NULL)
        return 0;

//Declare all sprite properties below here**********************************

	//call to function that declares all variables
	declareVar();

    //return okay
    return 1;
}

//Four different collision function for
//ship/ship, ship/weapon, asteroid/weapon
//and asteroid/ship
int Collision1(SHIP ship1, SHIP ship2)
{
	RECT rect1;
	rect1.left = ship1.x + 1;
	rect1.top = ship1.y + 1;
	rect1.right = ship1.x + ship1.width - 1;
	rect1.bottom = ship1.y + ship1.height - 1;

	RECT rect2;
	rect2.left = ship2.x + 1;
	rect2.top = ship2.y + 1;
	rect2.right = ship2.x + ship2.width - 1;
	rect2.bottom = ship2.y + ship2.height - 1;

	RECT dest;

	return IntersectRect(&dest, &rect1, &rect2);
}

int Collision2(SHIP ship1, WEAPON weapon)
{
	RECT rect1;
	rect1.left = ship1.x + 1;
	rect1.top = ship1.y + 1;
	rect1.right = ship1.x + ship1.width - 1;
	rect1.bottom = ship1.y + ship1.height - 1;

	RECT rect2;
	rect2.left = weapon.x + 1;
	rect2.top = weapon.y + 1;
	rect2.right = weapon.x + weapon.width - 1;
	rect2.bottom = weapon.y + weapon.height - 1;

	RECT dest;

	return IntersectRect(&dest, &rect1, &rect2);
}

//function to check if a collision happens
void checkCollison()
{
	for(i = 0; i < ROWS; i++)
	{
		for(n = 0; n < COLUMNS; n++)
		{
			if(enemies1[i][n].alive)
			{
				if(Collision1(player, enemies1[i][n]))
				{
					explosion.alive = false;
					playerExplosion.alive = false;
					ResetSound(sound_explosion);
					PlaySound(sound_explosion);
					enemies1[i][n].alive = false;
					explosion.x = enemies1[i][n].x;
					explosion.y = enemies1[i][n].y;
					playerExplosion.x = player.x;
					playerExplosion.y = player.y;
					explosion.alive = true;
					playerExplosion.alive = true;
					player.lives -= 1;
					player.enemies -= 1;
					player.alive = false;
					if(player.lives <= 0)
					{
						player.alive = false;
						gameState = LOSER;
					}
					if(player.enemies <= 0)
					{
						gameState = STAGE2;
					}
				}
			}
		}
	}
	for(i = 0; i < ROWS; i++)
	{
		for(n = 0; n < COLUMNS; n++)
		{
			if(enemies2[i][n].alive)
			{
				if(Collision1(player, enemies2[i][n]))
				{
					explosion.alive = false;
					playerExplosion.alive = false;
					ResetSound(sound_explosion);
					PlaySound(sound_explosion);
					enemies2[i][n].alive = false;
					explosion.x = enemies2[i][n].x;
					explosion.y = enemies2[i][n].y;
					playerExplosion.x = player.x;
					playerExplosion.y = player.y;
					explosion.alive = true;
					playerExplosion.alive = true;
					player.lives -= 1;
					player.enemies -= 1;
					player.alive = false;
					if(player.lives <= 0)
					{
						player.alive = false;
						gameState = LOSER;
					}
					if(player.enemies <= 0)
					{
						gameState = STAGE2;
					}
				}
			}
		}
	}
	if(Collision1(player, boss))
	{
		ResetSound(sound_explosion);
		PlaySound(sound_explosion);
		player.lives -= 1;
		boss.lives -= 1;
		player.alive = false;
		if(player.lives <= 0)
		{
			player.alive = false;
			gameState = LOSER;
		}
		if(boss.lives <= 0)
		{
			gameState = WINNER;
		}
	}
	for(i = 0; i < ROWS; i++)
	{
		for(n = 0; n < COLUMNS; n++)
		{
			if(enemies1[i][n].alive)
			{
				if(Collision2(enemies1[i][n], cannon))
				{
					explosion.alive = false;
					ResetSound(sound_explosion);
					PlaySound(sound_explosion);
					enemies1[i][n].alive = false;
					explosion.x = enemies1[i][n].x;
					explosion.y = enemies1[i][n].y;
					explosion.alive = true;
					cannon.alive = false;
					player.enemies -= 1;
					//moves the cannon off the screen so there is no odd collision
					cannon.x = -10;
					cannon.y = 0;
					if(player.enemies <= 0)
					{
						gameState = STAGE2;
					}
				}
			}
		}
	}
	for(i = 0; i < ROWS; i++)
	{
		for(n = 0; n < COLUMNS; n++)
		{
			if(enemies2[i][n].alive)
			{
				if(Collision2(enemies2[i][n], cannon))
				{
					explosion.alive = false;
					ResetSound(sound_explosion);
					PlaySound(sound_explosion);
					enemies2[i][n].alive = false;
					explosion.x = enemies2[i][n].x;
					explosion.y = enemies2[i][n].y;
					explosion.alive = true;
					cannon.alive = false;
					player.enemies -= 1;
					//moves the cannon off the screen after collision, so there is no odd
					//collision
					cannon.x = -10;
					cannon.y = 0;
					if(player.enemies <= 0)
					{
						gameState = STAGE2;
					}
				}
			}
		}
	}
	if(cannon.alive && boss.alive)
	{
		if(Collision2(boss, cannon))
		{
			explosion.alive = false;
			ResetSound(sound_explosion);
			PlaySound(sound_explosion);
			cannon.alive = false;
			explosion.x = boss.x;
			explosion.y = boss.y;
			explosion.alive = true;
			boss.lives -= 1;
			if(boss.lives <= 0)
			{
				gameState = WINNER;
			}
		}
	}
}

//function for animating the explosion
void animateExplosion()
{
	//enemy explosion
	if(explosion.alive == true)
	{
		//has animation delay reached threshold?
		if (++explosion.animcount > explosion.animdelay)
		{
			//reset counter
			explosion.animcount = 0;

			//animate the sprite
			if (++explosion.curframe > explosion.lastframe)
				explosion.curframe = 0;

			else if(explosion.curframe > 6)
				explosion.alive = false;
		}
	}
	//player explosion
	if(playerExplosion.alive == true)
	{
		//has animation delay reached threshold?
		if (++playerExplosion.animcount > playerExplosion.animdelay)
		{
			//reset counter
			playerExplosion.animcount = 0;

			//animate the sprite
			if (++playerExplosion.curframe > playerExplosion.lastframe)
				playerExplosion.curframe = 0;

			else if(playerExplosion.curframe > 6)
				playerExplosion.alive = false;
		}
	}
}

//function to control the movement of space 1, 2 and 3
void scrollBackground()
{
	if(gameState == STAGE1)
	{
		space1.moveY = 10;
		space2.moveY = 10;
		space3.moveY = 10;
	}
	else if(gameState == STAGE2)
	{
		space1.moveY = 3;
		space2.moveY = 3;
		space3.moveY = 3;
	}
	if(gameState == WINNER || gameState == LOSER || gameState == MENU)
	{
		space1.moveY = 0;
		space2.moveY = 0;
		space3.moveY = 0;
	}
	space1.y += space1.moveY;
	if(space1.y > SCREEN_HEIGHT)
		space1.y = -1536;
	space2.y += space2.moveY;
	if(space2.y > SCREEN_HEIGHT)
		space2.y = -1536;
	space3.y += space3.moveY;
	if(space3.y > SCREEN_HEIGHT)
		space3.y = -1536;
}

//function to handle the keys during menu gamestate
void menuControl()
{
	if(Key_Down(DIK_P))
	{
		gameState = STAGE1;
	}
}

//function to handle the keys during gameplay
void gameControl()
{
	if(player.alive)
	{
		if(Key_Down(DIK_UP))
		{
			player.y -= player.movey;
		}
		else if(Key_Down(DIK_DOWN))
		{
			player.y += player.movey;
		}
		if(Key_Down(DIK_LEFT))
		{
			player.x -= player.movex;
		}
		else if(Key_Down(DIK_RIGHT))
		{
			player.x += player.movex;
		}

		if(!cannon.alive)//checks to see if cannon is alive first
		{
			if(Key_Down(DIK_SPACE))
			{
				ResetSound(sound_cannon);
				cannon.x = player.x + 10;
				cannon.y = player.y - 9;
				cannon.alive = true;
				PlaySound(sound_cannon);
			}
		}
	}
}

//function to handle the keys during win/lose gamestate
void win_loseControl()
{
	if(Key_Down(DIK_R))
	{
		declareVar();
		gameState = STAGE1;
	}
	else if(Key_Down(DIK_M))
	{
		declareVar();
		gameState = MENU;
	}
}

//function to control the movement of the player, creating
//a boundary for the window so the player cannot leave it
void playerMovement()
{
	if(player.alive)
	{
		if(player.x < 0)
		{
			player.x = 0;
		}
		else if(player.x > SCREEN_WIDTH - player.width)
		{
			player.x = SCREEN_WIDTH - player.width;
		}
		if(player.y < 0)
		{
			player.y = 0;
		}
		else if(player.y > SCREEN_HEIGHT - player.height)
		{
			player.y = SCREEN_HEIGHT - player.height;
		}
	}
	else if(!player.alive) // moves the player back to a start position so the collision wont keep accuring
	{
		player.x = (SCREEN_WIDTH / 2) - (player.width / 2);
		player.y = SCREEN_HEIGHT - 100;
		player.alive = true;
	}
}

//checks to see if cannon is alive or not,
//if it is alive moves the cannon across
//the screen
void weaponMovement()
{
	if(cannon.alive)
		cannon.y -= cannon.movey;
	if(cannon.y < 0)
		cannon.alive = false;
}

//function to control the enemy movement pattern during
//gameplay for both stage1 and stage2 and to check if each
//stage has been beaten and then changes the gameState
void enemyMovement()
{
	if(gameState == STAGE1)
	{
		for(i = 0; i < ROWS; i++)
		{
			for(n = 0; n < COLUMNS; n++)
			{
				//moves the enemies into the game screen
				if(enemies1[i][n].y < 0)
				{
					enemies1[i][n].y += enemies1[i][n].movey;
				}
				else if(enemies1[i][n].y > 0)
				{
					//once enemies are in place they start moving in both axis directions
					enemies1[i][n].x += enemies1[i][n].movex;
					enemies1[i][n].y += enemies1[i][n].movey;
					//keep enemies in screen boundaries
					if(enemies1[i][n].x > SCREEN_WIDTH - enemies1[i][n].width)
					{
						enemies1[i][n].movex *= -1;
					}
					else if(enemies1[i][n].x < 0)
					{
						enemies1[i][n].movex *= -1;
					}
					if(enemies1[i][n].y > SCREEN_HEIGHT - enemies1[i][n].height)
					{
						enemies1[i][n].movey *= -1;
					}
					else if(enemies1[i][n].y < 0)
					{
						enemies1[i][n].movey *= -1;
					}
				}
			}
		}

		for(i = 0; i < ROWS; i++)
		{
			for(n = 0; n < COLUMNS; n++)
			{
				//moves the enemies into the game screen
				if(enemies2[i][n].y < 0)
				{
					enemies2[i][n].y += enemies2[i][n].movey;
				}
				else if(enemies2[i][n].y > 0)
				{
					//once enemies are in place they start moving in both axis directions
					enemies2[i][n].x -= enemies2[i][n].movex;
					enemies2[i][n].y += enemies2[i][n].movey;
					//keep enemies in screen boundaries
					if(enemies2[i][n].x > SCREEN_WIDTH - enemies2[i][n].width)
					{
						enemies2[i][n].movex *= -1;
					}
					else if(enemies2[i][n].x < 0)
					{
						enemies2[i][n].movex *= -1;
					}
					if(enemies2[i][n].y > SCREEN_HEIGHT - enemies2[i][n].height)
					{
						enemies2[i][n].movey *= -1;
					}
					else if(enemies1[i][n].y < 0)
					{
						enemies2[i][n].movey *= -1;
					}
				}
			}
		}
	}
	else if(gameState == STAGE2)
	{
		//moves the boss into the game screen
		if(boss.y < 0)
			boss.y += boss.movey;
		else if(boss.y >= 0)
		{
			//once the boss is in place it starts moving along the y and x axis
			boss.x += boss.movex;
			boss.y += boss.movey;
			//keep boss in screen boundaries
			if(boss.x < 0)
			{
				boss.movex *= -1;
			}
			else if(boss.x > SCREEN_WIDTH - boss.width)
			{
				boss.movex *= -1;
			}
			if(boss.y < 0)
			{
				boss.movey *= -1;
			}
			else if(boss.y > SCREEN_HEIGHT - boss.height)
			{
				boss.movey *= -1;
			}
		}
	}
}

//function to handle which music schemes play for gameStates
void sound()
{
	switch(gameState)
	{
	case MENU:
		PlaySound(sound_menu);
		break;
	case STAGE1:
		StopSound(sound_menu);
		PlaySound(sound_stage1);
		break;
	case STAGE2:
		StopSound(sound_stage1);
		PlaySound(sound_stage2);
		break;
	case WINNER:
	case LOSER:
		ResetSound(sound_menu);
		ResetSound(sound_stage1);
		ResetSound(sound_stage2);
		StopSound(sound_stage2);
		break;
	};
}

//function to handle all the function for playing the game
void update()
{
	//switch statement to determine which controls and movement
	//are getting used during certain gameStates
	switch(gameState)
	{
	case MENU:
		menuControl();
		sound();
		break;
	case STAGE1:
		sound();
		scrollBackground();
		gameControl();
		playerMovement();
		weaponMovement();
		enemyMovement();
		checkCollison();
		animateExplosion();
		break;
	case STAGE2:
		sound();
		scrollBackground();
		gameControl();
		playerMovement();
		weaponMovement();
		enemyMovement();
		checkCollison();
		animateExplosion();
		break;
	case WINNER:
	case LOSER:
		sound();
		win_loseControl();
		break;
	};
}

void drawScrollingBackground()
{
	//draw space 1, 2 and 3 with scrolling
	position.x = (float)space1.x;
	position.y = (float)space1.y;
	sprt->Draw(spaceImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	position.x = (float)space2.x;
	position.y = (float)space2.y;
	sprt->Draw(spaceImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	position.x = (float)space3.x;
	position.y = (float)space3.y;
	sprt->Draw(spaceImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
}
void drawPlayer()
{
	//Draw player
	position.x = (float)player.x;
	position.y = (float)player.y;
	sprt->Draw(playerImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	//condition if cannon is alive then draw cannon
	if(cannon.alive)
	{
		//draw cannon
		position.x = (float)cannon.x;
		position.y = (float)cannon.y;
		sprt->Draw(cannonImage, NULL, NULL, &position, D3DCOLOR_XRGB(255 ,255, 255));
	}
}
void drawEnemies()
{
	for(i = 0; i < ROWS; i++)
	{
		//for loop to draw n amount of enemies
		for(n = 0; n < COLUMNS; n++)
		{
			//condition to check is enemies are alive if so then draw enemy
			if(enemies1[i][n].alive)
			{
				//draw enemy
				position.x = (float)enemies1[i][n].x;
				position.y = (float)enemies1[i][n].y;
				sprt->Draw(enemyImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
			}
		}
	}

	for(i = 0; i < ROWS; i++)
	{
		//for loop to draw n amount of enemies
		for(n = 0; n < COLUMNS; n++)
		{
			//condition to check is enemies are alive if so then draw enemy
			if(enemies2[i][n].alive)
			{
				//draw enemy
				position.x = (float)enemies2[i][n].x;
				position.y = (float)enemies2[i][n].y;
				sprt->Draw(enemyImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
			}
		}
	}
}
void drawBoss()
{
	if(boss.alive)
	{
		position.x = (float)boss.x;
		position.y = (float)boss.y;
		sprt->Draw(bossImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void drawMenuLoserWinner()
{
	if(gameState == MENU)
	{
		//Draw menu
		position.x = (float)menu.x;
		position.y = (float)menu.y;
		sprt->Draw(menuImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	}
	if(gameState == LOSER)
	{
		//draw loser
		position.x = (float)loser.x;
		position.y = (float)loser.y;
		sprt->Draw(loserImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	}
	if(gameState == WINNER)
	{
		//draw winner
		position.x = (float)winner.x;
		position.y = (float)winner.y;
		sprt->Draw(winnerImage, NULL, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void drawExplosion()
{
	if(explosion.alive)
	{
		//configure the rect for the source tile
		RECT srcRect1;
		int columns = 2;
		srcRect1.left = (explosion.curframe % columns) * explosion.width;
		srcRect1.top = (explosion.curframe / columns) * explosion.height;
		srcRect1.right = srcRect1.left + explosion.width;
		srcRect1.bottom = srcRect1.top + explosion.height;

		//draw explosion
		position.x = (float)explosion.x;
		position.y = (float)explosion.y;
		sprt->Draw(explosionImage, &srcRect1, NULL, &position, D3DCOLOR_XRGB(255, 255, 255));
	}
}

//the main game loop
void Game_Run(HWND hwnd)
{
    //make sure the Direct3D device is valid
    if (d3ddev == NULL)
        return;

    //update mouse and keyboard
    Poll_Mouse();
    Poll_Keyboard();

    //after short delay, ready for next frame?
    //this keeps the game running at a steady frame rate
    if (GetTickCount() - start >= 30)
    {
        //reset timing
        start = GetTickCount();

		//insert game function here
		update();
        
    }

    //start rendering
    if (d3ddev->BeginScene())
    {
        //erase the entire background
        d3ddev->StretchRect(back, NULL, backbuffer, NULL, D3DTEXF_NONE);

        //start sprite handler
        sprt->Begin(D3DXSPRITE_ALPHABLEND);

		//switch statement to control which function are being called
		//for image drawing
		switch(gameState)
		{
		case MENU:
			drawScrollingBackground();
			drawMenuLoserWinner();
			break;
		case STAGE1:
			drawScrollingBackground();
			drawPlayer();
			drawEnemies();
			drawExplosion();
			break;
		case STAGE2:
			drawScrollingBackground();
			drawPlayer();
			drawBoss();
			drawExplosion();
			break;
		case WINNER:
			drawScrollingBackground();
			drawMenuLoserWinner();
			break;
		case LOSER:
			drawScrollingBackground();
			drawMenuLoserWinner();
			break;
		};

        //stop drawing
        sprt->End();
       
        //stop rendering
        d3ddev->EndScene();
    }

    //display the back buffer on the screen
    d3ddev->Present(NULL, NULL, NULL, NULL);

    //check for mouse button (to exit program)
    if (Mouse_Button(0))
        PostMessage(hwnd, WM_DESTROY, 0, 0);

    //check for escape key (to exit program)
    if (Key_Down(DIK_ESCAPE))
        PostMessage(hwnd, WM_DESTROY, 0, 0);
}

//frees memory and cleans up before the game ends
void Game_End(HWND hwnd)
{

    if (explosionImage != NULL)
        explosionImage->Release();

	if (cannonImage != NULL)
        cannonImage->Release();

	if (bossImage != NULL)
        bossImage->Release();

	if (enemyImage != NULL)
        enemyImage->Release();

	if (playerImage != NULL)
        playerImage->Release();

	if (winnerImage != NULL)
        winnerImage->Release();

	if (loserImage != NULL)
        loserImage->Release();

	if (menuImage != NULL)
        menuImage->Release();

	if (spaceImage != NULL)
        spaceImage->Release();

    if (back != NULL)
        back->Release();

    if (sprt != NULL)
        sprt->Release();
}

