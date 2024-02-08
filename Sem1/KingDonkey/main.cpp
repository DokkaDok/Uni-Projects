#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<SDL.h>

#define SCREEN_WIDTH	640 // pixels
#define SCREEN_HEIGHT	480 // pixels

#define STRUCT_ARGS		Game *game, Player *player // pointers to structs
#define STRUCT_PVALS	&game, &player // addresses of structs
#define STRUCT_VALS		game, player // structs

#define INVI_FRAMES		30 // frames of invincibility
#define VELOCITY		500 // velocity of objects, controls jump hight

#define PLAYER_WIDTH	36 // pixels
#define	PLAYER_HEIGHT	-44 // pixels
#define PLAYER_SPEED	200 // speed of player
#define PLAYER_STARTX	10 // pixels
#define PLAYER_STARTY	SCREEN_HEIGHT

#define BARREL_SPEED	250 // speed of traveling barrel
#define BARREL_NUMBER	6 // number of live barrels
#define BARREL_WIDTH	36 // pixels
#define	BARREL_HEIGHT	34 // pixels
#define BARREL_YSTART	70 // y position in pixels
#define BARREL_VELOCITY 200 // speed of falling barrel
#define BARREL_YEND		440 // from which y should barrrels dissapear

#define PLATFORM_GAP	120 // pixels
#define PLATFORM_HEIGHT	16 // pixels
#define	PLATFORM_LWIDTH	530 // pixels
#define PLATFORM_SWIDTH	100 // pixels
#define GAPS			4 // number of gaps between platforms, uniform for all levels

#define LADDER_WIDTH	48 // pixels
#define LADDER_HEIGHT	120 // pixels
#define LADDER_YSTART	PLATFORM_GAP-PLATFORM_HEIGHT // starting y of ladders, calculated to fit platforms

#define LEVEL_NUMB		3 // number of levels
#define GAME_LEVEL		game->levels.level[game->stage-1] // shortcut for later functions, directs to choosen stage

#define END_WIDTH		32 // end object dimactions in pixels
#define END_HEIGHT		32 // end object dimactions in pixels

#define TIME_INFO_Y		10 // pixels
#define GAME_INFO_Y		26 // pixels

struct Player {
	SDL_Surface* toad;
	double speed = 0;
	double grav = 0;
	double x = PLAYER_STARTX;
	double y = PLAYER_STARTY;
	bool isJump = false;
	bool isClimbUp = false;
	bool isClimbDown = false;
	int direction = 0;
	int health = 3;
	bool invulnerable = false;
};

struct EndGate {
	int x;
	int y;
};

struct Barrel {
	double speed;
	int xMax; // limiters for movemant in x direction
	int xMin; // limiters for movemant in x direction
	double x;
	double y;
	int direction = 1;
	int xStart; // saved x for restarting
};

struct Platform {
	int x, y, width; //width needed for collision with diffrent kind of platforms (futureproof)
};

struct Ladder {
	int x, y;
};

struct Level {
	struct Platform platform[GAPS];
	struct Ladder ladder[GAPS - 1];
	struct Barrel barrel[BARREL_NUMBER];
	struct EndGate peach;
};

