#ifndef GAME_MECHANICS_HPP
#define GAME_MECHANICS_HPP

#include <vector>
#include "Actors.hpp"

struct GameState {
	GameState();
	~GameState();
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
	void processEvents(const std::vector<sf::Event>&);
	void updateState(const sf::RenderWindow &window, const sf::Time &elapsed);
	void tick();
	bool cleanUp();
	GameState& getState();
private:
	GameState m_state;
};

#endif