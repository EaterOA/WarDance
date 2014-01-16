#include "Actors.hpp"
#include "GameMechanics.hpp"

Grunt::Grunt(sf::Vector2f pos)
	: Fighter("grunt", util::ShapeVector(util::Rectangle, 50.f, 50.f), pos, 200, 1)
{
	m_max_v = 50.f;
	m_move_cd = 0;
	m_attack_cd = 0;
	m_radius_close = 30.f;
	m_radius_far = 70.f;
}

void Grunt::cooldown(GameState& state)
{
    Fighter::cooldown(state);
    m_move_cd -= state.elapsed.asMilliseconds();
    if (m_move_cd < -5000) m_move_cd = 0;
}

void Grunt::attack(GameState& state)
{
    m_attack_cd = util::rand(500, 1500) / 1000.f;
	shoot(state, REGULAR, 40.f, 7.f);
}

void Grunt::hit(GameState &state, int damage)
{
	if (isDead(state)) return;
	m_hp -= damage;
	if (isDead(state)) {
		state.score += 500;
		if (rand() % 15 == 0) state.items.push_back(new Medkit(m_pos, 10));
	}
}

void Grunt::act(GameState& state)
{
	cooldown(state);

	if (m_move_cd <= 0) {
        m_move_cd = util::rand(200, 600) / 1000.f;
		sf::Vector2f vec = state.player->getPos() - m_pos;
		float vr = util::getLen(vec);
		float unitX = vec.x / vr;
		float unitY = vec.y / vr;
		m_dir = util::toDir(unitX, unitY);
		if (vr > m_radius_far)
			m_vel = sf::Vector2f(unitX * m_max_v, unitY * m_max_v);
		else if (vr < m_radius_close)
			m_vel = sf::Vector2f(-unitX * m_max_v, -unitY * m_max_v);
		else
			m_vel = sf::Vector2f(0, 0);
	}
	m_pos += m_vel * state.elapsed.asSeconds();
	
	if (m_attack_cd <= 0) {
        attack(state);
    }
}