struct Levels {
	Level level[3] = { 
		{ 
			//level1
			{
				// platforms placemant in x,y,width
				{ (SCREEN_WIDTH - PLATFORM_LWIDTH) / 2, PLATFORM_GAP, PLATFORM_LWIDTH },
				{ (SCREEN_WIDTH - PLATFORM_LWIDTH) / 2, PLATFORM_GAP * 2, PLATFORM_LWIDTH },
				{ (SCREEN_WIDTH - PLATFORM_LWIDTH) / 2, PLATFORM_GAP * 3, PLATFORM_LWIDTH },
				{ (SCREEN_WIDTH - PLATFORM_LWIDTH) / 2, PLATFORM_GAP * 4, PLATFORM_LWIDTH }
			},
			{
				// ladders placemant in x,y
				{450 ,LADDER_YSTART},
				{150 ,LADDER_YSTART + PLATFORM_GAP},
				{450 ,LADDER_YSTART + PLATFORM_GAP * 2}
			},
			{
				// barrels movemant in speed,xmax,xmin,x,y,initial direction,starting x
				{BARREL_SPEED, 580, 60, 60, BARREL_YSTART, 1, 60},
				{BARREL_SPEED, 580, 60, 60, BARREL_YSTART - 300, 1, 60},
				{BARREL_SPEED, 580, 60, 60, BARREL_YSTART - 300 * 2, 1, 60},
				{BARREL_SPEED, 580, 60, 60, BARREL_YSTART - 300 * 3, 1, 60},
				{BARREL_SPEED, 580, 60, 60, BARREL_YSTART - 300 * 4, 1, 60},
				{BARREL_SPEED, 580, 60, 60, BARREL_YSTART - 300 * 5, 1, 60}
			},
			// end gate position in x,y 
			{55,104}
		},
		//level2
		{
			{
				// platforms placemant in x,y,width
				{ SCREEN_WIDTH - PLATFORM_LWIDTH + 15, PLATFORM_GAP, PLATFORM_LWIDTH },
				{ -15, PLATFORM_GAP * 2, PLATFORM_LWIDTH },
				{ SCREEN_WIDTH - PLATFORM_LWIDTH + 15, PLATFORM_GAP * 3, PLATFORM_LWIDTH },
				{ -15, PLATFORM_GAP * 4, PLATFORM_LWIDTH }
			},
			{
				// ladders placemant in x,y
				{570 ,LADDER_YSTART},
				{150 ,LADDER_YSTART + PLATFORM_GAP},
				{450 ,LADDER_YSTART + PLATFORM_GAP * 2}
			},
			{
				// barrels movemant in speed,xmax,xmin,x,y,initial direction,starting x
				{BARREL_SPEED, 515, 125, 515, BARREL_YSTART, -1, 515},
				{BARREL_SPEED, 515, 125, 515, BARREL_YSTART - 300, -1, 515},
				{BARREL_SPEED, 515, 125, 515, BARREL_YSTART - 300 * 2, -1, 515},
				{BARREL_SPEED, 515, 125, 515, BARREL_YSTART - 300 * 3, -1, 515},
				{BARREL_SPEED, 515, 125, 515, BARREL_YSTART - 300 * 4, -1, 515},
				{BARREL_SPEED, 515, 125, 515, BARREL_YSTART - 300 * 5, -1, 515}
			},
			// end gate position in x,y
			{10,104}
		},
		//level3
		{
			{
				// platforms placemant in x,y,width
				{ -30, PLATFORM_GAP, PLATFORM_LWIDTH },
				{ SCREEN_WIDTH - PLATFORM_LWIDTH + 30, PLATFORM_GAP * 2, PLATFORM_LWIDTH },
				{ -30, PLATFORM_GAP * 3, PLATFORM_LWIDTH },
				{ SCREEN_WIDTH - PLATFORM_LWIDTH + 30, PLATFORM_GAP * 4, PLATFORM_LWIDTH }
			},
			{
				// ladders placemant in x,y
				{500 ,LADDER_YSTART},
				{150 ,LADDER_YSTART + PLATFORM_GAP},
				{400 ,LADDER_YSTART + PLATFORM_GAP * 2}
			},
			{
				// barrels movemant in speed,xmax,xmin,x,y,initial direction,starting x
				{BARREL_SPEED, 500, 100, 100, BARREL_YSTART, 1, 100},
				{BARREL_SPEED, 500, 100, 100, BARREL_YSTART - 260, 1, 100},
				{BARREL_SPEED, 500, 100, 500, BARREL_YSTART + PLATFORM_GAP, -1, 500},
				{BARREL_SPEED, 500, 100, 100, BARREL_YSTART - 260 * 3, 1, 100},
				{BARREL_SPEED, 500, 100, 100, BARREL_YSTART - 260 * 4, 1, 100},
				{BARREL_SPEED, 500, 100, 100, BARREL_YSTART - 260 * 4, 1, 100}
			},
			// end gate position in x,y 
			{10,104}
		}
	};
};

struct Game {
	SDL_Surface* screen;
	SDL_Surface* charset;
	SDL_Surface* platform_l;
	SDL_Surface* ladder;
	SDL_Surface* peach;
	SDL_Surface* keg;
	SDL_Texture* scrtex;
	SDL_Window* window;
	SDL_Renderer* renderer;

	int rc;
	int gameState = 2;
	double delta = 0;
	int frames = 0;
	double fpsTimer = 0;
	double fps = 0;
	double worldTime = 0;
	double distance = 0;
	int quit = 0;
	int stage = 1;

	Levels levels;

	// initializing sdl and checking for errors
	int init() {
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
			printf("SDL_Init error: %s\n", SDL_GetError());
			return 1;
		};
	};
};

