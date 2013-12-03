#ifndef GAME_MECHANICS_HPP
#define GAME_MECHANICS_HPP

#include <vector>
#include "Actors.hpp"

struct GameState {
	GameState();
	~GameState();
	bool inMap(sf::Vector2f p) const;

	int map_width, map_height;
	std::vector<Fighter*> enemies;
	std::vector<Projectile*> projectiles;
	Player* player;
	int score;
	int level;

	sf::Time elapsed;
	sf::Vector2f mouse;
	bool mouseLeft;
	bool W,A,S,D;
};

class GameMechanics {
public:
	void gameInit();
	void updateState(const sf::RenderWindow &window, const sf::Time &elapsed);
	void tick();
	bool cleanUp();
	GameState& getState();
private:
	GameState m_state;
};

#endif