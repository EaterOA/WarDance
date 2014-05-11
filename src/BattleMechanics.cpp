#include "GameConfig.hpp"
#include "BattleMechanics.hpp"
#include "GameScript.hpp"

BattleState::BattleState()
{
    player = 0;
}

BattleState::~BattleState()
{
    cleanAll();
}

void BattleState::cleanStage()
{
    for (unsigned i = 0; i < enemies.size(); i++) delete enemies[i];
    for (unsigned i = 0; i < projectiles.size(); i++) delete projectiles[i];
    for (unsigned i = 0; i < items.size(); i++) delete items[i];
    enemies = std::vector<Enemy*>();
    projectiles = std::vector<Projectile*>();
    items = std::vector<Item*>();
}

void BattleState::resetLevel()
{
    totalElapsed = sf::Time();
    elapsed = sf::Time();
    shot = fired = hit = 0;
}

void BattleState::cleanAll()
{
    resetLevel();
    score = 0;
    cleanStage();
    delete player;
    player = 0;
}

bool BattleMechanics::init()
{ 
    m_state.map = sf::Vector2f(1600.f, 1200.f);
    m_script = new GameScript(this);

    return true;
}

void BattleMechanics::initLevel()
{
    m_state.cleanAll();
    m_state.player = new Player(sf::Vector2f(100.f, 100.f));
}

void BattleMechanics::clearEnemyProjectiles()
{
    m_state.projectiles.push_back(new Wiper(m_state.player->getPos(), 0));
}

void BattleMechanics::clearPlayerProjectiles()
{
    m_state.projectiles.push_back(new Wiper(m_state.player->getPos(), 1));
}

void BattleMechanics::saveLevelStats()
{
    //Calculate level end stats
    config.setInt("shot", m_state.shot);
    config.setInt("hit", m_state.hit);
    config.setInt("fired", m_state.fired);
    config.setInt("time", (int)m_state.totalElapsed.asSeconds());
    if (m_state.hit >= m_state.fired) config.setInt("accuracy", 100);
    else config.setInt("accuracy", (int)(100.f * m_state.hit / m_state.fired));
    config.setInt("bonus", 200 * config.getInt("accuracy"));
}

void BattleMechanics::startLevel()
{
    m_state.resetLevel();
    std::stringstream scriptName;
    scriptName << "config/Levels/lvl" << config.getInt("level") << ".wds";
    m_script->parseFile(scriptName.str());
}

bool BattleMechanics::isPlayerDead() const
{
    return m_state.player->isDead(m_state);
}

bool BattleMechanics::isLevelDone() const
{
    return m_script->isDone() && m_state.enemies.size() == 0;
}

BattleState& BattleMechanics::getBattleState()
{
    return m_state;
}

void BattleMechanics::updateState(const sf::Time &elapsed, sf::Vector2f mouse)
{
    //Update application-related data
    m_state.elapsed = elapsed;
    m_state.totalElapsed += elapsed;
    m_state.cursor = mouse;

    //Advance script and actors
    m_script->tick(m_state.totalElapsed.asSeconds());
    m_state.player->act(m_state);
    for (unsigned i = 0; i < m_state.projectiles.size(); i++)
        m_state.projectiles[i]->act(m_state);
    for (unsigned i = 0; i < m_state.enemies.size(); i++)
        m_state.enemies[i]->act(m_state);
    for (unsigned i = 0; i < m_state.items.size(); i++)
        m_state.items[i]->act(m_state);

    //Update highscore
    if (m_state.score > config.getInt("highscore")) config.setInt("highscore", m_state.score);

    //Clean up dead things
    for (unsigned i = 0; i < m_state.projectiles.size(); i++) {
        if (m_state.projectiles[i]->isDead(m_state)) {
            delete m_state.projectiles[i];
            std::swap(m_state.projectiles[i], m_state.projectiles.back());
            m_state.projectiles.pop_back();
            i--;
        }
    }
    for (unsigned i = 0; i < m_state.enemies.size(); i++) {
        if (m_state.enemies[i]->isDead(m_state)) {
            delete m_state.enemies[i];
            std::swap(m_state.enemies[i], m_state.enemies.back());
            m_state.enemies.pop_back();
            i--;
        }
    }
    for (unsigned i = 0; i < m_state.items.size(); i++) {
        if (m_state.items[i]->isDead(m_state)) {
            delete m_state.items[i];
            std::swap(m_state.items[i], m_state.items.back());
            m_state.items.pop_back();
            i--;
        }
    }
}

void BattleMechanics::spawnEnemy(std::string &name, std::string &item)
{
    Item::Type i = Item::None;
    if (item == "medkit") i = Item::Medkit;

    if (name == "grunt") {
        sf::Vector2f pos = offMapEntrance(25, 25);
        m_state.enemies.push_back(new Grunt(pos, i));
    }
    else if (name == "alien") {
        sf::Vector2f pos = offMapEntrance(60, 60);
        m_state.enemies.push_back(new Alien(pos, i));
    }
    else if (name == "sprinkler") {
        sf::Vector2f pos = inMapEntrance();
        m_state.enemies.push_back(new Sprinkler(pos, i));
    }
}

void BattleMechanics::spawnEnemy(std::string &name, std::string &item, sf::Vector2f pos)
{
    Item::Type i = Item::None;
    if (item == "medkit") i = Item::Medkit;

    if (name == "grunt") {
        m_state.enemies.push_back(new Grunt(pos, i));
    }
    else if (name == "alien") {
        m_state.enemies.push_back(new Alien(pos, i));
    }
    else if (name == "sprinkler") {
        m_state.enemies.push_back(new Sprinkler(pos, i));
    }
}

sf::Vector2f BattleMechanics::inMapEntrance()
{
    float x = (float)RAND(0, (int)m_state.map.x);
    float y = (float)RAND(0, (int)m_state.map.y);
    return sf::Vector2f(x, y);
}

sf::Vector2f BattleMechanics::offMapEntrance(float offsetX, float offsetY)
{
    int side = RAND(1, 4);
    float x = (float)RAND(0, (int)m_state.map.x);
    float y = (float)RAND(0, (int)m_state.map.y);
    if (side == 1) x = m_state.map.x + offsetX;
    else if (side == 2) y = m_state.map.y + offsetY;
    else if (side == 3) x = -offsetX;
    else if (side == 4) y = -offsetY;
    return sf::Vector2f(x, y);
}