void DrawString(SDL_Surface* screen, int x, int y, const char* text,
	SDL_Surface* charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};

void DrawSurface(SDL_Surface* screen, SDL_Surface* sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

void DrawPixel(SDL_Surface* surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32*)p = color;
};

void DrawLine(SDL_Surface* screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for (int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
	};
};

void DrawRectangle(SDL_Surface* screen, int x, int y, int l, int k,
	Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for (i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
};

void BarrelMovemant(STRUCT_ARGS) {
	for (int i = 0; i < BARREL_NUMBER; i++)
	{
		// if barrel above spawn y make it fall
		if (GAME_LEVEL.barrel[i].y < BARREL_YSTART) {
			GAME_LEVEL.barrel[i].y += BARREL_VELOCITY * game->delta;
		}
		// otherwise barrel can move normally
		else {
			GAME_LEVEL.barrel[i].x += BARREL_SPEED * GAME_LEVEL.barrel[i].direction * game->delta;
		}

		// if barrel x is at max change direction and move platform lower
		if (GAME_LEVEL.barrel[i].x > GAME_LEVEL.barrel[i].xMax + 10 && GAME_LEVEL.barrel[i].y < BARREL_YEND) {
			GAME_LEVEL.barrel[i].y += PLATFORM_GAP;
			GAME_LEVEL.barrel[i].x = GAME_LEVEL.barrel[i].xMax;
			GAME_LEVEL.barrel[i].direction *= -1;
		}
		// if barrel x is at min change direction and move platform lower
		if (GAME_LEVEL.barrel[i].x < GAME_LEVEL.barrel[i].xMin - 10 && GAME_LEVEL.barrel[i].y < BARREL_YEND) {
			GAME_LEVEL.barrel[i].y += PLATFORM_GAP;
			GAME_LEVEL.barrel[i].x = GAME_LEVEL.barrel[i].xMin;
			GAME_LEVEL.barrel[i].direction *= -1;
		}
		// if barrel in end zone y and crossed xmin/xmax move to y start
		if (GAME_LEVEL.barrel[i].y > BARREL_YEND 
			&& (GAME_LEVEL.barrel[i].x < GAME_LEVEL.barrel[i].xMin
			|| GAME_LEVEL.barrel[i].x > GAME_LEVEL.barrel[i].xMax)) {

			GAME_LEVEL.barrel[i].y = BARREL_YSTART;
			GAME_LEVEL.barrel[i].x = GAME_LEVEL.barrel[i].xStart;
		}
	}
};

void BarrelCollision(STRUCT_ARGS) {
	for (int i = 0; i < BARREL_NUMBER; i++)
	{
		// check player between barrels x
		if (player->x >= GAME_LEVEL.barrel[i].x
			&& player->x <= GAME_LEVEL.barrel[i].x + BARREL_WIDTH
			// check player between barrels y
			&& player->y >= GAME_LEVEL.barrel[i].y 
			&& player->y <= GAME_LEVEL.barrel[i].y + BARREL_HEIGHT - PLAYER_HEIGHT/2) {

			// check invulnerable frames
			if (!player->invulnerable) {
				player->health--;
				player->invulnerable = true;
			}
		}
	}
};

int LadderCollision(STRUCT_ARGS) {
	for (int i = 0; i < GAPS - 1; i++)
	{
		// checks if player is between two sides of ladder and bottom and top
		if ((player->x >= GAME_LEVEL.ladder[i].x || player->x + PLAYER_WIDTH / 2 >= GAME_LEVEL.ladder[i].x)
			&& player->x <= GAME_LEVEL.ladder[i].x + LADDER_WIDTH
			&& player->y >= GAME_LEVEL.ladder[i].y
			&& player->y <= GAME_LEVEL.ladder[i].y + LADDER_HEIGHT) {

			return 1;
		}
	}
	return 0;
}

int PlayerCollision(STRUCT_ARGS) {
	for (int i = 0; i < GAPS; i++)
	{
		// checks if player is between platform bottom and top and two sides
		if (player->y <= GAME_LEVEL.platform[i].y 
			&& player->y >= GAME_LEVEL.platform[i].y - PLATFORM_HEIGHT
			&& player->x + PLAYER_WIDTH > GAME_LEVEL.platform[i].x
			&& player->x < GAME_LEVEL.platform[i].x + GAME_LEVEL.platform[i].width) {

			return i+1;
		}
	}
	return 0;
}

void LoadSprites(STRUCT_ARGS) {
	game->charset = SDL_LoadBMP("./images/cs8x8.bmp");
	player->toad = SDL_LoadBMP("./images/toad.bmp");
	game->keg = SDL_LoadBMP("./images/barrel.bmp");
	game->platform_l = SDL_LoadBMP("./images/platform_long.bmp");
	game->ladder = SDL_LoadBMP("./images/ladder.bmp");
	game->peach = SDL_LoadBMP("./images/peach.bmp");
}

// sets initial values of levels and players x,y,health
void GameRestart(STRUCT_ARGS) {
	Levels levels;

	game->stage = 1;
	game->worldTime = 0; // reset timer
	game->levels.level[0] = levels.level[0];
	game->levels.level[1] = levels.level[1];
	game->levels.level[2] = levels.level[2];
	player->x = PLAYER_STARTX;
	player->y = PLAYER_STARTY;
	player->health = 3;
}

// loads new stage with starting values and sets players x,y
void ChangeStage(STRUCT_ARGS, int stage) {
	Levels levels;
	game->levels.level[game->stage-1] = levels.level[game->stage-1];
	player->x = PLAYER_STARTX;
	player->y = PLAYER_STARTY;
	game->worldTime = 0; // reset timer
	game->stage = stage;
}

void EventHandler(STRUCT_ARGS) {
	SDL_Event event;
	// lets player move if in screen
	if (player->x < 0 || player->x > SCREEN_WIDTH-PLAYER_WIDTH) {
		player->x += PLAYER_SPEED * (-player->direction) * game->delta;
	};
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_a:
				// checks if player in screen and isnt jumping
				if (player->x >= 0 && !player->isJump) {
					player->speed = PLAYER_SPEED;
					player->direction = -1; // changes direction
				}
				break;
			case SDLK_d:
				// checks if player in screen and isnt jumping
				if (player->x <= SCREEN_WIDTH && !player->isJump) {
					player->speed = PLAYER_SPEED;
					player->direction = 1; // changes direction
				} 
				break;
			case SDLK_w:
				// if on ladder, can move up
				if (LadderCollision(STRUCT_VALS)) {
					player->isClimbUp = true;
				}
				break;
			case SDLK_s:
				// if on ladder, can move down
				if (LadderCollision(STRUCT_VALS)) {
					player->isClimbDown = true;
				}
				break;
			case SDLK_SPACE:
				// jump only if not jumping
				if (!player->isJump) {
					player->direction = player->direction; // saves direction, not possible to change during jump
					player->grav = -250; // makes player move upwards
					player->isJump = true;
				}
				break;
			case SDLK_n:
				game->gameState = 1; // resets game
				break;
			case SDLK_ESCAPE:
				game->quit = 1; // quits game
				break;
			case SDLK_1:
				game->stage = 1;
				ChangeStage(STRUCT_VALS, 1);
				break;
			case SDLK_2:
				game->stage = 2;
				ChangeStage(STRUCT_VALS, 2);
				break;
			case SDLK_3:
				game->stage = 3;
				ChangeStage(STRUCT_VALS, 3);
				break;
			};
			break;
		// if key up stop all movemant
		case SDL_KEYUP:
			switch (event.key.keysym.sym) {
			case SDLK_a:
				player->speed = 0;
				break;
			case SDLK_d:
				player->speed = 0;
				break;
			}
			player->isClimbUp = false;
			player->isClimbDown = false;
			break;
		case SDL_QUIT:
			game->quit = 1;
			break;
		};
	};
}

