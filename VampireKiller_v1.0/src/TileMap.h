#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,


	TREE_TOP1_SKY = 1, TREE_TOP2_SKY, TREE_TOP3_SKY, TREE_TOP4_SKY, TREE_TOP1, TREE_TOP2,TREE_TOP3, TREE_TOP4,
	TREE_MID1, TREE_MID2,TREE_MID3, TREE_MID4, TREE_LOW1, TREE_LOW2, TREE_LOW3, TREE_LOW4, 
	
	TREE_LOWEST1, TREE_LOWEST2, TREE_LOWEST3, TREE_LOWEST4, TREE_BOT1, TREE_BOT2, TREE_BOT3, TREE_BOT4, 
	TREE_VINES1, TREE_VINES2, TREE_VINES3, TREE_VINES4, TREE_VINES5, TREE_VINES6, TREE_EX1, TREE_EX2,

	/*STATUE & TRUNK*/
	ST_BL, ST_BR, ST_TL, ST_TR, PODIUM_L, PODIUM_R, EXTRATHATDOESNTEXIST, FAT_TRUNK, THIN_TRUNK, 
	/*fire at special tiles (4 tiles)*/ PODIUM, BARS, SHORTBARS, 


	/*outisde wall*/
	GREYWALL1, GREYWALL2, GREYWALL_L, GREAYWALL_R = 49, GREAYWALL3, TREE_WALL, BRICKS1, BRICKS2,
		/*BRICKS3,*/ CURVE1, CURVE2, CONTACTCURVE, WINDOW1, BRICKS4, BRICKS5, BRICKS6,
	RIGHTEND2, BRICKS8, OUTSIDEBRICKS, WINDOW2,LEFTEND, RIGHTEND, WINDOW3, BRICKS9L, LEFTEND2, 
		 BARTOP_L=73, BARTOP_R, BARMID_L, BARMID_R,
	BARLOW_L, BARLOW_R,


	/*INSIDE*/
	BLACKGROUND = 48, GROUND1=70, GROUND2, GROUND3, LAND = 79, STEPSUP1, STEPSUP2, STEPSDOWN, WALL1, WALLDIRTTOP_L, WALLDIRTTOP_R, WALLDIRTMID_L,
		WALLDIRTMID_R, WALLDIRTLOW_L, WALLDIRTLOW_R, WALLDIRTBOT_L, WALLDIRTBOT_R, WALLDIRT1, STEPSDOWN2, 
	WALLDIRT3, WINDOWINS, COLTOPL, COLTOPR, COLMIDL, COLMIDR, COLLOWL, COLLOWR, COLBROKELOW, COLBROKETOP,
		CURTAINTOPL, CURTAINTOPR, CURTAINMIDL, CURTAINMIDR, CURTAINLOWL, CURTAINLOWR,
	CURTAINFALL_L, CURTAINFALL_R, CURTAINFALL2L, CURTAINFALL2R, CURTAINBROKE,DIRT2, 



	// 200 <= id < 500: special tiles

	FIRE_FRAME1_L = 200, FIRE_FRAME1_R, FIRE_FRAME2_L, FIRE_FRAME2_R, SHIELD, MORNINGSTAR, DEATHBALL, CHESS, KEY_ORANGE, KEY_WHITE,

	// id >= 500: entities' initial locations
	PLAYER = 500,
	SLIME = 600,
	TURRET_LEFT = 601, TURRET_RIGHT = 602,

	//Intervals
	SOLID_FIRST =  BLACKGROUND  ,
	SOLID_LAST =  STEPSDOWN,
	OBJECT_FIRST =  FIRE_FRAME1_L,
	OBJECT_LAST =  KEY_WHITE,
	ENTITY_FIRST = PLAYER,
	ENTITY_LAST = TURRET_RIGHT
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void ClearObjectEntityPositions();

	void Update();
	void Render();
	void Release();

	bool IsTileObject(Tile tile) const;
	bool IsTileEntity(Tile tile) const;

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	
	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	bool TestOnLadder(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

	//Given a hitbox, computes the maximum swept box model along the X-axis without solid tiles
	AABB GetSweptAreaX(const AABB& hitboxbox) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool IsTileFire(Tile tile) const;
	bool IsTileDEATHBALL(Tile tile) const;
	bool IsTileSHIELD(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite* deathball;
	Sprite *fire;
	Sprite* shield;

	//Tile sheet
	const Texture2D *img_tiles;
};

