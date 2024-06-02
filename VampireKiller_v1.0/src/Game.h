#pragma once
#include "Globals.h"
#include "Scene.h"
#include "FadeTransition.h"

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene* scene;
    const Texture2D* img_descr;
    const Texture2D* img_menu;
    const Texture2D* img_konami;
    const Texture2D* img_start1;
    const Texture2D* img_startblack;
    const Texture2D* img_ending;
    const Texture2D* img_developed;
    const Texture2D* img_credits;

    Music MusicVK;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;

    //To make fade out/in transitions effects
    FadeTransition fade_transition;

    //bool to make blinking effect
    bool Blink = true;
    float blinktime = 0.1;
    float BlinkTimer = 10;
};