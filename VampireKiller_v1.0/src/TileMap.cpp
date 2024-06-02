#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width = 0;
	height = 0;
	fire = nullptr;
	deathball = nullptr;
	shield = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	if (fire != nullptr)
	{
		fire->Release();
		delete fire;
		fire = nullptr;
	}
	if (deathball != nullptr)
	{
		deathball->Release();
		delete deathball;
		deathball = nullptr;
	}
	if (shield != nullptr)
	{
		shield->Release();
		delete shield;
		shield = nullptr;
	}

}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::TREE_TOP1_SKY] = {	0,	  0, n, n };
	dict_rect[(int)Tile::TREE_TOP2_SKY] = {	n,	  0, n, n };
	dict_rect[(int)Tile::TREE_TOP3_SKY] = {	2*n,  0, n, n };
	dict_rect[(int)Tile::TREE_TOP4_SKY] = {	3*n,  0, n, n };
	dict_rect[(int)Tile::TREE_TOP1]		= { 4*n,  0, n, n };
	dict_rect[(int)Tile::TREE_TOP2]		= { 5*n,  0, n, n };
	dict_rect[(int)Tile::TREE_TOP3]		= { 6*n,  0, n, n };
	dict_rect[(int)Tile::TREE_TOP4]		= { 7*n,  0, n, n };
	dict_rect[(int)Tile::TREE_MID1]		= { 8*n,  0, n, n };
	dict_rect[(int)Tile::TREE_MID2]		= { 9*n,  0, n, n };
	dict_rect[(int)Tile::TREE_MID3]		= { 10*n, 0, n, n };
	dict_rect[(int)Tile::TREE_MID4]		= { 11*n, 0, n, n };
	dict_rect[(int)Tile::TREE_LOW1]		= { 12*n, 0, n, n };
	dict_rect[(int)Tile::TREE_LOW2]		= { 13*n, 0, n, n };
	dict_rect[(int)Tile::TREE_LOW3]		= { 14*n, 0, n, n };
	dict_rect[(int)Tile::TREE_LOW4]		= { 15*n, 0, n, n };

	dict_rect[(int)Tile::TREE_LOWEST1]	= { 0,		n, n, n };
	dict_rect[(int)Tile::TREE_LOWEST2]	= { n,		n, n, n };
	dict_rect[(int)Tile::TREE_LOWEST3]	= { 2*n,	n, n, n };
	dict_rect[(int)Tile::TREE_LOWEST4]	= { 3*n,	n, n, n };
	dict_rect[(int)Tile::TREE_BOT1]		= { 4*n,	n, n, n };
	dict_rect[(int)Tile::TREE_BOT2]		= { 5*n,	n, n, n };
	dict_rect[(int)Tile::TREE_BOT3]		= { 6*n,	n, n, n };
	dict_rect[(int)Tile::TREE_BOT4]		= { 7*n,	n, n, n };
	dict_rect[(int)Tile::TREE_VINES1]	= { 8*n,	n, n, n };
	dict_rect[(int)Tile::TREE_VINES2]	= { 9*n,	n, n, n };
	dict_rect[(int)Tile::TREE_VINES3]	= { 10*n,	n, n, n };
	dict_rect[(int)Tile::TREE_VINES4]	= { 11*n,	n, n, n };
	dict_rect[(int)Tile::TREE_VINES5]	= { 12*n,	n, n, n };
	dict_rect[(int)Tile::TREE_VINES6]	= { 13*n,	n, n, n };
	dict_rect[(int)Tile::TREE_EX1]		= { 14*n,	n, n, n };
	dict_rect[(int)Tile::TREE_EX2]		= { 15*n,	n, n, n };


	dict_rect[(int)Tile::ST_BL]			= { 0,		2*n, n, n };
	dict_rect[(int)Tile::ST_BR]			= { n,		2*n, n, n };
	dict_rect[(int)Tile::ST_TL]			= { 2*n,	2*n, n, n };
	dict_rect[(int)Tile::ST_TR]			= { 3*n,	2*n, n, n };
	dict_rect[(int)Tile::PODIUM_L]		= { 4*n,	2*n, n, n };
	dict_rect[(int)Tile::PODIUM_R]		= { 5*n,	2*n, n, n };
	dict_rect[(int)Tile::LAND]			= {	6*n,	2*n, n, n };
	dict_rect[(int)Tile::FAT_TRUNK]		= { 7*n,	2*n, n, n };
	dict_rect[(int)Tile::THIN_TRUNK]	= { 8*n,	2*n, n, n };
	dict_rect[(int)Tile::FIRE_FRAME1_L] = { 9*n,	2*n, n, n };
	dict_rect[(int)Tile::FIRE_FRAME1_R] = { 10*n,	2*n, n, n };
	dict_rect[(int)Tile::FIRE_FRAME2_L] = { 11*n,	2*n, n, n };
	dict_rect[(int)Tile::FIRE_FRAME2_R] = { 12*n,	2*n, n, n };
	dict_rect[(int)Tile::PODIUM]		= { 13*n,	2*n, n, n };
	dict_rect[(int)Tile::BARS]			= { 14*n,	2*n, n, n };
	dict_rect[(int)Tile::SHORTBARS]		= { 15*n,	2*n, n, n };

	dict_rect[(int)Tile::GREYWALL1]		= { 0,		3*n, n, n };
	dict_rect[(int)Tile::GREYWALL2]		= { n,		3*n, n, n };
	dict_rect[(int)Tile::GREYWALL_L]	= { 2*n,	3*n, n, n };
	dict_rect[(int)Tile::BLACKGROUND]	= { 3*n,	3*n, n, n };
	dict_rect[(int)Tile::GREAYWALL_R]	= { 4*n,	3*n, n, n };
	dict_rect[(int)Tile::GREAYWALL3]	= { 5*n,	3*n, n, n };
	dict_rect[(int)Tile::TREE_WALL]		= {	6*n,	3*n, n, n };
	dict_rect[(int)Tile::BRICKS1]		= { 7*n,	3*n, n, n };
	dict_rect[(int)Tile::BRICKS2]		= { 8*n,	3*n, n, n };
	dict_rect[(int)Tile::CURVE1]		= { 9*n,	3*n, n, n };
	dict_rect[(int)Tile::CURVE2]		= { 10*n,	3*n, n, n };
	dict_rect[(int)Tile::CONTACTCURVE]	= { 11*n,	3*n, n, n };
	dict_rect[(int)Tile::WINDOW1]		= { 12*n,	3*n, n, n };
	dict_rect[(int)Tile::BRICKS4]		= { 13*n,	3*n, n, n };
	dict_rect[(int)Tile::BRICKS5]		= { 14*n,	3*n, n, n };
	dict_rect[(int)Tile::BRICKS6]		= { 15*n,	3*n, n, n };


	dict_rect[(int)Tile::RIGHTEND2]			= { 0,		4*n, n, n };
	dict_rect[(int)Tile::BRICKS8]			= { n,		4*n, n, n };
	dict_rect[(int)Tile::OUTSIDEBRICKS]		= { 2*n,	4*n, n, n };
	dict_rect[(int)Tile::WINDOW2]			= { 3*n,	4*n, n, n };
	dict_rect[(int)Tile::LEFTEND]			= { 4*n,	4*n, n, n };
	dict_rect[(int)Tile::RIGHTEND]			= { 5*n,	4*n, n, n };
	dict_rect[(int)Tile::WINDOW3]			= {	6*n,	4*n, n, n };
	dict_rect[(int)Tile::BRICKS9L]			= { 7*n,	4*n, n, n };
	dict_rect[(int)Tile::LEFTEND2]			= { 8*n,	4*n, n, n };
	dict_rect[(int)Tile::GROUND1]			= { 9*n,	4*n, n, n };
	dict_rect[(int)Tile::GROUND2]			= { 10*n,	4*n, n, n };
	dict_rect[(int)Tile::GROUND3]			= { 11*n,	4*n, n, n };
	dict_rect[(int)Tile::BARTOP_L]			= { 12*n,	4*n, n, n };
	dict_rect[(int)Tile::BARTOP_R]			= { 13*n,	4*n, n, n };
	dict_rect[(int)Tile::BARMID_L]			= { 14*n,	4*n, n, n };
	dict_rect[(int)Tile::BARMID_R]			= { 15*n,	4*n, n, n };


	dict_rect[(int)Tile::BARLOW_L]			= { 0,		5 * n, n, n };
	dict_rect[(int)Tile::BARLOW_R]			= { n,		5 * n, n, n };
	dict_rect[(int)Tile::WALL1]				= { 2 * n,	5 * n, n, n };
	dict_rect[(int)Tile::STEPSUP1]			= { 3 * n,	5 * n, n, n };
	dict_rect[(int)Tile::STEPSUP2]			= { 4 * n,	5 * n, n, n };
	dict_rect[(int)Tile::STEPSDOWN]			= { 5 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTTOP_L]		= { 6 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTTOP_R]		= { 7 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTMID_L]		= { 8 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTMID_R]		= { 9 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTLOW_L]		= { 10 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTLOW_R]		= { 11 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTBOT_L]		= { 12 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRTBOT_R]		= { 13 * n,	5 * n, n, n };
	dict_rect[(int)Tile::WALLDIRT1]			= { 14 * n,	5 * n, n, n };
	dict_rect[(int)Tile::STEPSDOWN2]		= { 15 * n,	5 * n, n, n };

	dict_rect[(int)Tile::WALLDIRT3]			= { 0,		6 * n, n, n };
	dict_rect[(int)Tile::WINDOWINS]			= { n,		6 * n, n, n };
	dict_rect[(int)Tile::COLTOPL]			= { 2 * n,	6 * n, n, n };
	dict_rect[(int)Tile::COLTOPR]			= { 3 * n,	6 * n, n, n };
	dict_rect[(int)Tile::COLMIDL]			= { 4 * n,	6 * n, n, n };
	dict_rect[(int)Tile::COLMIDR]			= { 5 * n,	6 * n, n, n };
	dict_rect[(int)Tile::COLLOWL]			= { 6 * n,	6 * n, n, n };
	dict_rect[(int)Tile::COLLOWR]			= { 7 * n,	6 * n, n, n };
	dict_rect[(int)Tile::COLBROKELOW]		= { 8 * n,	6 * n, n, n };
	dict_rect[(int)Tile::COLBROKETOP]		= { 9 * n,	6 * n, n, n };
	dict_rect[(int)Tile::CURTAINTOPL]		= { 10 * n,	6 * n, n, n };
	dict_rect[(int)Tile::CURTAINTOPR]		= { 11 * n,	6 * n, n, n };
	dict_rect[(int)Tile::CURTAINMIDL]		= { 12 * n,	6 * n, n, n };
	dict_rect[(int)Tile::CURTAINMIDR]		= { 13 * n,	6 * n, n, n };
	dict_rect[(int)Tile::CURTAINLOWL]		= { 14 * n,	6 * n, n, n };
	dict_rect[(int)Tile::CURTAINLOWR]		= { 15 * n,	6 * n, n, n };

	dict_rect[(int)Tile::CURTAINFALL_L]		= { 0,		7 * n, n, n };
	dict_rect[(int)Tile::CURTAINFALL_R]		= { n,		7 * n, n, n };
	dict_rect[(int)Tile::CURTAINFALL2L]		= { 2 * n,	7 * n, n, n };
	dict_rect[(int)Tile::CURTAINFALL2R]		= { 3 * n,	7 * n, n, n };
	dict_rect[(int)Tile::CURTAINBROKE]		= { 4 * n,	7 * n, n, n };
	dict_rect[(int)Tile::DIRT2]				= { 5 * n,	7 * n, n, n };
	dict_rect[(int)Tile::SHIELD]			= { 6 * n,	7 * n, n, n };
	dict_rect[(int)Tile::MORNINGSTAR]		= { 7 * n,	7 * n, n, n };
	dict_rect[(int)Tile::DEATHBALL]			= { 8 * n,	7 * n, n, n };
	dict_rect[(int)Tile::CHESS]				= { 9 * n,	7 * n, n, n };
	dict_rect[(int)Tile::KEY_ORANGE]		= { 10* n,	7 * n, n, n };
	dict_rect[(int)Tile::KEY_WHITE]			= { 11* n,	7 * n, n, n };


}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/VK_Tiles.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);

	fire = new Sprite(img_tiles);
	if (fire == nullptr)
	{
		LOG("Failed to allocate memory for fire sprite");
		return AppStatus::ERROR;
	}

	fire->SetNumberAnimations(2);
	fire->SetAnimationDelay(0, ANIM_DELAY);
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME1_L]);
	fire->AddKeyFrame(0, dict_rect[(int)Tile::FIRE_FRAME2_L]);
	fire->SetAnimation(0);


	deathball = new Sprite(img_tiles);
	if (deathball == nullptr)
	{
		LOG("Failed to allocate memory for deathball sprite");
		return AppStatus::ERROR;
	}

	shield = new Sprite(img_tiles);
	if (shield == nullptr)
	{
		LOG("Failed to allocate memory for shield sprite");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}
