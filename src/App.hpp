#ifndef APP_HPP
#define APP_HPP

#include <vector>

struct GameState;

enum AppState {
    SPLASH, MAIN, SELECTLEVEL, SETTINGS, GAME, PAUSED, LEVELENDSEQUENCE, NOFOCUS, CLOSED, NONE
};

const int APP_WIDTH = 800;
const int APP_HEIGHT = 600;
extern std::vector<AppState> appStates;

AppState getAppState();
void back();
void startGame();
void pauseGame();
void goToSettings();
void goToSelectLevel();
void goToMain();
void goToLevelEnd();
void goToLevelStart();
void endGame();

#endif
