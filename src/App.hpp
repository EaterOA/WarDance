#ifndef APP_HPP
#define APP_HPP

enum AppState {
	SPLASH, MAIN, SETTINGS, GAME, PAUSED, DEAD, NOFOCUS, CLOSED, NONE
};

const int APP_WIDTH = 800;
const int APP_HEIGHT = 600;
extern AppState appState;
extern AppState prevState;
extern AppState haltState;

void startGame();
void pauseGame();
void resumeGame();
void goToSettings();
void goToMain();
void endGame();

#endif
