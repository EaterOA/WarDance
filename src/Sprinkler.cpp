#include "Actors.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"

Sprinkler::Sprinkler(sf::Vector2f pos): Fighter(3, sf::Vector2f(50.f, 50.f), pos, 10, 1)
{
	m_dir = 0;
}

void Sprinkler::attack(GameState& state)
{
	m_attack_cd = 10;
	m_dir += 10;
	if (m_dir >= 360.f) m_dir -= 360.f;

	float normX, normY, outX, outY;
	
	normX = std::cos(m_dir / 57.29578f);
	normY = std::sin(m_dir / 57.29578f);
	outX = normX * 20.f;
	outY = normY * 20.f;
	Projectile* bullet = new RegularBullet(sf::Vector2f(m_pos.x + outX, m_pos.y + outY), m_faction);
	bullet->setVel(sf::Vector2f(normX * 100.f, normY * 100.f));
	bullet->setDir(m_dir);
	state.projectiles.push_back(bullet);
}

void Sprinkler::act(GameState& state)
{
	Actor::act(state);
	cooldown();
	if (m_attack_cd <= 0) attack(state);
}