void isGround(STRUCT_ARGS) {
	// checks all collisions for ground and stops jump
	if (player->y == SCREEN_HEIGHT || PlayerCollision(STRUCT_VALS) || LadderCollision(STRUCT_VALS)) {
		player->grav = 0;
		player->isJump = false;
	}
	else {
		player->isJump = true;
	}
}

void PlayerMovemant(STRUCT_ARGS) {
	// moves player in choosen direction
	player->x += player->speed * player->direction * game->delta;
	
	// stops player from falling under screen
	if (player->y > SCREEN_HEIGHT) {
		player->grav = 0;
		player->y = SCREEN_HEIGHT;
	}

	// moves player on top of platform
	if (PlayerCollision(STRUCT_VALS) && !LadderCollision(STRUCT_VALS)) {
		player->y = GAME_LEVEL.platform[PlayerCollision(STRUCT_VALS)-1].y-PLATFORM_HEIGHT;
	}

	// changes gravity to make player rise and then fall
	if (player->isJump) {
		player->y += player->grav * game->delta;
		player->grav += VELOCITY * game->delta;
	}

	isGround(STRUCT_VALS);

	if (LadderCollision(STRUCT_VALS) && player->isClimbUp) {
		player->y -= 0.2;
	}
	if (LadderCollision(STRUCT_VALS) && player->isClimbDown) {
		player->y += 0.2;
	}
}

