#include "Actors.hpp"
#include "../BattleMechanics.hpp"
#include "../GameConfig.hpp"
#include "../GameController.hpp"

Player::Player(sf::Vector2f pos)
    : Fighter(util::ShapeVector(util::Rectangle, 20.f, 25.f), pos, 500, 0)
{
    m_velBase = 150.f;
    m_grenades = 3;
    m_grenadeCD = 0.f;
    m_shieldCD = 0;
    m_shieldRegen = 10.f;
    m_shieldMax = 200.f;
    m_shield = m_shieldMax;

    m_image.frame = "player";
}

void Player::act(BattleState &state)
{
    //Regen shield
    if (m_shieldCD <= 0 && m_shield < m_shieldMax) {
        m_shield += m_shieldRegen * state.elapsed.asSeconds();
        if (m_shield > m_shieldMax) m_shield = m_shieldMax;
    }

    //Progress and apply statuses
    bool confused = false;
    float velExtra = 0;
    for (std::list<Status>::iterator iter = m_status.begin(); iter != m_status.end();) {
        iter->dur -= state.elapsed.asSeconds();
        if (iter->dur <= 0)
            m_status.erase(iter++);
        else {
            if (iter->type == HASTE) velExtra += 75.f;
            else if (iter->type == SLOW) velExtra -= 60.f;
            else if (iter->type == CONFUSION) confused = true;
            iter++;
        }
    }

    //Compute velocity based on player activity
    bool up = controller.pressing(GameController::K_UP) ^ confused,
         left = controller.pressing(GameController::K_LEFT) ^ confused,
         down = controller.pressing(GameController::K_DOWN) ^ confused,
         right = controller.pressing(GameController::K_RIGHT) ^ confused;
    float velFinal = m_velBase + velExtra;
    m_vel.x = m_vel.y = 0;
    if (up && !down)
        m_vel.y = -velFinal;
    else if (!up && down)
        m_vel.y = velFinal;
    if (left && !right)
        m_vel.x = -velFinal;
    else if (!left && right)
        m_vel.x = velFinal;
    m_pos += m_vel * state.elapsed.asSeconds();

    //Bound by map
    if (m_pos.x < 0)
        m_pos.x = 0;
    else if (m_pos.x > state.map.x)
        m_pos.x = state.map.x;
    if (m_pos.y < 0)
        m_pos.y = 0;
    else if (m_pos.y > state.map.y)
        m_pos.y = state.map.y;
    m_dir = util::toDir(state.cursor.x - m_pos.x, state.cursor.y - m_pos.y);

    //Trigger attack
    cooldown(state);
    if (controller.clicking(GameController::B_LEFT) && m_attackCD <= 0)
        attack(state);
    else if (controller.clicking(GameController::B_RIGHT) && m_grenadeCD <= 0 && m_grenades > 0)
        throwGrenade(state);
}

void Player::hit(BattleState &state, int damage)
{
    state.shot++;
    m_shieldCD = 10.f;
    int finaldmg = damage;
    if (m_shield > 0) {
        if (m_shield > damage) {
            finaldmg = 0;
            m_shield -= (float)damage;
        }
        else {
            finaldmg = damage - (int)(m_shield + 0.5f);
            m_shield = 0;
        }
    }
    m_hp -= finaldmg;
}

int Player::getGrenades() const
{
    return m_grenades;
}

float Player::getShield() const
{
    return m_shield;
}

float Player::getMaxShield() const
{
    return m_shieldMax;
}

void Player::cooldown(BattleState &state)
{
    Fighter::cooldown(state);
    m_grenadeCD -= state.elapsed.asSeconds();
    m_shieldCD -= state.elapsed.asSeconds();
    if (m_grenadeCD < -5)
        m_grenadeCD = 0;
    if (m_shieldCD < -5)
        m_shieldCD = 0;
}

void Player::attack(BattleState &state)
{
    m_attackCD = 0.15f;
    state.fired++;
    shoot(state, REGULAR, 35.f, 10.f);
}

void Player::throwGrenade(BattleState &state)
{
    m_grenades--;
    m_grenadeCD = 0.3f;
    sf::Vector2f grenadePos = util::translatePos(m_pos, m_dir, 30.f, 10.f);
    state.projectiles.push_back(new Grenade(grenadePos, state.cursor));
}

void Player::restoreHP(int amt)
{
    m_hp = MIN(m_hpMax, m_hp + amt);
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
