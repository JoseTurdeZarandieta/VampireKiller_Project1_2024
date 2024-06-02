#pragma once

//Displaying scale factor
#define GAME_SCALE_FACTOR	3.0f

//Function return values
enum class AppStatus { OK = 0, ERROR, QUIT };

//Game states
enum class GameState { DESCRIPTION, KONAMI, MAIN_MENU, START, PLAYING, ENDING, DEVELOPED, CREDITS };

//Main return values
#define EXIT_SUCCESS	0
#define EXIT_FAILURE	1

//Log messages
#define LOG(text, ...) log(__FILE__, __LINE__, text, __VA_ARGS__);
void log(const char file[], int line, const char* text, ...);

//Window size
#define WINDOW_WIDTH    288
#define WINDOW_HEIGHT   280

#define REAL_WINDOW_WIDTH 254

//Game constants
#define MARGIN_GUI_Y	0
#define TILE_SIZE		16
#define LEVEL_WIDTH		18
#define LEVEL_HEIGHT	14

//Entities animation delay
#define ANIM_DELAY		7