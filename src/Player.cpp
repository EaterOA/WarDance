#include "Actors.hpp"
#include "GameMechanics.hpp"

Player::Player(sf::Vector2f pos)
	: Fighter("player", util::ShapeVector(util::Rectangle, 25.f, 30.f), pos, 50, 0)
{
	m_base_v = 150.f;
	m_numGrenades = 3;
	m_grenade_cd = 0.f;
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
	bool confuse = isStatus(CONFUSE);
	bool up = conf::pressing(conf::K_UP) ^ confuse,
		 left = conf::pressing(conf::K_LEFT) ^ confuse,
		 down = conf::pressing(conf::K_DOWN) ^ confuse,
		 right = conf::pressing(conf::K_RIGHT) ^ confuse;
	float final_v = m_base_v
					+ (isStatus(HASTE) ? 75.f : 0.f)
				    - (isStatus(SLOW) ? 60.f : 0.f);
	m_vel.x = m_vel.y = 0;
	if (up && !down) m_vel.y = -final_v;
	else if (!up && down) m_vel.y = final_v;
	if (left && !right) m_vel.x = -final_v;
	else if (!left && right) m_vel.x = final_v;
	m_pos += m_vel * state.elapsed.asSeconds();

	//Bound by map
	if (m_pos.x < 0) m_pos.x = 0;
	else if (m_pos.x > (float)state.map_width) m_pos.x = (float)state.map_width;
	if (m_pos.y < 0) m_pos.y = 0;
	else if (m_pos.y > (float)state.map_height) m_pos.y = (float)state.map_height;
	m_dir = util::toDir(state.cursor.x - m_pos.x, state.cursor.y - m_pos.y);

	//Trigger attack
	cooldown(state);
	if (conf::clicking(conf::B_LEFT) && m_attack_cd <= 0) attack(state);
	else if (conf::clicking(conf::B_RIGHT) && m_grenade_cd <= 0 && m_numGrenades > 0) throwGrenade(state);
}

void Player::hit(GameState &state, int damage)
{
	int final = damage;
	if (isStatus(SHIELD)) {
		int remaining = m_status[SHIELD].data.shield;
		if (remaining > damage) {
			final = 0;
			m_status[SHIELD].data.shield -= damage;
		}
		else {
			final = damage - remaining;
			m_status.erase(SHIELD);
		}
	}
	m_hp -= final;
}

int Player::getNumGrenades() const
{
	return m_numGrenades;
}

int Player::getShield() const
{
	if (isStatus(SHIELD)) return m_status.find(SHIELD)->second.data.shield;
	return 0;
}

int Player::getMaxShield() const
{
	return 50;
}

void Player::cooldown(GameState &state)
{
	Fighter::cooldown(state);
	m_grenade_cd -= state.elapsed.asMilliseconds();
	if (m_grenade_cd < -5000) m_grenade_cd = 0;
}

void Player::attack(GameState &state)
{
	m_attack_cd = 150;
	shoot(state, REGULAR, 40.f, 10.f);
}

void Player::throwGrenade(GameState &state)
{
	m_numGrenades--;
	m_grenade_cd = 300;
	state.projectiles.push_back(new RegularGrenade(m_pos, state.cursor));
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