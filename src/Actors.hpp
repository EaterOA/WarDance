#ifndef ACTORS_H
#define ACTORS_H

#include "Util.hpp"
#include <SFML/Graphics.hpp>

struct GameState;

class Actor
{
public:
	Actor(int type, sf::Vector2f pos);
	virtual void act(GameState&);
	sf::Vector2f getPos() const;
	int getType() const;
	float getDir() const;
protected:
	int m_type;
	sf::Vector2f m_pos, m_vel, m_acc;
	float m_dir;
};

class Fighter: public Actor
{
public:
	Fighter(int type, util::ShapeVector size, sf::Vector2f pos, int hp, int faction);
	virtual void act(GameState &state) = 0;
	virtual bool isDead(GameState &state);
	virtual void hit(int damage, GameState &state);
	util::ShapeVector getSize() const;
	int getHP() const;
	int getMaxHP() const;
	int getFaction() const;
protected:
	virtual void attack(GameState &state) = 0;
	virtual void cooldown(GameState &state);
	void shoot(GameState &state, float offsetX, float offsetY);
	void shoot(GameState &state, sf::Vector2f &dest, float offsetX, float offsetY);
	void shoot(GameState &state, float dir, float offsetX, float offsetY);
	void shoot(GameState &state, float dir, float extraDir, float offsetX, float offsetY);
	void shoot(GameState &state, float dir, float normX, float normY, float offsetX, float offsetY);
	int m_attack_cd;
	int m_hp;
	int m_maxHp;
	int m_faction;
	util::ShapeVector m_size;
};

class Player: public Fighter
{
public:
	Player(sf::Vector2f pos, int hp);
	virtual void act(GameState& state);
private:
	virtual void attack(GameState& state);
	sf::Vector2f m_max_v;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Enemies @@@@@@@@@@@@@@@@@@@@@@@@

class Grunt: public Fighter
{
public:
	Grunt(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(int damage, GameState& state);
protected:
	virtual void attack(GameState& state);
    virtual void cooldown(GameState& state);
	float m_max_v;
	int m_move_cd;
};

class Sprinkler: public Fighter
{
public:
	Sprinkler(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(int damage, GameState& state);
protected:
	virtual void attack(GameState& state);
};

class Alien: public Fighter
{
public:
	Alien(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(int damage, GameState& state);
protected:
	virtual void attack(GameState& state);
    virtual void cooldown(GameState& state);
	int m_move_cd;
	float m_max_v;
	float m_gunDir1, m_gunDir2;
};


//@@@@@@@@@@@@@@@@@@@@@@@@ Projectiles @@@@@@@@@@@@@@@@@@@@@@@@

class Projectile: public Fighter
{
public:
	Projectile(int type, sf::Vector2f pos, util::ShapeVector size, int hp, int damage, int faction);
	virtual bool isDead(GameState &state);
protected:
	virtual void attack(GameState& state);
	int m_damage;
};

class RegularBullet: public Projectile
{
public:
	RegularBullet(sf::Vector2f pos, float dir, int faction);
	RegularBullet(sf::Vector2f pos, float dir, float normX, float normY, int faction);
	virtual void act(GameState& state);
protected:
};

class SplittingBullet: public Projectile
{
public:
	SplittingBullet(sf::Vector2f pos, float dir, int faction);
	SplittingBullet(sf::Vector2f pos, float dir, float normX, float normY, int faction);
	virtual void act(GameState& state);
protected:
};

#endif
