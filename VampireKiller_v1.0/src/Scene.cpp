#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

int currentStage = 0;
const int n = TILE_SIZE;


Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
	font1 = nullptr;

	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					//No rotation
	camera.zoom = 1.0f;						//Default zoom

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	if (font1 != nullptr)
	{
		delete font1;
		font1 = nullptr;
	}
}
AppStatus Scene::Init()
{

	//Create player
	player = new Player({ n,12*n }, State::IDLE, Look::RIGHT);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	
	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}


	//Assign the tile map reference to the player to check collisions while navigating
	player->SetTileMap(level);

	//Create text font 1
	font1 = new Text();
	if (font1 == nullptr)
	{
		LOG("Failed to allocate memory for font 1");
		return AppStatus::ERROR;
	}
	//Initialise text font 1
	if (font1->Initialise(Resource::IMG_FONT1, "images/font8x8.png", ' ', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Point pos;
	Tile tile;
	int* map = nullptr;
	int* submap = nullptr;
	Object *obj;
	AABB hitbox, area;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		currentStage = 1;
		map = new int[size] {
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  1,   2,   3,   4,    1,   2,   3,     4,  1,   2,   3,   4,   1,   2,   3,   4, 48,
			48,  5,   6,   7,   8,    5,   6,   7,     8,  5,   6,   7,   8,   5,   6,   7,   8, 48,
			48,  9,  10,  11,  12,    9,  10,  11,    12,  9,  10,  11,  12,   9,  10,  11,  12, 48,
			48, 13,  14,  15,  16,   13,  14,  15,    16, 13,  14,  15,  16,  13,  14,  15,  16, 48,
			48, 17,  18,  19,  20,   17,  18,  19,    20, 17,  18,  19,  20,  17,  18,  19,  20, 48,
			48, 21,	 31,  29,  29,	 32,  22,  23,    24, 21,  22,  23,  24,  32,  22,  23,  24, 48,
			48, 25,	 26,  35,  36,	 28,  27,  25,    27, 25,  27,  25,  29,  28,  27,  25,  27, 48,
			48, 40,	 44,  33,  34,	 44,  44,  41,    44, 41,  44,  41,  44,  44,  44,  41,  44, 48,
			48, 43,  43,  37,  38,	 42,  43,  43,    43, 43,  43,  43,  43,  42,  43,  43,  43, 48,
			48, 79,  79,  79,  79,	 79,  79,  79,    79, 79,  79,  79,  79,  79,  79,  79,  79, 48,
			48,  0,   0,   0,   0,	  0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
		};
		submap = new int[size] {
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  1,   2,   3,   4,    1,   2,   3,     4,  1,   2,   3,   4,   1,   2,   3,   4, 48,
			48,  5,   6,   7,   8,    5,   6,   7,     8,  5,   6,   7,   8,   5,   6,   7,   8, 48,
			48,  9,  10,  11,  12,    9,  10,  11,    12,  9,  10,  11,  12,   9,  10,  11,  12, 48,
			48, 13,  14,  15,  16,   13,  14,  15,    16, 13,  14,  15,  16,  13,  14,  15,  16, 48,
			48, 17,  18,  19,  20,   17,  18,  19,    20, 17,  18,  19,  20,  17,  18,  19,  20, 48,
			48, 21,	 31,  29,  29,	 32,  22,  23,    24, 21,  22,  23,  24,  32,  22,  23,  24, 48,
			48, 25,	 26,  35,  36,	 28,  27,  25,    27, 25,  27,  25,  29,  28,  27,  25,  27, 48,
			48, 40,	 44,  33,  34,	200,  44,  41,    44, 41,  44,  41,  44, 200,  44,  41,  44, 48,
			48, 43,  43,  37,  38,	 42,  43, 204,    43, 43, 206,  43,  43,  42,  43,  43,  43, 48,
			48, 79,  79,  79,  79,	 79,  79,  79,    79, 79,  79,  79,  79,  79,  79,  79,  79, 48,
			48,  0,   0,   0,   0,	  0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		currentStage = 2;
		map = new int[size] {
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  1,   2,   3,   4,    1,   2,   3,     4,  1,   2,   3,   4,   1,   2,   3,   4, 48,
			48,  5,   6,   7,   8,    5,   6,   7,     8,  5,   6,   7,   8,   5,   6,   7,   8, 48,
			48,  9,  10,  11,  12,    9,  10,  11,    12,  9,  10,  11,  12,   9,  10,  11,  12, 48,
			48, 13,  14,  15,  16,   13,  14,  15,    16, 13,  14,  15,  16,  13,  14,  15,  16, 48,
			48, 17,  18,  19,  20,   17,  18,  19,    20, 17,  18,  19,  20,  17,  18,  19,  20, 48,
			48, 21,	 31,  29,  29,	 32,  22,  23,    24, 21,  22,  23,  24,  32,  22,  23,  24, 48,
			48, 25,	 26,  35,  36,	 28,  27,  25,    27, 25,  27,  25,  29,  28,  27,  25,  27, 48,
			48, 40,	 44,  33,  34,	 44,  44,  41,    44, 41,  44,  41,  44,  44,  44,  41,  44, 48,
			48, 43,  43,  37,  38,	 42,  43,  43,    43, 43,  43,  43,  43,  42,  43,  43,  43, 48,
			48, 79,  79,  79,  79,	 79,  79,  79,    79, 79,  79,  79,  79,  79,  79,  79,  79, 48,
			48,  0,   0,   0,   0,	  0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0, 48
		};
				submap = new int[size] {
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  1,   2,   3,   4,    1,   2,   3,     4,  1,   2,   3,   4,   1,   2,   3,   4, 48,
			48,  5,   6,   7,   8,    5,   6,   7,     8,  5,   6,   7,   8,   5,   6,   7,   8, 48,
			48,  9,  10,  11,  12,    9,  10,  11,    12,  9,  10,  11,  12,   9,  10,  11,  12, 48,
			48, 13,  14,  15,  16,   13,  14,  15,    16, 13,  14,  15,  16,  13,  14,  15,  16, 48,
			48, 17,  18,  19,  20,   17,  18,  19,    20, 17,  18,  19,  20,  17,  18,  19,  20, 48,
			48, 21,	 31,  29,  29,	 32,  22,  23,    24, 21,  22,  23,  24,  32,  22,  23,  24, 48,
			48, 25,	 26,  35,  36,	 28,  27,  25,    27, 25,  27,  25,  29,  28,  27,  25,  27, 48,
			48, 40,	 44,  33,  34,	200,  44,  41,    44, 41,  44,  41,  44, 200,  44,  41,  44, 48,
			48, 43,  43,  37,  38,	 42,  43,  43,    43, 43,  43,  43,  43,  42,  43,  43,  43, 48,
			48, 79,  79,  79,  79,	 79,  79,  79,    79, 79,  79,  79,  79,  79,  79,  79,  79, 48,
			48,  0,   0,   0,   0,	  0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
		};
	}

				/*		entry mansion		*/
	else if (stage == 3)
	{
		currentStage = 3;
		map = new int[size] {
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  1,   2,   3,   4,    1,   2,   3,     4,  1,   2,  51,  52,  53,  52,  53,  52, 48,
			48,  5,   6,   7,   8,    5,   6,   7,     8,  5,   6,  63,  62,  60,  60,  56,  62, 48,
			48,  9,  10,  11,  12,    9,  10,  11,    12,  9,  10,  63,  64,  54,  55,  56,  64, 48,
			48, 13,  14,  15,  16,   13,  14,  15,    16, 13,  14,  63,  62,  69,  48,  61,  62, 48,
			48, 17,  18,  19,  20,   17,  18,  19,    20, 17,  18,  63,  64,  65,  48,  66,  64, 48,
			48, 21,	 31,  29,  29,	 32,  22,  23,    24, 21,  22,  63,  62,  69,  48,  61,  62, 48,
			48, 25,	 26,  35,  36,	 28,  27,  25,    27, 25,  27,  63,  64,  65,  48,  66,	 64, 48,
			48, 40,	 44,  33,  34,	 44,  44,  41,    44, 41,  44,  45,  46,  47,  48,  49,  50, 48,
			48, 43,  43,  37,  38,	 42,  43,  43,    43, 43,  43,  45,  46,  47,  48,  49,  50, 48,
			48, 79,  79,  79,  79,	 79,  79,  79,    79, 79,  79,  79,  79,  79,  79,  79,  79, 48,
			48,  0,   0,   0,   0,	  0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0, 48
		};
		submap = new int[size] {
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  1,   2,   3,   4,    1,   2,   3,     4,  1,   2,  51,  52,  53,  52,  53,  52, 48,
			48,  5,   6,   7,   8,    5,   6,   7,     8,  5,   6,  63,  62,  60,  60,  56,  62, 48,
			48,  9,  10,  11,  12,    9,  10,  11,    12,  9,  10,  63,  64,  54,  55,  56,  64, 48,
			48, 13,  14,  15,  16,   13,  14,  15,    16, 13,  14,  63,  62,  69,  48,  61,  62, 48,
			48, 17,  18,  19,  20,   17,  18,  19,    20, 17,  18,  63,  64,  65,  48,  66,  64, 48,
			48, 21,	 31,  29,  29,	 32,  22,  23,    24, 21,  22,  63,  62,  69,  48,  61,  62, 48,
			48, 25,	 26,  35,  36,	 28,  27,  25,    27, 25,  27,  63,  64,  65,  48,  66,	 64, 48,
			48, 40,	 44,  33,  34,	 44,  44,  41,    44, 41,  44,  45,  46,  47,  48,  49,  50, 48,
			48, 43,  43,  37,  38,	 42,  43,  43,    43, 43,  43,  45,  46,  47,  48,  49,  50, 48,
			48, 79,  79,  79,  79,	 79,  79,  79,    79, 79,  79,  79,  79,  79,  79,  79,  79, 48,
			48,  0,   0,   0,   0,	  0,   0,   0,    0,   0,   0,   0,   0,   0,   0,   0,   0, 48
		};
	}

				/*		inside mansion		*/
	
	else if (stage == 4)
	{
		currentStage = 4;
		map = new int[size] {
			48,  0,   0,   0,   0,	  0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48, 96,  97, 104, 105,   83,  83,  86,    87, 96,  97, 108, 109,  83,  83,  90,  91, 48,
			48, 98,  99, 106, 107,   83,  83,  88,    89, 98,  99, 113,  83,  83,  83,  83,  83, 48,
			48, 98,  99, 108, 109,   95,  95,  90,    91, 98,  99, 114,  83,  95,  95,  83,  83, 48,
			48, 98,  99, 110,  83,   95,  95,  83,    83, 98,  99,  84,  85,  95,  95,  83,  83, 48,
			48, 98,  99, 112,  83,   95,  95,  83,    83, 98,  99,  86,  87,  95,  95,  83,  83, 48,
			48, 98,  99, 112,  83,   95,  95,  83,    83, 98,  99,  88,  89,  95,  95,  83,  83, 48,
			48, 98,	 99, 112,  83,	 95,  95,  83,    83, 98,  99,  90,  91,  95,  95,  83,  83, 48,
			48, 98,	 99, 113,  83,	 95,  95,  83,    83, 98,  99,  83,  83,  95,  95,  83,  83, 48,
			48, 98,	 99, 114,  83,	 95,  95,  83,    83, 98,  99,  83,  83,  95,  95,  83,  83, 48,
			48,100, 101,  83,  83,	 83,  83,  83,    83,100, 101,  83,  83,  83,  83,  83,  83, 48,
			48, 70,  71,  70,  71,	 70,  71,  70,    71, 70,  71,  70,  71,  70,  71,  70,  71, 48
		};
		submap = new int[size] {
			48,  0,   0,   0,   0,	  0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,     0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48, 96,  97, 104, 105,   83,  83,  86,    87, 96,  97, 108, 109,  83,  83,  90,  91, 48,
			48, 98,  99, 106, 107,   83,  83,  88,    89, 98,  99, 113,  83,  83,  83,  83,  83, 48,
			48, 98,  99, 108, 109,   95,  95,  90,    91, 98,  99, 114,  83,  95,  95,  83,  83, 48,
			48, 98,  99, 110,  83,   95,  95,  83,    83, 98,  99,  84,  85,  95,  95,  83,  83, 48,
			48, 98,  99, 112,  83,   95,  95,  83,    83, 98,  99,  86,  87,  95,  95,  83,  83, 48,
			48, 98,  99, 112,  83,   95,  95,  83,    83, 98,  99,  88,  89,  95,  95,  83,  83, 48,
			48, 98,	 99, 112,  83,	 95,  95,  83,    83, 98,  99,  90,  91,  95,  95,  83,  83, 48,
			48, 98,	 99, 113,  83,	 95,  95,  83,    83, 98,  99,  83,  83,  95,  95,  83,  83, 48,
			48, 98,	 99, 114,  83,	 95,  95,  83,    83, 98,  99,  83,  83,  95,  95,  83,  83, 48,
			48,100, 101,  83,  83,	 83,  83,  83,    83,100, 101,  83,  83,  83,  83,  83,  83, 48,
			48, 70,  71,  70,  71,	 70,  71,  70,    71, 70,  71,  70,  71,  70,  71,  70,  71, 48
		};
	}

		/*	BACKWARDS AT 4	*/
	else if (stage == 5)
	{
		currentStage = 5;
		map = new int[size] {
			48,  0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48, 70,  71,  83,  83,  96,  97,  83,  83, 83,  83,  83,  83,  90,  91,  96,  97, 48,
			48, 70,  71,  83,  83,  98,  99,  83,  83, 83,  83,  83,  83,  84,  85,  98,  99, 48,
			48, 70,  71,  70,  71,  98,  99,  70,  71, 72,  82,  83,  83,  86,  87,  98,  99, 48,
			48, 70,  71,  83,  83,  98,  99,  88,  89, 83,  93,  82,  83,  88,  89,  98,  99, 48,
			48, 70,  71,  83,  83,  98,  99,  90,  91, 83, 115,  93,  82,  90,  91,  98,  99, 48,
			48, 70,  71,  83,  83,  98,  99,  83,  83, 84,  85,  83,  93,  82,  83,  98,  99, 48,
			48, 70,	 71,  70,  71,  70,  71,  70,  71, 86,  87,  83, 115,  70,  71,  70,  71, 48,
			48, 70,	 71,  88,  89,  96,  97,  83,  83, 88,  89,  83,  83,  83,  83,  96,  97, 48,
			48, 70,	 71,  90,  91,  98,  99,  83,  83, 90,  91,  83,  83,  83,  83,  98,  99, 48,
			48, 70,  71,  83,  83, 100, 101,  83,  83, 83,  83,  83,  83,  83,  83, 100, 101, 48,
			48, 70,  71,  70,  71,  70,  71,  70,  71, 70,  71,  70,  71,  70,  71,  70,  71, 48
		};
		submap = new int[size] {
			48,  0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48, 70,  71,  83,  83,  96,  97,  83,  83, 83,  83,  83,  83,  90,  91,  96,  97, 48,
			48, 70,  71,  83,  83,  98,  99,  83,  83, 83,  83,  83,  83,  84,  85,  98,  99, 48,
			48, 70,  71,  70,  71,  98,  99,  70,  71, 72,  82,  83,  83,  86,  87,  98,  99, 48,
			48, 70,  71,  83,  83,  98,  99,  88,  89, 83,  93,  82,  83,  88,  89,  98,  99, 48,
			48, 70,  71,  83,  83,  98,  99,  90,  91, 83, 115,  93,  82,  90,  91,  98,  99, 48,
			48, 70,  71,  83, 119,  98,  99,  83,  83, 84,  85,  83,  93,  82,  83,  98,  99, 48,
			48, 70,	 71,  70,  71,  70,  71,  70,  71, 86,  87,  83, 115,  70,  71,  70,  71, 48,
			48, 70,	 71,  88,  89,  96,  97,  83,  83, 88,  89,  83,  83,  83,  83,  96,  97, 48,
			48, 70,	 71,  90,  91,  98,  99,  83,  83, 90,  91,  83,  83,  83,  83,  98,  99, 48,
			48, 70,  71, 120,  83, 100, 101,  83,  83, 83,  83,  83,  83,  83,  83, 100, 101, 48,
			48, 70,  71,  70,  71,  70,  71,  70,  71, 70,  71,  70,  71,  70,  71,  70,  71, 48
		};
	}

	/* FORAWRD AT 4 */
		
	else if (stage == 6)
	{
		currentStage = 6;
		map = new int[size] {
			48,  0,   0,   0,   0,	  0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48, 96,  97, 104, 105,   83,  83,  86,  87, 83, 115,  93,  82,  83,  83,  90,  91, 48,
			48, 98,  99, 106, 107,   95,  95,  95,  95, 83,  83,  83,  93,  82,  83,  83,  83, 48,
			48, 98,  99, 108, 109,   95,  95,  95,  95, 83,  83,  83, 115,  93,  82,  83,  83, 48,
			48, 98,  99, 110,  83,   95,  95,  95,  95, 83,  83,  83,  83,  83,  93,  82,  83, 48,
			48, 98,  99, 112,  83,   95,  95,  95,  95, 83,  83,  80,  72,  70,  71,  70,  71, 48,
			48, 98,  99, 112,  83,   88,  89,  83,  83, 83,  80,  81,  83,  96,  97,  84,  85, 48,
			48, 98,	 99, 112,  83,	 90,  91,  80,  72, 70,  71,  92,  83,  98,  99,  86,  87, 48,
			48, 98,	 99, 113,  83,	 83,  80,  81,  83, 83,  83,  83,  83,  98,  99,  88,  89, 48,
			48,103,	 99, 114,  83,	 80,  81,  92,  83, 83,  83,  83,  83,  98,  99,  90,  91, 48,
			48,100, 101,  83,  80,	 81,  83,  83,  83, 83,  83,  83,  83,  98,  99,  83,  83, 48,
			48, 70,  71,  70,  71,	 70,  71,  70,  71, 70,  71,  70,  71,  70,  71,  70,  71, 48
		};
		submap = new int[size] {
			48,  0,   0,   0,   0,	  0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48,  0,   0,   0,   0,    0,   0,   0,   0,  0,   0,   0,   0,   0,   0,   0,   0, 48,
			48, 96,  97, 104, 105,   83,  83,  86,  87, 83, 115,  93,  82,  83,  83,  90,  91, 48,
			48, 98,  99, 106, 107,   95,  95,  95,  95, 83,  83,  83,  93,  82,  83,  83,  83, 48,
			48, 98,  99, 108, 109,   95,  95,  95,  95, 83,  83,  83, 115,  93,  82,  83,  83, 48,
			48, 98,  99, 110,  83,   95,  95,  95,  95, 83,  83,  83,  83,  83,  93,  82,  83, 48,
			48, 98,  99, 112,  83,   95,  95,  95,  95, 83,  83,  80,  72,  70,  71,  70,  71, 48,
			48, 98,  99, 112,  83,   88,  89,  83,  83, 83,  80,  81,  83,  96,  97,  84,  85, 48,
			48, 98,	 99, 112,  83,	 90,  91,  80,  72, 70,  71,  92,  83,  98,  99,  86,  87, 48,
			48, 98,	 99, 113,  83,	 83,  80,  81,  83, 83,  83,  83,  83,  98,  99,  88,  89, 48,
			48,103,	 99, 114,  83,	 80,  81,  92,  83, 83,  83,  83,  83,  98,  99,  90,  91, 48,
			48,100, 101,  83,  80,	 81,  83,  83,  83, 83,  83,  83,  83,  98,  99,  83,  83, 48,
			48, 70,  71,  70,  71,	 70,  71,  70,  71, 70,  71,  70,  71,  70,  71,  70,  71, 48
		};
	}


	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}
	
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);
	//level->Load(submap, LEVEL_WIDTH, LEVEL_HEIGHT);

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)submap[i];
			if (level->IsTileEntity(tile) || level->IsTileObject(tile))
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
			
				if (tile == Tile::PLAYER)
				{
					player->SetPos(pos);
				}
				else if (tile == Tile::FIRE_FRAME1_L)
				{
					obj = new Object(pos, ObjectType::FIRE);
					objects.push_back(obj);			
				}
				else if (tile == Tile::SHIELD)
				{
					obj = new Object(pos, ObjectType::SHIELD);
					objects.push_back(obj);
				}
				else if (tile == Tile::DEATHBALL) {
					obj = new Object(pos, ObjectType::DEATHBALL);
					objects.push_back(obj);
					int aux = obj->EndGameCheck();
					//Scene::EndGame(aux);
				}
				else
				{
					LOG("Internal error loading scene: invalid entity or object tile id")
				}
			}
			++i;
		}
	}
	
	//Remove initial positions of objects and entities from the map
	level->ClearObjectEntityPositions();
	
	delete[] map;

	return AppStatus::OK;
}