void NextStage(STRUCT_ARGS) {
	// checks if player inside x,y of end gate
	if ((player->x >= GAME_LEVEL.peach.x || player->x + PLAYER_WIDTH / 2 >= GAME_LEVEL.peach.x)
		&& player->x <= GAME_LEVEL.peach.x + END_WIDTH
		&& player->y >= GAME_LEVEL.peach.y
		&& player->y <= GAME_LEVEL.peach.y + END_HEIGHT) {

		// sets starting x,y for player
		player->x = PLAYER_STARTX;
		player->y = PLAYER_STARTY;

		if (game->stage == 3) {
			game->stage = 1;
		}
		else {
			game->stage++;
		}
		game->worldTime = 0; // resets time
	}
}

void GameMovemant(STRUCT_ARGS) {
	// manages invulnerability, checks if invi frames passed
	if (player->invulnerable) {
		if (game->worldTime < game->frames * INVI_FRAMES) {
			game->worldTime;
		}
		else {
			player->invulnerable = false;
		}
	}

	EventHandler(STRUCT_VALS);
	PlayerMovemant(STRUCT_VALS);

	BarrelMovemant(STRUCT_VALS);
	BarrelCollision(STRUCT_VALS);

	NextStage(STRUCT_VALS);
}

void GameRender(STRUCT_ARGS) {
	int t1, t2;

	// sets colors
	int czarny = SDL_MapRGB(game->screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(game->screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(game->screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(game->screen->format, 0x11, 0x11, 0xCC);

	char text[128];
	game->worldTime = 0; // sets worldTime
	game->fps = 0;

	t1 = SDL_GetTicks();
	
	while (!game->quit && game->gameState == 2) {

		// manages time
		t2 = SDL_GetTicks();

		game->delta = (t2 - t1) * 0.001;
		t1 = t2;

		game->worldTime += game->delta;

		// fills screen black
		SDL_FillRect(game->screen, NULL, czarny);
		// draws player and end gate
		DrawSurface(game->screen, player->toad, player->x + PLAYER_WIDTH / 2, player->y + PLAYER_HEIGHT/2);
		DrawSurface(game->screen, game->peach, GAME_LEVEL.peach.x + END_WIDTH / 2, GAME_LEVEL.peach.y - END_HEIGHT / 2);

		// draws every platform
		for (int i = 0; i < GAPS; i++)
		{
			DrawSurface(game->screen, game->platform_l, GAME_LEVEL.platform[i].x + GAME_LEVEL.platform[i].width / 2, GAME_LEVEL.platform[i].y - PLATFORM_HEIGHT / 2);
		}
		// draws every ladder
		for (int i = 0; i < GAPS-1; i++)
		{
			DrawSurface(game->screen, game->ladder, GAME_LEVEL.ladder[i].x + LADDER_WIDTH / 2, GAME_LEVEL.ladder[i].y + LADDER_HEIGHT / 2);
		}
		// draws every barrel
		for (int i = 0; i < BARREL_NUMBER; i++)
		{
			DrawSurface(game->screen, game->keg, GAME_LEVEL.barrel[i].x + BARREL_WIDTH / 2, GAME_LEVEL.barrel[i].y + BARREL_HEIGHT / 2);
		}
		// adds additional platform for end gate (peach)
		if (game->stage == 2) {
			DrawSurface(game->screen, game->platform_l, -220, PLATFORM_GAP - PLATFORM_HEIGHT / 2);
		}

		// manages fps
		game->fpsTimer += game->delta;
		if (game->fpsTimer > 0.5) {
			game->fps = game->frames * 2;
			game->frames = 0;
			game->fpsTimer -= 0.5;
		}

		// draws info windows on top of the screen
		DrawRectangle(game->screen, 4, 4, SCREEN_WIDTH - 8, 36, czerwony, niebieski);
		sprintf(text, "Game Time = %.1lf s  %.0lf frames / s", game->worldTime, game->fps);
		DrawString(game->screen, game->screen->w / 2 - strlen(text) * 8 / 2, TIME_INFO_Y, text, game->charset);
		sprintf(text, "Life: %.1li. Points done: 1. 2. 3. 4. A. B. C.", player->health);
		DrawString(game->screen, game->screen->w / 2 - strlen(text) * 8 / 2, GAME_INFO_Y, text, game->charset);
		// updates screen
		SDL_UpdateTexture(game->scrtex, NULL, game->screen->pixels, game->screen->pitch);
		SDL_RenderCopy(game->renderer, game->scrtex, NULL, NULL);
		SDL_RenderPresent(game->renderer);

		GameMovemant(STRUCT_VALS);

		game->frames++;
	};
};

void CreateGame(STRUCT_ARGS) {
	// creates window
	game->rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&game->window, &game->renderer);

	// sets rendering settings
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(game->renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_SetWindowTitle(game->window, "King Donkey s197633");

	game->screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	game->scrtex = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	LoadSprites(STRUCT_VALS);

	SDL_SetColorKey(game->charset, true, 0x000000);
	SDL_ShowCursor(SDL_DISABLE);
}

// manages restart game logic with gameState
void Logic(STRUCT_ARGS) {
	while (!game->quit) {
		if (game->gameState == 1) {
			GameRestart(STRUCT_VALS);
			game->gameState = 2;
		};
		if (game->gameState == 2) {
			GameRender(STRUCT_VALS);
		};
	};
};

// checks error for sprite loading
int ErrorCheck(STRUCT_ARGS) {
	if (game->charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(game->screen);
		SDL_DestroyTexture(game->scrtex);
		SDL_DestroyWindow(game->window);
		SDL_DestroyRenderer(game->renderer);
		SDL_Quit();
		return 0;
	};

	if (player->toad == NULL) {
		printf("SDL_LoadBMP(toad.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(game->charset);
		SDL_FreeSurface(game->screen);
		SDL_DestroyTexture(game->scrtex);
		SDL_DestroyWindow(game->window);
		SDL_DestroyRenderer(game->renderer);
		SDL_Quit();
		return 0;
	};

	if (game->keg == NULL) {
		printf("SDL_LoadBMP(barrel.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(game->charset);
		SDL_FreeSurface(game->screen);
		SDL_DestroyTexture(game->scrtex);
		SDL_DestroyWindow(game->window);
		SDL_DestroyRenderer(game->renderer);
		SDL_Quit();
		return 0;
	};

	if (game->platform_l == NULL) {
		printf("SDL_LoadBMP(platform_long.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(game->charset);
		SDL_FreeSurface(game->screen);
		SDL_DestroyTexture(game->scrtex);
		SDL_DestroyWindow(game->window);
		SDL_DestroyRenderer(game->renderer);
		SDL_Quit();
		return 0;
	};

	if (game->peach == NULL) {
		printf("SDL_LoadBMP(barrel.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(game->charset);
		SDL_FreeSurface(game->screen);
		SDL_DestroyTexture(game->scrtex);
		SDL_DestroyWindow(game->window);
		SDL_DestroyRenderer(game->renderer);
		SDL_Quit();
		return 0;
	};

	// IF USED IN FUTURE shorter platform
	/*if (game->platform_s == NULL) {
		printf("SDL_LoadBMP(barrel.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(game->charset);
		SDL_FreeSurface(game->screen);
		SDL_DestroyTexture(game->scrtex);
		SDL_DestroyWindow(game->window);
		SDL_DestroyRenderer(game->renderer);
		SDL_Quit();
		return 0;
	};*/

	if (game->ladder == NULL) {
		printf("SDL_LoadBMP(ladder.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(game->charset);
		SDL_FreeSurface(game->screen);
		SDL_DestroyTexture(game->scrtex);
		SDL_DestroyWindow(game->window);
		SDL_DestroyRenderer(game->renderer);
		SDL_Quit();
		return 0;
	};

	if (game->rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 0;
	};
	return 1;
}

// WYKONANO PRZEZ: ARTUR SZLEDAK S197633

int main(int argc, char** argv) {
	Game game;
	Player player;

	if (game.init()) {
		return 0;
	};

	CreateGame(STRUCT_PVALS);

	if (!ErrorCheck(STRUCT_PVALS)) {
		return 0;
	}
	
	Logic(STRUCT_PVALS);

	// frees game graphics
	SDL_FreeSurface(game.charset);
	SDL_FreeSurface(game.screen);
	SDL_DestroyTexture(game.scrtex);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);

	SDL_Quit();
	return 0;
};