#include "Actors.hpp"
#include "GameMechanics.hpp"

Player::Player(sf::Vector2f pos)
	: Fighter("player", util::ShapeVector(util::Rectangle, 25.f, 30.f), pos, 50, 0)
{
	m_base_v = 150.f;
}

void Player::act(GameState &state)
{
	//Progress statuses
	for (std::map<StatusT, StatusD>::iterator iter = m_status.begin(); iter != m_status.end();) {
		iter->second.dur -= state.elapsed.asSeconds();
		if (iter->second.dur <= 0) {
			iter = m_status.erase(iter);
		}
		else {
			iter++;
		}
	}

	//Compute velocity based on player activity
	float final_v = m_base_v
					+ (isStatus(HASTE) ? 75.f : 0.f)
				    - (isStatus(SLOW) ? 60.f : 0.f);
	m_vel.x = m_vel.y = 0;
	if (state.W && !state.S) m_vel.y = -final_v;
	else if (!state.W && state.S) m_vel.y = final_v;
	if (state.A && !state.D) m_vel.x = -final_v;
	else if (!state.A && state.D) m_vel.x = final_v;
	m_pos += m_vel * state.elapsed.asSeconds();

	//Bound by map
	if (m_pos.x < 0) m_pos.x = 0;
	else if (m_pos.x > (float)state.map_width) m_pos.x = (float)state.map_width;
	if (m_pos.y < 0) m_pos.y = 0;
	else if (m_pos.y > (float)state.map_height) m_pos.y = (float)state.map_height;
	m_dir = util::toDir(state.mouse.x - m_pos.x, state.mouse.y - m_pos.y);

	//Trigger attack
	cooldown(state);
	if (state.mouseLeft && m_attack_cd <= 0) attack(state);
}

void Player::attack(GameState& state)
{
	m_attack_cd = 150;
	shoot(state, REGULAR, 40.f, 10.f);
}

void Player::restoreHP(int amt)
{
    m_hp += amt;
    if (m_hp > m_maxHp) m_hp = m_maxHp;
}

void Player::applyStatus(StatusT s, float dur)
{
	if (s == HASTE) {
		m_status[s].dur = dur;
	}
	else if (s == SLOW) {
		m_status[s].dur = dur;
	}
	else if (s == CONFUSE) {
		m_status[s].dur = dur;
	}
	else if (s == SHIELD) {
		m_status[s].dur = dur;
		m_status[s].data.shield = 50;
	}
}

bool Player::isStatus(StatusT s) const
{
	return m_status.find(s) != m_status.end();
}