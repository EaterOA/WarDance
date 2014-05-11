#ifndef BATTLEMECHANICS_HPP
#define BATTLEMECHANICS_HPP

#include "Actors/Actors.hpp"
#include "GameScript.hpp"

struct BattleState
{
    BattleState();
    ~BattleState();
    void cleanAll();
    void cleanStage();
    void resetLevel();

    std::vector<Enemy*> enemies;
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

class BattleMechanics
{
public:
    bool init();
    void initLevel();
    void startLevel();
    void saveLevelStats();
    bool isLevelDone() const;
    bool isPlayerDead() const;
    BattleState& getBattleState();
    void updateState(const sf::Time &elapsed, sf::Vector2f mouse);
    void spawnEnemy(std::string &name, std::string &item);
    void spawnEnemy(std::string &name, std::string &item, sf::Vector2f pos);
    void clearEnemyProjectiles();
    void clearPlayerProjectiles();
private:
    sf::Vector2f inMapEntrance();
    sf::Vector2f offMapEntrance(float offsetX, float offsetY);

    BattleState m_state;
    GameScript* m_script;
};

#endif
