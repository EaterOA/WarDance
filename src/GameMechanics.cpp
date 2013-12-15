#include "Config.hpp"
#include "GameMechanics.hpp"

GameState::GameState()
{
	player = 0;
	score = 0;
	level = 0;
}

GameState::~GameState()
{
	for (unsigned i = 0; i < enemies.size(); i++) delete enemies[i];
	for (unsigned i = 0; i < projectiles.size(); i++) delete projectiles[i];
	delete player;
}

bool GameState::inMap(sf::Vector2f p) const
{
	return p.x >= 0 && p.x <= map_width && p.y >= 0 && p.y <= map_height;
}

bool GameMechanics::init()
{
	m_state.map_width = 1600;
	m_state.map_height = 1200;
	m_state.player = new Player(sf::Vector2f(100.f, 100.f), 50);
	return true;
}

void GameMechanics::updateState(const sf::RenderWindow &window, const sf::Time &elapsed)
{
	m_state.elapsed = elapsed;
	m_state.mouse = sf::Vector2f(sf::Mouse::getPosition(window));
	m_state.mouse.y += window.getView().getCenter().y - window.getView().getSize().y / 2;
	m_state.mouse.x += window.getView().getCenter().x - window.getView().getSize().x / 2;
	m_state.mouseLeft = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	m_state.W = conf::pressing(conf::UP);
	m_state.A = conf::pressing(conf::LEFT);
	m_state.S = conf::pressing(conf::DOWN);
	m_state.D = conf::pressing(conf::RIGHT);
}

void GameMechanics::tick()
{
	//Placeholder enemy generator
	if (rand() % 200 == 0) {
		if (rand() % 5 == 0) m_state.enemies.push_back(new Alien(sf::Vector2f(float(rand() % 1600), float(rand() % 1200))));
		else if (rand() % 2 == 0) m_state.enemies.push_back(new Grunt(sf::Vector2f(float(rand() % 1600), float(rand() % 1200))));
		else m_state.enemies.push_back(new Sprinkler(sf::Vector2f(float(rand() % 1600), float(rand() % 1200))));
	}

	m_state.player->act(m_state);
	for (unsigned i = 0; i < m_state.projectiles.size(); i++)
		m_state.projectiles[i]->act(m_state);
	for (unsigned i = 0; i < m_state.enemies.size(); i++)
		m_state.enemies[i]->act(m_state);
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
	return 0;
}

GameState& GameMechanics::getState()
{
	return m_state;
}