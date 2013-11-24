#ifndef APP_HPP
#define APP_HPP

enum AppState {
	SPLASH, MENU, GAME, PAUSED, DEAD, NOFOCUS, CLOSED
};

const int APP_WIDTH = 800;
const int APP_HEIGHT = 600;
extern AppState appState;

void pauseGame();
void resumeGame();
void endGame();

#endif