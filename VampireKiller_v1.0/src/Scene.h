#pragma once
#include <raylib.h>
#include "Player.h"
#include "TileMap.h"
#include "Object.h"
#include "Text.h"

enum class DebugMode { OFF, SPRITES_AND_HITBOXES, ONLY_HITBOXES, SIZE };

class Scene
{
public:
    Scene();
    ~Scene();

    AppStatus Init();
    void Update();
    void Render();
    void Release();

    bool EndGame(int Check);

private:
    AppStatus LoadLevel(int stage);
    
    void CheckObjectCollisions();
    void ClearLevel();

    void RenderObjects() const;
    void RenderObjectsDebug(const Color& col) const;

    void RenderGUI() const;

    Player *player;
    
    //Level structure that contains all the static tiles
    TileMap *level;
    
    //Dynamic objects of the level: items and collectables
    std::vector<Object*> objects;
    
    Camera2D camera;
    DebugMode debug;

    Text *font1;
};

