#ifndef GAME_MECHANICS_HPP
#define GAME_MECHANICS_HPP

#include "Actors.hpp"

struct GameState {
	GameState();
	~GameState();
	void clean();

	int map_width, map_height;
	int score;
	std::vector<Fighter*> enemies;
	std::vector<Projectile*> projectiles;
	Player* player;

	sf::Time totalElapsed;
	sf::Time elapsed;
	sf::Vector2f mouse;
	bool mouseLeft;
	bool W,A,S,D;
};

class GameMechanics {
public:
	bool init();
	void start();
	void reset();
	void tick();
	bool cleanUp();
	GameState& getState();
	void updateState(const sf::RenderWindow &window, const sf::Time &elapsed);
	void spawnEnemy(std::string name);
private:
	sf::Vector2f inMapEntrance();
	sf::Vector2f offMapEntrance(float offsetX, float offsetY);

	GameState m_state;
};

#endif
