#include "Actors.hpp"
#include "../BattleMechanics.hpp"
#include <math.h>

Fighter::Fighter(util::ShapeVector size, sf::Vector2f pos, int hp, int faction)
    : Actor(pos, size)
{
    m_attack_cd = 0;
    m_hp = hp;
    m_maxHp = hp;
    m_faction = faction;
    m_collidable = true;
}

bool Fighter::isCollidable() const
{
    return m_collidable;
}

int Fighter::getMaxHP() const
{
    return m_maxHp;
}

int Fighter::getHP() const
{
    return m_hp;
}

int Fighter::getFaction() const
{
    return m_faction;
}

void Fighter::hit(BattleState &state, int damage)
{
    m_hp -= damage;
}

void Fighter::cooldown(BattleState &state)
{
    m_attack_cd -= state.elapsed.asSeconds();
    if (m_attack_cd < -5) m_attack_cd = 0;
}

bool Fighter::isDead(const BattleState &state) const
{
    return m_hp <= 0;
}

void Fighter::shoot(BattleState &state, Bullet b, float offsetX, float offsetY)
{
    shoot(state, b, m_dir, offsetX, offsetY);
}

void Fighter::shoot(BattleState &state, Bullet b, sf::Vector2f &dest, float offsetX, float offsetY)
{
    float dist = util::getDist(m_pos, dest);
    float unitX = (dest.x - m_pos.x) / dist;
    float unitY = (dest.y - m_pos.y) / dist;
    float dir = util::toDir(unitX, unitY);
    sf::Vector2f pos = util::translatePos(m_pos, unitX, unitY, offsetX, offsetY);
    shoot(state, b, pos, dir);
}

void Fighter::shoot(BattleState &state, Bullet b, float dir, float offsetX, float offsetY)
{
    sf::Vector2f pos = util::translatePos(m_pos, dir, offsetX, offsetY);
    shoot(state, b, pos, dir);
}

void Fighter::shoot(BattleState &state, Bullet b, sf::Vector2f pos, float dir)
{
    Projectile* bullet;
    if (b == SPLITTING) 
        bullet = new SplittingBullet(pos, dir, m_faction);
    else if(b == LASER)
        bullet = new Laser(pos, dir, m_faction);
    else if (b == ROUND)
        bullet = new RoundBullet(pos, dir, m_faction);
    else
        bullet = new RegularBullet(pos, dir, m_faction);
    state.projectiles.push_back(bullet);
}
