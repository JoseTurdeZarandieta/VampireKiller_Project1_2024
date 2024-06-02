#include "Game.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <stdio.h>

Game::Game()
{
    state = GameState::DESCRIPTION;
    scene = nullptr;
    img_menu = nullptr;
    img_descr = nullptr;
    img_start1 = nullptr;
    img_startblack = nullptr;
    img_developed = nullptr;
    img_credits = nullptr;

    target = {};
    src = {};
    dst = {};
}
Game::~Game()
{
    if (scene != nullptr)
    {
        scene->Release();
        delete scene;
        scene = nullptr;
    }
}
AppStatus Game::Initialise(float scale)
{
    float w, h;
    w = WINDOW_WIDTH * scale;
    h = WINDOW_HEIGHT * scale;

    //Initialise window
    InitWindow((int)w, (int)h, "Vampire Killer");
    InitAudioDevice();


    //Render texture initialisation, used to hold the rendering result so we can easily resize it
    target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (target.id == 0)
    {
        LOG("Failed to create render texture");
        return AppStatus::ERROR;
    }
    SetTextureFilter(target.texture, TEXTURE_FILTER_POINT);
    src = { 0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT };
    dst = { 0, 0, w, h };

    //Load resources
    if (LoadResources() != AppStatus::OK)
    {
        LOG("Failed to load resources");
        return AppStatus::ERROR;
    }

    //Initialise the fade in effect
    fade_transition.Set(GameState::DESCRIPTION, 60, dst);

    //Set the target frame rate for the application
    SetTargetFPS(60);
    //Disable the escape key to quit functionality
    SetExitKey(0);

    return AppStatus::OK;
}
AppStatus Game::LoadResources()
{
    ResourceManager& data = ResourceManager::Instance();

    /*         description          */
    if (data.LoadTexture(Resource::IMG_DESCR, "images/description.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_descr = data.GetTexture(Resource::IMG_DESCR);

    /*         IMG_KONAMI          */
    if (data.LoadTexture(Resource::IMG_KONAMI, "images/konami.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_konami = data.GetTexture(Resource::IMG_KONAMI);
    /*          MAIN MENU           */
    if (data.LoadTexture(Resource::IMG_MENU, "images/startScreen.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_menu = data.GetTexture(Resource::IMG_MENU);

    /*          START1           */

    if (data.LoadTexture(Resource::IMG_START1, "images/startScreenPlay.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_start1 = data.GetTexture(Resource::IMG_START1);

    /*          STARTBLACK           */

    if (data.LoadTexture(Resource::IMG_START2, "images/startScreenBlack.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_startblack = data.GetTexture(Resource::IMG_START2);

    /*         ENDING          */

    if (data.LoadTexture(Resource::IMG_END, "images/ending.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_ending = data.GetTexture(Resource::IMG_END);

    /*          DEVELOPED BY           */

    if (data.LoadTexture(Resource::IMG_DEV, "images/developed_by.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_developed = data.GetTexture(Resource::IMG_DEV);

    /*          CREDITS TO           */

    if (data.LoadTexture(Resource::IMG_CREDS, "images/allcredits.png") != AppStatus::OK)
    {
        return AppStatus::ERROR;
    }
    img_credits = data.GetTexture(Resource::IMG_CREDS);

    /*      SOUND    */
    MusicVK = LoadMusicStream("music/02_VampireKiller");
    MusicVK.looping = true;


    return AppStatus::OK;
}
AppStatus Game::BeginPlay()
{
    scene = new Scene();
    PlayMusicStream(MusicVK);
    if (scene == nullptr)
    {
        LOG("Failed to allocate memory for Scene");
        return AppStatus::ERROR;
    }
    if (scene->Init() != AppStatus::OK)
    {
        LOG("Failed to initialise Scene");
        return AppStatus::ERROR;
    }

    return AppStatus::OK;
}
void Game::FinishPlay()
{
    scene->Release();
    StopMusicStream(MusicVK);
    delete scene;
    scene = nullptr;
}
AppStatus Game::Update()
{
    //Check if user attempts to close the window, either by clicking the close button or by pressing Alt+F4
    if (WindowShouldClose()) return AppStatus::QUIT;

    if (fade_transition.IsActive())
    {
        GameState prev_frame = state;
        state = fade_transition.Update();

        //Begin play and finish play are delayed due to the fading transition effect
        if (prev_frame == GameState::DESCRIPTION && state == GameState::KONAMI)
        {
            if (BeginPlay() != AppStatus::OK) return AppStatus::ERROR;
        }
        else if (prev_frame == GameState::PLAYING && state == GameState::CREDITS)
        {
            FinishPlay();
        }
    }
    else
    {
        switch (state)
        {
        case GameState::DESCRIPTION:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                fade_transition.Set(GameState::DESCRIPTION, 0, GameState::KONAMI, 60, dst);
            }
            break;

        case GameState::KONAMI:
            if (IsKeyPressed(KEY_ESCAPE)) return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE))
            {
                fade_transition.Set(GameState::KONAMI, 0, GameState::MAIN_MENU, 60, dst);
            }
            break;

        case GameState::MAIN_MENU:
            if (IsKeyPressed(KEY_ESCAPE))   return AppStatus::QUIT;
            if (IsKeyPressed(KEY_SPACE)) {
                //state = GameState::START;
                fade_transition.Set(GameState::MAIN_MENU, 2, GameState::START, 60, dst);
            }
            break;

        case GameState::START:
            state = GameState::PLAYING;
            //fade_transition.Set(GameState::START, 60, GameState::PLAYING, 60, dst);
            scene = new Scene;
            scene->Init();
            break;

        case GameState::PLAYING:
            if (IsKeyPressed(KEY_ESCAPE))
            {
                fade_transition.Set(GameState::PLAYING, 2, GameState::ENDING, 60, dst);
            }
            //else if (scene->EndGame()) {
            //    fade_transition.Set(GameState::PLAYING, 60, GameState::ENDING, 60, dst);
            //}
            else
            {
                UpdateMusicStream(MusicVK);
                scene->Update();
            }
            break;

        case GameState::ENDING:
            if (IsKeyPressed(KEY_ESCAPE))       state = GameState::DEVELOPED;
            break;

        case GameState::DEVELOPED:
            if (IsKeyPressed(KEY_SPACE))
            {
                state = GameState::CREDITS;
            }
            break;
        case GameState::CREDITS:
            if (IsKeyPressed(KEY_ESCAPE))
            {
                state = GameState::MAIN_MENU;
            }
            break;
        }
    }

    return AppStatus::OK;
}
void Game::Render()
{
    //Draw everything in the render texture, note this will not be rendered on screen, yet
    BeginTextureMode(target);
    ClearBackground(BLACK);

    switch (state)
    {
    case GameState::DESCRIPTION:
        DrawTexture(*img_descr, 0, 0, WHITE);
        break;
    case GameState::KONAMI:
        DrawTexture(*img_konami, 0, 0, WHITE);
        break;

    case GameState::MAIN_MENU:
        DrawTexture(*img_menu, 0, 0, WHITE);
        break;

    case GameState::START:
        BlinkTimer += GetFrameTime();
        if (BlinkTimer > blinktime) {
            Blink = !Blink;
            BlinkTimer = 0;
        }
        if (Blink) {
            DrawTexture(*img_start1, 0, 0, WHITE);
        }
        else {
            DrawTexture(*img_startblack, 0, 0, WHITE);
        }
        break;

    case GameState::PLAYING:
        scene->Render();
        break;

    case GameState::ENDING:

        DrawTexture(*img_ending, 0, 0, WHITE);
        break;
    case GameState::DEVELOPED:
        DrawTexture(*img_developed, 0, 0, WHITE);
        break;
    case GameState::CREDITS:
        DrawTexture(*img_credits, 0, 0, WHITE);
        break;
    }

    EndTextureMode();

    //Draw render texture to screen, properly scaled
    BeginDrawing();
    DrawTexturePro(target.texture, src, dst, { 0, 0 }, 0.0f, WHITE);
    if (fade_transition.IsActive()) fade_transition.Render();
    EndDrawing();
}
void Game::Cleanup()
{
    CloseAudioDevice();
    UnloadResources();
    CloseWindow();
}
void Game::UnloadResources()
{
    ResourceManager& data = ResourceManager::Instance();
    data.ReleaseTexture(Resource::IMG_MENU);
    data.ReleaseTexture(Resource::IMG_CREDS);
    data.ReleaseTexture(Resource::IMG_DEV);
    data.ReleaseTexture(Resource::IMG_END);
    data.ReleaseTexture(Resource::IMG_ENEMIES);
    data.ReleaseTexture(Resource::IMG_KONAMI);
    data.ReleaseTexture(Resource::IMG_START1);
    data.ReleaseTexture(Resource::IMG_START2);
    data.ReleaseTexture(Resource::IMG_DESCR);

    UnloadRenderTexture(target);
    UnloadMusicStream(MusicVK);
}