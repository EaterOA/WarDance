#include "Actors.hpp"
#include "GameMechanics.hpp"

Grunt::Grunt(sf::Vector2f pos)
	: Fighter("grunt", util::ShapeVector(util::Rectangle, 50.f, 50.f), pos, 20, 1)
{
	m_max_v = 50.f;
	m_move_cd = 0;
	m_attack_cd = 0;
}

void Grunt::cooldown(GameState& state)
{
    Fighter::cooldown(state);
    m_move_cd -= state.elapsed.asMilliseconds();
    if (m_move_cd < -5000) m_move_cd = 0;
}

void Grunt::attack(GameState& state)
{
    m_attack_cd = util::rand(500, 1500);
	shoot(state, REGULAR, 40.f, 7.f);
}

void Grunt::hit(int damage, GameState &state)
{
	if (isDead(state)) return;
	m_hp -= damage;
	if (isDead(state)) state.score += 500;
}

void Grunt::act(GameState& state)
{
	Actor::act(state);
	cooldown(state);
	if (m_move_cd <= 0) {
        m_move_cd = util::rand(200, 600);
		sf::Vector2f vec = state.player->getPos() - m_pos;
		float vr = util::getLen(vec);
		float normX = vec.x / vr;
		float normY = vec.y / vr;
		m_dir = util::toDir(normX, normY);
		m_vel = sf::Vector2f(normX * m_max_v, normY * m_max_v);
	}
	
	if (m_attack_cd <= 0) {
        attack(state);
    }
}
