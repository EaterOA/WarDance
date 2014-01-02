#ifndef GAME_MECHANICS_HPP
#define GAME_MECHANICS_HPP

#include "Actors.hpp"
#include "GameScript.hpp"

struct GameState {
	GameState();
	~GameState();
	void clean();

	int map_width, map_height;
	int score;
	std::vector<Fighter*> enemies;
	std::vector<Projectile*> projectiles;
    std::vector<Item*> items;
	Player* player;

	sf::Time totalElapsed;
	sf::Time elapsed;
	sf::Vector2f cursor;
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
	void spawnEnemy(std::string name, sf::Vector2f pos);
private:
	sf::Vector2f inMapEntrance();
	sf::Vector2f offMapEntrance(float offsetX, float offsetY);

	GameState m_state;
	GameScript* m_script;
	bool m_readFromScript;
};

#endif
