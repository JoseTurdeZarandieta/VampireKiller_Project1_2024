#include "Object.h"
#include "StaticImage.h"

Object::Object(const Point& p, ObjectType t) :
	Entity(p, OBJECT_PHYSICAL_SIZE, OBJECT_PHYSICAL_SIZE, OBJECT_FRAME_SIZE, OBJECT_FRAME_SIZE)
{
	type = t; 
	
	Rectangle rc;
	const int n = TILE_SIZE;
	switch (type)
	{
		case ObjectType::FIRE:  rc = { 9 * n, 2 * n, n, n }; break;
		case ObjectType::DEATHBALL: rc = { 8 * n,	7 * n, n, n }; break;
		case ObjectType::SHIELD: rc = { 6 * n,	7 * n, n, n }; break;

		default: LOG("Internal error: object creation of invalid type");
	}

	ResourceManager& data = ResourceManager::Instance();
	render = new StaticImage(data.GetTexture(Resource::IMG_TILES), rc);
}
Object::~Object()
{
}
void Object::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}

int Object::EndGameCheck() {
	return 1;
}

int Object::Points() const
{
	if (type == ObjectType::FIRE)		return POINTS_FIRE;
	if (type == ObjectType::DEATHBALL)	return POINTS_DEATH;
	if (type == ObjectType::SHIELD)	return POINTS_SHIELD;
	else
	{
		LOG("Internal error: object type invalid when giving points");
		return 0;
	}
}