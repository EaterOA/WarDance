#include "Actors.hpp"
#include "BattleMechanics.hpp"
#include "GameConfig.hpp"
#include "GameController.hpp"

Player::Player(sf::Vector2f pos)
    : Fighter(util::ShapeVector(util::Rectangle, 20.f, 25.f), pos, 500, 0)
{
    m_base_v = 150.f;
    m_numGrenades = 3;
    m_grenade_cd = 0.f;
    m_shield_cd = 0;
    m_shield_regen = 10.f;
    m_maxShield = 200.f;
    m_shield = m_maxShield;

    m_image.frame = "player";
}

void Player::act(GameState &state)
{
    //Regen shield
    if (m_shield_cd <= 0 && m_shield < m_maxShield) {
        m_shield += m_shield_regen * state.elapsed.asSeconds();
        if (m_shield > m_maxShield) m_shield = m_maxShield;
    }

    //Progress and apply statuses
    bool confused = false;
    float extra_v = 0;
    for (std::list<Status>::iterator iter = m_status.begin(); iter != m_status.end();) {
        iter->dur -= state.elapsed.asSeconds();
        if (iter->dur <= 0)
            m_status.erase(iter++);
        else {
            if (iter->type == HASTE) extra_v += 75.f;
            else if (iter->type == SLOW) extra_v -= 60.f;
            else if (iter->type == CONFUSION) confused = true;
            iter++;
        }
    }

    //Compute velocity based on player activity
    bool up = controller.pressing(GameController::K_UP) ^ confused,
         left = controller.pressing(GameController::K_LEFT) ^ confused,
         down = controller.pressing(GameController::K_DOWN) ^ confused,
         right = controller.pressing(GameController::K_RIGHT) ^ confused;
    float final_v = m_base_v + extra_v;
    m_vel.x = m_vel.y = 0;
    if (up && !down) m_vel.y = -final_v;
    else if (!up && down) m_vel.y = final_v;
    if (left && !right) m_vel.x = -final_v;
    else if (!left && right) m_vel.x = final_v;
    m_pos += m_vel * state.elapsed.asSeconds();

    //Bound by map
    if (m_pos.x < 0) m_pos.x = 0;
    else if (m_pos.x > state.map.x) m_pos.x = state.map.x;
    if (m_pos.y < 0) m_pos.y = 0;
    else if (m_pos.y > state.map.y) m_pos.y = state.map.y;
    m_dir = util::toDir(state.cursor.x - m_pos.x, state.cursor.y - m_pos.y);

    //Trigger attack
    cooldown(state);
    if (controller.clicking(GameController::B_LEFT) && m_attack_cd <= 0) attack(state);
    else if (controller.clicking(GameController::B_RIGHT) && m_grenade_cd <= 0 && m_numGrenades > 0) throwGrenade(state);
}

void Player::hit(GameState &state, int damage)
{
    state.shot++;
    m_shield_cd = 10.f;
    int final = damage;
    if (m_shield > 0) {
        if (m_shield > damage) {
            final = 0;
            m_shield -= (float)damage;
        }
        else {
            final = damage - (int)(m_shield + 0.5f);
            m_shield = 0;
        }
    }
    m_hp -= final;
}

int Player::getNumGrenades() const
{
    return m_numGrenades;
}

float Player::getShield() const
{
    return m_shield;
}

float Player::getMaxShield() const
{
    return m_maxShield;
}

void Player::cooldown(GameState &state)
{
    Fighter::cooldown(state);
    m_grenade_cd -= state.elapsed.asSeconds();
    m_shield_cd -= state.elapsed.asSeconds();
    if (m_grenade_cd < -5) m_grenade_cd = 0;
    if (m_shield_cd < -5) m_shield_cd = 0;
}

void Player::attack(GameState &state)
{
    m_attack_cd = 0.15f;
    state.fired++;
    shoot(state, REGULAR, 35.f, 10.f);
}

void Player::throwGrenade(GameState &state)
{
    m_numGrenades--;
    m_grenade_cd = 0.3f;
    sf::Vector2f grenadePos = util::translatePos(m_pos, m_dir, 30.f, 10.f);
    state.projectiles.push_back(new Grenade(grenadePos, state.cursor));
}

void Player::restoreHP(int amt)
{
    m_hp += amt;
    if (m_hp > m_maxHp) m_hp = m_maxHp;
}

std::list<Player::Status> Player::getStatuses() const
{
    return m_status;
}

void Player::applyStatus(StatusType s)
{
    Status st;
    st.type = s;
    if (s == HASTE)
        st.dur = 30;
    else if (s == SLOW)
        st.dur = 30;
    else if (s == CONFUSION)
        st.dur = 30;

    for (std::list<Status>::iterator iter = m_status.begin(); iter != m_status.end(); iter++) {
        if (iter->type == s) {
            *iter = st;
            return;
        }
    }
    m_status.push_back(st);
}