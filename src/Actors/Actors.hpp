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
    virtual void animate(float elapsed);

    Actor::Image m_image;
    int m_frame, m_frameCount;
    float m_frameElapsed, m_frameDur;
    std::string m_frameBase;
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
    enum Bullet { REGULAR, SPLITTING, LASER, ROUND, SPLITTING_RND, CHASING };
    virtual void attack(BattleState &state) = 0;
    virtual void cooldown(BattleState &state);
    void shoot(BattleState &state, Bullet b, float offsetX, float offsetY);
    void shoot(BattleState &state, Bullet b, sf::Vector2f &dest, float offsetX, float offsetY);
    void shoot(BattleState &state, Bullet b, float dir, float offsetX, float offsetY);
    void shoot(BattleState &state, Bullet b, sf::Vector2f pos, float dir);
    float m_attackCD;
    int m_hp;
    int m_hpMax;
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
    int getGrenades() const;
    float getShield() const;
    float getMaxShield() const;
private:
    virtual void cooldown(BattleState& state);
    virtual void attack(BattleState& state);
    virtual void throwGrenade(BattleState& state);

    float m_velBase;
    int m_grenades;
    float m_grenadeCD;
    float m_shield, m_shieldCD, m_shieldRegen, m_shieldMax;
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
    float m_dur;
};

class Medkit: public Item
{
public:
    Medkit(sf::Vector2f pos);
    virtual void act(BattleState& state);
protected:
    int m_restore;
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
    Grunt(util::ShapeVector size, sf::Vector2f pos, int hp, int faction, int bounty = 0, Item::Type drop = Item::None);
    virtual void act(BattleState& state);
    virtual void hit(BattleState& state, int damage);
protected:
    virtual void attack(BattleState& state);
    virtual void cooldown(BattleState& state);
    float m_velMax;
    float m_radiusClose, m_radiusFar;
    float m_turnCD;
};

class Orange: public Enemy
{
public:
    Orange(sf::Vector2f pos, Item::Type drop = Item::None);
    Orange(util::ShapeVector size, sf::Vector2f pos, int hp, int faction, int bounty = 0, Item::Type drop = Item::None);
    virtual void act(BattleState& state);
    virtual void hit(BattleState& state, int damage);
protected:
    virtual void attack(BattleState& state);
    virtual void cooldown(BattleState& state);
    float m_velMax;
    float m_radiusClose, m_radiusFar;
    float m_turnCD;
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
    float m_turnCD;
    float m_velMax;
    float m_gunDir1, m_gunDir2;
};

class Disappoint: public Grunt
{
public:
    Disappoint(sf::Vector2f pos, Item::Type drop = Item::None);
    virtual void onDeath(BattleState& state);
protected:
    virtual void attack(BattleState& state);
    virtual void cooldown(BattleState& state);
    float m_attackCD1, m_attackCD2;
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
    bool m_attacked, m_dead;
    float m_dur;
};

class RegularBullet: public Projectile
{
public:
    RegularBullet(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
protected:
};

class ChasingBullet: public Projectile
{
public:
    ChasingBullet(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
    virtual bool isDead(const BattleState &state) const;
protected:
    Fighter* m_target;
    float m_dur;
    float m_turnCD;
    float m_maxVel;
};

class SplittingBullet: public Projectile
{
public:
    SplittingBullet(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
protected:
    float m_dur, m_durMax;
    float m_avel;
};

class RoundBullet: public Projectile
{
public:
    RoundBullet(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
};

class SplittingBulletRound: public Projectile
{
public:
    SplittingBulletRound(sf::Vector2f pos, float dir, int faction);
    virtual void act(BattleState& state);
protected:
    float m_dur, m_durMax;
};

class Wiper: public Projectile
{
public:
    Wiper(sf::Vector2f pos, int faction);
    virtual bool isDead(const BattleState& state) const;
    virtual void act(BattleState& state);
protected:
    float m_expandVel;
};

class GrenadeWiper: public Wiper
{
public:
    GrenadeWiper(sf::Vector2f pos);
    virtual bool isDead(const BattleState& state) const;
    virtual void act(BattleState& state);
protected:
    virtual void attack(BattleState& state);
    float m_expandAcc;
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
    float m_dur, m_fadeDur;
    bool m_countHit;
};

#endif
