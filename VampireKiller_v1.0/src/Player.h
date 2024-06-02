#pragma once
#include "Entity.h"
#include "TileMap.h"

//Representation model size: 16x32
#define PLAYER_FRAME_SIZE_X		16
#define PLAYER_FRAME_SIZE_Y		32


//Logical model size: 12x28
#define PLAYER_PHYSICAL_WIDTH	12
#define PLAYER_PHYSICAL_HEIGHT	28

//Horizontal speed and vertical speed while falling down
#define PLAYER_SPEED			1

//Vertical speed while on a ladder
#define PLAYER_LADDER_SPEED		2

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define PLAYER_JUMP_FORCE		8

//Frame delay for updating the jump velocity
#define PLAYER_JUMP_DELAY		2

//Player is levitating when abs(speed) <= this value
#define PLAYER_LEVITATING_SPEED	4

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class State { IDLE, WALKING, SHIELD, CROUCHING, ATTACKING_NORMAL, JUMPING, FALLING, CLIMBING, DEAD };

//Rendering states
enum class PlayerAnim {
	IDLE_LEFT, IDLE_RIGHT,
	WALKING_LEFT, WALKING_RIGHT,
	ATTACKING_NORMAL_RIGHT, ATTACKING_NORMAL_LEFT,
	CROUCHRIGHT, CROUCHLEFT,
	SHIELD_WALK_LEFT, SHIELD_WALK_RIGHT,


	JUMPING_LEFT, JUMPING_RIGHT,
	LEVITATING_LEFT, LEVITATING_RIGHT,
	FALLING_LEFT, FALLING_RIGHT,
	CLIMBING_RIGHT, CLIMBING_LEFT, CLIMBING_PRE_TOP, CLIMBING_TOP,
	NUM_ANIMATIONS
};

class Player: public Entity
{
public:
	Player(const Point& p, State s, Look view);
	~Player();
	
	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void InitScore();
	void IncrScore(int n);
	int GetScore();
	int GetXPos();
	int GetYPos();

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;

	//Player mechanics
	void MoveX();
	void MoveY();
	void LogicJumping();
	void LogicAttack();
	void LogicShield();

	//Animation management
	void SetAnimation(int id);
	PlayerAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartFalling();
	void StartJumping();
	void ChangeAnimRight();
	void ChangeAnimLeft();

	//Jump steps
	bool IsAscending() const;
	bool IsLevitating() const;
	bool IsDescending() const;

	//Ladder get in/out steps
	bool IsInFirstHalfTile() const;
	bool IsInSecondHalfTile() const;

	State state;
	Look look;
	int jump_delay;

	//Reference to the TileMap object
	//This class does not own the object, it only holds a reference to it
	TileMap *map;

	int score;
};

