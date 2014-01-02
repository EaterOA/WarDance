#include "Config.hpp"
#include "GameMechanics.hpp"
#include "GameScript.hpp"

GameState::GameState()
{
	player = 0;
}

GameState::~GameState()
{
	clean();
}

void GameState::clean()
{
	score = 0;
	totalElapsed = sf::Time();
	elapsed = sf::Time();
	delete player;
	for (unsigned i = 0; i < enemies.size(); i++) delete enemies[i];
	for (unsigned i = 0; i < projectiles.size(); i++) delete projectiles[i];
    for (unsigned i = 0; i < items.size(); i++) delete items[i];
	player = 0;
	enemies = std::vector<Fighter*>();
	projectiles = std::vector<Projectile*>();
    items = std::vector<Item*>();
}

bool GameMechanics::init()
{
	m_state.map_width = 1600;
	m_state.map_height = 1200;
	m_script = new GameScript(this);

	return true;
}

void GameMechanics::start()
{
	m_state.clean();
	m_state.player = new Player(sf::Vector2f(100.f, 100.f));

	//wds - WarDance script
	std::stringstream scriptName;
	scriptName << "config/lvl" << (config["level"]+1) << ".wds";
	m_script->parseFile(scriptName.str());
}

void GameMechanics::tick()
{

	m_script->tick(m_state.totalElapsed.asSeconds());
	m_state.player->act(m_state);
	for (unsigned i = 0; i < m_state.projectiles.size(); i++)
		m_state.projectiles[i]->act(m_state);
	for (unsigned i = 0; i < m_state.enemies.size(); i++)
		m_state.enemies[i]->act(m_state);
    for (unsigned i = 0; i < m_state.items.size(); i++)
        m_state.items[i]->act(m_state);

	//Placeholder highscore updater
	if (m_state.score > config["highscore"]) config["highscore"] = m_state.score;
}

bool GameMechanics::cleanUp()
{
	if (m_state.player->isDead(m_state)) return 1;
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
	return 0;
}

GameState& GameMechanics::getState()
{
	return m_state;
}

void GameMechanics::updateState(const sf::RenderWindow &window, const sf::Time &elapsed)
{
	m_state.elapsed = elapsed;
	m_state.totalElapsed += elapsed;
	m_state.cursor = sf::Vector2f(sf::Mouse::getPosition(window));
	m_state.cursor.y += window.getView().getCenter().y - window.getView().getSize().y / 2;
	m_state.cursor.x += window.getView().getCenter().x - window.getView().getSize().x / 2;
}

void GameMechanics::spawnEnemy(std::string name)
{
	if (name == "grunt") {
		sf::Vector2f pos = offMapEntrance(25, 25);
		m_state.enemies.push_back(new Grunt(pos));
	}
	else if (name == "alien") {
		sf::Vector2f pos = offMapEntrance(60, 60);
		m_state.enemies.push_back(new Alien(pos));
	}
	else if (name == "sprinkler") {
		sf::Vector2f pos = inMapEntrance();
		m_state.enemies.push_back(new Sprinkler(pos));
	}
}

void GameMechanics::spawnEnemy(std::string name, sf::Vector2f pos)
{
	if (name == "grunt") {
		m_state.enemies.push_back(new Grunt(pos));
	}
	else if (name == "alien") {
		m_state.enemies.push_back(new Alien(pos));
	}
	else if (name == "sprinkler") {
		m_state.enemies.push_back(new Sprinkler(pos));
	}
}

sf::Vector2f GameMechanics::inMapEntrance()
{
	float x = (float)util::rand(0, m_state.map_width);
	float y = (float)util::rand(0, m_state.map_height);
	return sf::Vector2f(x, y);
}

sf::Vector2f GameMechanics::offMapEntrance(float offsetX, float offsetY)
{
	int side = util::rand(1, 4);
	float x = (float)util::rand(0, m_state.map_width);
	float y = (float)util::rand(0, m_state.map_height);
	if (side == 1) x = (float)m_state.map_width + offsetX;
	else if (side == 2) y = (float)m_state.map_height + offsetY;
	else if (side == 3) x = -offsetX;
	else if (side == 4) y = -offsetY;
	return sf::Vector2f(x, y);
}