void Scene::Update()
{
	Point p1, p2;
	AABB hitbox;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);
	}
	//Debug levels instantly
	if (IsKeyPressed(KEY_ONE)) {
		player->SetPos({ n, 12 * n });
		LoadLevel(1);
	}
	else if (IsKeyPressed(KEY_TWO)) {
		player->SetPos({ n, 12 * n });
		LoadLevel(2);
	}
	else if (IsKeyPressed(KEY_THREE)) {
		player->SetPos({ n, 12 * n });
		LoadLevel(3);
	}
	else if (IsKeyPressed(KEY_FOUR)) {
		player->SetPos({ n, 13 * n });
		LoadLevel(4);
	}
	else if (IsKeyPressed(KEY_FIVE)) {
		player->SetPos({ REAL_WINDOW_WIDTH - n, 13 * n });
		LoadLevel(5);
	}
	else if (IsKeyPressed(KEY_SIX)) {
		player->SetPos({ n, 13 * n });
		LoadLevel(6);
	}

	/*		NEXT STAGE		*/
	int yPos = player->GetYPos();

	if ((currentStage >= 1) && (currentStage < 3) && (player->GetXPos() == REAL_WINDOW_WIDTH)) {
		player->SetPos({n+1, yPos});
		LoadLevel(currentStage + 1);
	}
	else if (currentStage == 3 && player->GetXPos() == REAL_WINDOW_WIDTH - 4*n) {

		player->SetPos({ n+1, 13*n });
		LoadLevel(currentStage + 1);
	}

	else if (currentStage >= 4 && player->GetXPos() == REAL_WINDOW_WIDTH) {
		if (currentStage == 4) {
			player->SetPos({ n+1, yPos });
			LoadLevel(6);
		}
		else if (currentStage == 5) {
			player->SetPos({ n + 1, yPos });
			LoadLevel(4);
		}
		else {
			player->SetPos({ n+1, yPos });
			LoadLevel(currentStage + 1);
		}
	}

	/*		PREV STAGE		*/
	else if ((currentStage >= 2) && (currentStage < 4) && (player->GetXPos() == n)) {
		player->SetPos({ REAL_WINDOW_WIDTH - 1, yPos });
		LoadLevel(currentStage - 1);
	}
	else if (currentStage >= 4 && player->GetXPos() == n) {
		if (currentStage == 4) {
			player->SetPos({ REAL_WINDOW_WIDTH - n, yPos });
			LoadLevel(5);
		}
		else if (currentStage == 6) {
			player->SetPos({ REAL_WINDOW_WIDTH - n, yPos });
			LoadLevel(4);
		}  
	}

	level->Update();
	player->Update();
	CheckObjectCollisions();

	hitbox = player->GetHitbox();

}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects();
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
}
void Scene::CheckObjectCollisions()
{
	AABB player_box, obj_box;

	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if (player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());

			//Delete the object
			delete* it;
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it);
		}
		else
		{
			//Move to the next object
			++it;
		}
	}
}

void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	static int frame;
	frame++;
	frame%=1000;
	font1->Draw( 100,  15, TextFormat("SCORE:%d", player->GetScore()));
}