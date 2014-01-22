#ifndef ACTORS_H
#define ACTORS_H

#include "Util.hpp"
#include <SFML/Graphics.hpp>

struct GameState;

class Actor
{
public:
	Actor(std::string frame, sf::Vector2f pos, util::ShapeVector size);
	virtual void act(GameState& state);
	sf::Vector2f getPos() const;
	const std::string& getFrame() const;
	float getDir() const;
    util::ShapeVector getSize() const;
protected:
	std::string m_frame;
	sf::Vector2f m_pos, m_vel, m_acc;
	float m_dir;
    util::ShapeVector m_size;
};

class Fighter: public Actor
{
public:
	Fighter(std::string frame, util::ShapeVector size, sf::Vector2f pos, int hp, int faction);
	virtual void act(GameState &state) = 0;
	virtual bool isDead(const GameState &state) const;
	virtual void hit(GameState &state, int damage);
	int getHP() const;
	int getMaxHP() const;
	int getFaction() const;
protected:
    enum Bullet { REGULAR, SPLITTING, LASER };
	virtual void attack(GameState &state) = 0;
	virtual void cooldown(GameState &state);
	void shoot(GameState &state, Bullet b, float offsetX, float offsetY);
	void shoot(GameState &state, Bullet b, sf::Vector2f &dest, float offsetX, float offsetY);
	void shoot(GameState &state, Bullet b, float dir, float offsetX, float offsetY);
	void shoot(GameState &state, Bullet b, float dir, float extraDir, float offsetX, float offsetY);
	void shoot(GameState &state, Bullet b, float dir, float unitX, float unitY, float offsetX, float offsetY);
	float m_attack_cd;
	int m_hp;
	int m_maxHp;
	int m_faction;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Allies @@@@@@@@@@@@@@@@@@@@@@@@

class Player: public Fighter
{
public:
	//Status structures
	enum StatusT { SHIELD, HASTE, SLOW, CONFUSE };
	struct StatusD
	{
		float dur;
		union
		{
			int shield;
		} data;
	};

	Player(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(GameState& state, int damage);
    virtual void restoreHP(int amt);
	virtual void applyStatus(StatusT s, float dur);
	virtual bool isStatus(StatusT s) const;
	int getNumGrenades() const;
	float getShield() const;
	float getMaxShield() const;
private:
	virtual void cooldown(GameState& state);
	virtual void attack(GameState& state);
	virtual void throwGrenade(GameState& state);
	float m_base_v;
	int m_numGrenades;
	float m_grenade_cd;
	float m_laser_cd;
	float m_shield_cd;
	float m_shield, m_shield_regen, m_maxShield;
	std::map<StatusT, StatusD> m_status;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Enemies @@@@@@@@@@@@@@@@@@@@@@@@

class Grunt: public Fighter
{
public:
	Grunt(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(GameState& state, int damage);
protected:
	virtual void attack(GameState& state);
    virtual void cooldown(GameState& state);
	float m_max_v;
	float m_radius_close, m_radius_far;
	float m_move_cd;
};

class Sprinkler: public Fighter
{
public:
	Sprinkler(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(GameState& state, int damage);
protected:
	virtual void attack(GameState& state);
};

class Alien: public Fighter
{
public:
	Alien(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(GameState& state, int damage);
protected:
	virtual void attack(GameState& state);
    virtual void cooldown(GameState& state);
	float m_move_cd;
	float m_max_v;
	float m_gunDir1, m_gunDir2;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Projectiles @@@@@@@@@@@@@@@@@@@@@@@@

class Projectile: public Fighter
{
public:
	Projectile(std::string frame, sf::Vector2f pos, util::ShapeVector size, int hp, int damage, int faction);
	virtual bool isDead(const GameState &state) const;
protected:
	virtual void attack(GameState& state);
	int m_damage;
};

class RegularGrenade: public Projectile
{
public:
	RegularGrenade(sf::Vector2f pos, sf::Vector2f dest);
	virtual void act(GameState& state);
protected:
	virtual void attack(GameState& state);
	float m_time;
};

class RegularBullet: public Projectile
{
public:
	RegularBullet(sf::Vector2f pos, float dir, int faction);
	RegularBullet(sf::Vector2f pos, float dir, float unitX, float unitY, int faction);
	virtual void act(GameState& state);
protected:
};

class SplittingBullet: public Projectile
{
public:
	SplittingBullet(sf::Vector2f pos, float dir, int faction);
	SplittingBullet(sf::Vector2f pos, float dir, float unitX, float unitY, int faction);
	virtual void act(GameState& state);
protected:
};

class Wiper: public Projectile
{
public:
	Wiper(sf::Vector2f pos, int faction);
	virtual void act(GameState& state);
protected:
	float m_expandSpeed;
};

class Laser: public Projectile
{
public:
	Laser(sf::Vector2f pos, float dir, int faction);
	virtual void act(GameState& state);
	float getFadePerc() const;
protected:
	virtual void attack(GameState& state);
	float m_time;
	float m_fadeTime;
	bool m_countHit;
};



//@@@@@@@@@@@@@@@@@@@@@@@@ Items @@@@@@@@@@@@@@@@@@@@@@@@

class Item: public Actor
{
public:
    Item(std::string frame, sf::Vector2f pos, util::ShapeVector size, float dur);
    float getDuration() const;
    bool isDead(const GameState &state) const;
protected:
    float m_duration;
};

class Medkit: public Item
{
public:
    Medkit(sf::Vector2f pos, float dur);
    virtual void act(GameState& state);
protected:
    int m_hpRestore;
};

#endif
