#ifndef GAMEMECHANICS_HPP
#define GAMEMECHANICS_HPP

#include "Actors.hpp"
#include "GameScript.hpp"

struct GameState {
    GameState();
    ~GameState();
    void cleanAll();
    void cleanStage();
    void resetLevel();

    std::vector<Fighter*> enemies;
    std::vector<Projectile*> projectiles;
    std::vector<Item*> items;
    Player* player;
    
    sf::Vector2f map;
    int score;
    int fired, hit, shot;
    sf::Time totalElapsed;
    sf::Time elapsed;
    int level_bonus;
    sf::Vector2f cursor;
};

class GameMechanics {
public:
    bool init();
    void initLevel();
    void startLevel();
    const std::map<std::string, int> getLevelStats();
    bool isLevelDone() const;
    bool isPlayerDead() const;
    GameState& getGameState();
    void updateGameState(const sf::RenderWindow &window, const sf::Time &elapsed);
    void spawnEnemy(std::string name);
    void spawnEnemy(std::string name, sf::Vector2f pos);
    void clearEnemyProjectiles();
    void clearPlayerProjectiles();
private:
    sf::Vector2f inMapEntrance();
    sf::Vector2f offMapEntrance(float offsetX, float offsetY);

    GameState m_state;
    GameScript* m_script;
    bool m_readFromScript;
};

#endif
