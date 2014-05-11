#ifndef ACTORS_H
#define ACTORS_H

#include "../Util.hpp"
#include <SFML/Graphics.hpp>

struct BattleState;

class Actor
{
public:
    struct Image
    {
        Image();
        std::string frame;
        sf::Color color;
        bool rotated, resized;
        sf::Vector2f size;
    };
    
    Actor(sf::Vector2f pos, util::ShapeVector size);
    virtual void act(BattleState& state);
    sf::Vector2f getPos() const;
    const Actor::Image& getImage() const;
    float getDir() const;
    util::ShapeVector getSize() const;
    virtual ~Actor();
protected:
    Actor::Image m_image;
    sf::Vector2f m_pos, m_vel, m_acc;
    float m_dir;
    util::ShapeVector m_size;
};

class Fighter: public Actor
{
public:
    Fighter(util::ShapeVector size, sf::Vector2f pos, int hp, int faction);
    virtual void act(BattleState &state) = 0;
    virtual bool isDead(const BattleState &state) const;
    virtual void hit(BattleState &state, int damage);
    virtual bool isCollidable() const;
    int getHP() const;
    int getMaxHP() const;
    int getFaction() const;
protected:
    enum Bullet { REGULAR, SPLITTING, LASER, ROUND };
    virtual void attack(BattleState &state) = 0;
    virtual void cooldown(BattleState &state);
    void shoot(BattleState &state, Bullet b, float offsetX, float offsetY);
    void shoot(BattleState &state, Bullet b, sf::Vector2f &dest, float offsetX, float offsetY);
    void shoot(BattleState &state, Bullet b, float dir, float offsetX, float offsetY);
    void shoot(BattleState &state, Bullet b, sf::Vector2f pos, float dir);
    float m_attack_cd;
    int m_hp;
    int m_maxHp;
    int m_faction;
    bool m_collidable;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Allies @@@@@@@@@@@@@@@@@@@@@@@@

class Player: public Fighter
{
public:
    //Status structures
    enum StatusType { HASTE, SLOW, CONFUSION };
    struct Status
    {
        StatusType type;
        float dur;
        union
        {
        } data;
    };

    Player(sf::Vector2f pos);
    virtual void act(BattleState& state);
    virtual void hit(BattleState& state, int damage);
    virtual void restoreHP(int amt);
    virtual void applyStatus(StatusType s);
    std::list<Status> getStatuses() const;
    int getNumGrenades() const;
    float getShield() const;
    float getMaxShield() const;
private:
    virtual void cooldown(BattleState& state);
    virtual void attack(BattleState& state);
    virtual void throwGrenade(BattleState& state);

    float m_base_v;
    int m_numGrenades;
    float m_grenade_cd;
    float m_shield, m_shield_cd, m_shield_regen, m_maxShield;
    std::list<Status> m_status;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Items @@@@@@@@@@@@@@@@@@@@@@@@

class Item: public Actor
{
public:
    enum Type { None, Medkit };

    Item(sf::Vector2f pos, util::ShapeVector size, float dur);
    float getDuration() const;
    bool isDead(const BattleState &state) const;
protected:
    float m_duration;
};

class Medkit: public Item
{
public:
    Medkit(sf::Vector2f pos);
    virtual void act(BattleState& state);
protected:
    int m_hpRestore;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Enemies @@@@@@@@@@@@@@@@@@@@@@@@

class Enemy: public Fighter
{
public:
    Enemy(util::ShapeVector size, sf::Vector2f pos, int hp, int faction, int bounty = 0, Item::Type drop = Item::None);
    virtual void onDeath(BattleState& state);
private:
    int m_bounty;
    Item::Type m_drop;
};

class Grunt: public Enemy
{
public:
    Grunt(sf::Vector2f pos, Item::Type drop = Item::None);
    virtual void act(BattleState& state);
    virtual void hit(BattleState& state, int damage);
protected:
    virtual void attack(BattleState& state);
    virtual void cooldown(BattleState& state);
    float m_max_v;
    float m_radius_close, m_radius_far;
    float m_move_cd;
};

class Sprinkler: public Enemy
{
public:
    Sprinkler(sf::Vector2f pos, Item::Type drop = Item::None);
    virtual void act(BattleState& state);
    virtual void hit(BattleState& state, int damage);
protected:
    virtual void attack(BattleState& state);
};

class Alien: public Enemy
{
public:
    Alien(sf::Vector2f pos, Item::Type drop = Item::None);
    virtual void act(BattleState& state);
    virtual void hit(BattleState& state, int damage);
protected:
    virtual void attack(BattleState& state);
    virtual void cooldown(BattleState& state);
    float m_move_cd;
    float m_max_v;
    float m_gunDir1, m_gunDir2;
};

class AirstrikeTarget: public Enemy
{
public:
   AirstrikeTarget(sf::Vector2f pos);
   virtual void act(BattleState& state);
protected:
   float m_countdown;
   bool m_summoned;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Projectiles @@@@@@@@@@@@@@@@@@@@@@@@

class Projectile: public Fighter
{
public:
    Projectile(sf::Vector2f pos, util::ShapeVector size, int hp, int damage, int faction);
    virtual bool isDead(const BattleState &state) const;
protected:
    virtual void attack(BattleState& state);
    int m_damage;
};

class Grenade: public Projectile
{
public:
    Grenade(sf::Vector2f pos, sf::Vector2f dest);
    virtual void act(BattleState& state);
    virtual bool isDead(const BattleState &state) const;
protected:
    virtual void attack(BattleState& state);
    bool m_attacked;
    float m_time;
    float m_animationTime;
};

class RegularBullet: public Projectile
{
public:
    RegularBullet(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
protected:
};

class SplittingBullet: public Projectile
{
public:
    SplittingBullet(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
protected:
    float m_dur, m_maxDur;
    float m_angSpeed;
};

class RoundBullet: public Projectile
{
public:
    RoundBullet(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
};

class Wiper: public Projectile
{
public:
    Wiper(sf::Vector2f pos, int faction);
    virtual bool isDead(const BattleState& state) const;
    virtual void act(BattleState& state);
protected:
    float m_expandSpeed;
};

class GrenadeWiper: public Wiper
{
public:
    GrenadeWiper(sf::Vector2f pos);
    virtual bool isDead(const BattleState& state) const;
    virtual void act(BattleState& state);
protected:
    virtual void attack(BattleState& state);
    float m_expandAccel;
    float m_expandJerk;
    float m_range;
    std::set<Actor*> m_hit;
};

class Laser: public Projectile
{
public:
    Laser(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
protected:
    virtual void attack(BattleState& state);
    float m_time;
    float m_fadeTime;
    bool m_countHit;
};

#endif
