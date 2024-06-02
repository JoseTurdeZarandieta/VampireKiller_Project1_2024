#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	14
#define OBJECT_FRAME_SIZE		16

#define POINTS_FIRE		1
#define POINTS_DEATH	2
#define POINTS_SHIELD	3

enum class ObjectType { FIRE, DEATHBALL, SHIELD };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	
	int EndGameCheck();
	int Points() const;

private:
	ObjectType type;
};