void TileMap::ClearObjectEntityPositions()
{
	int i;
	Tile tile;
	for (i = 0; i < size; ++i)
	{
		tile = map[i];
		if(IsTileEntity(tile) || IsTileObject(tile) || tile == Tile::EMPTY)
			map[i] = Tile::AIR;
	}
}
void TileMap::Update()
{
	deathball->Update();
	fire->Update();
	shield->Update();
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::OBJECT_FIRST <= tile && tile <= Tile::OBJECT_LAST;
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
bool TileMap::IsTileSolid(Tile tile) const
{
	return Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST;
}
bool TileMap::IsTileLadderTop(Tile tile) const
{
	return tile == Tile::GROUND1 || tile == Tile::GROUND2 || tile == Tile::GROUND3;
}
bool TileMap::IsTileLadder(Tile tile) const
{
	return tile == Tile::STEPSUP1 || tile == Tile::STEPSDOWN;
}
bool TileMap::IsTileFire(Tile tile) const
{
	return tile == Tile::FIRE_FRAME1_L || tile == Tile::FIRE_FRAME2_L;
}
bool TileMap::IsTileSHIELD(Tile tile) const
{
	return tile == Tile::SHIELD;
}
bool TileMap::IsTileDEATHBALL(Tile tile) const
{
	return tile == Tile::DEATHBALL;
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionGround(const AABB& box, int *py) const
{
	Point p(box.pos.x, *py);	//control point
	int tile_y;

	if (CollisionY(p, box.width))
	{
		tile_y = p.y / TILE_SIZE;

		*py = tile_y * TILE_SIZE - 1;
		return true;
	}
	return false;
}
bool TileMap::TestFalling(const AABB& box) const
{
	return !CollisionY(box.pos + Point(0, box.height), box.width);
}
bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile) || IsTileLadderTop(tile))
			return true;
	}
	return false;
}
bool TileMap::TestOnLadder(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;
	
	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width-1;
	bottom = box.pos.y + box.height-1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	if (IsTileLadder(tile1) && IsTileLadder(tile2))
	{
		*px = GetLadderCenterPos(left, bottom) - box.width/2;
		return true;
	}
	return false;
}
bool TileMap::TestOnLadderTop(const AABB& box, int* px) const
{
	int left, right, bottom;
	int tx1, tx2, ty;
	Tile tile1, tile2;

	//Control points
	left = box.pos.x;
	right = box.pos.x + box.width - 1;
	bottom = box.pos.y + box.height - 1;

	//Calculate the tile coordinates
	tx1 = left / TILE_SIZE;
	tx2 = right / TILE_SIZE;
	ty = bottom / TILE_SIZE;

	//To be able to climb up or down, both control points must be on ladder
	tile1 = GetTileIndex(tx1, ty);
	tile2 = GetTileIndex(tx2, ty);
	if (IsTileLadderTop(tile1) && IsTileLadderTop(tile2))
	{
		*px = GetLadderCenterPos(left, bottom) - box.width / 2;
		return true;
	}
	return false;
}
int TileMap::GetLadderCenterPos(int pixel_x, int pixel_y) const
{
	int tx, ty;
	
	tx = pixel_x / TILE_SIZE;
	ty = pixel_y / TILE_SIZE;
	Tile tile = GetTileIndex(tx, ty);

	if (tile == Tile::BARLOW_L || tile == Tile::BARLOW_R)		return tx * TILE_SIZE + TILE_SIZE;
	else if (tile == Tile::BARTOP_L || tile == Tile::BARTOP_R)	return tx * TILE_SIZE;
	else
	{
		LOG("Internal error, tile should be a LADDER, coord: (%d,%d), tile type: %d", pixel_x, pixel_y, (int)tile);
		return 0;
	}
}
AABB TileMap::GetSweptAreaX(const AABB& hitbox) const
{
	AABB box;
	int column, x, y, y0, y1;
	bool collision;
	
	box.pos.y = hitbox.pos.y;
	box.height = hitbox.height;

	column = hitbox.pos.x / TILE_SIZE;
	y0 = hitbox.pos.y / TILE_SIZE;
	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;

	//Compute left tile index
	collision = false;
	x = column - 1;
	while (!collision && x >= 0)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if(!collision) x--;
	}
	box.pos.x = (x+1)*TILE_SIZE;

	//Compute right tile index
	collision = false;
	x = column + 1;
	while (!collision && x < LEVEL_WIDTH)
	{
		//Iterate over the tiles within the vertical range
		for (y = y0; y <= y1; ++y)
		{
			//One solid tile is sufficient
			if (IsTileSolid(GetTileIndex(x, y)))
			{
				collision = true;
				break;
			}
		}
		if (!collision) x++;
	}
	box.width = x*TILE_SIZE - box.pos.x;
	
	return box;
}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

				if (tile != Tile::FIRE_FRAME1_L && tile != Tile::DEATHBALL)
				{
					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				}
				else if (tile == Tile::DEATHBALL) {
					deathball->Draw((int)pos.x, (int)pos.y);
				}
				else if (tile == Tile::SHIELD) {
					shield->Draw((int)pos.x, (int)pos.y);
				}
				else
				{
					fire->Draw((int)pos.x, (int)pos.y);
				}
			}
		}
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	deathball->Release();
	fire->Release();
	shield->Release();

	dict_rect.clear();
}