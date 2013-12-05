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
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_acc;
	float m_dir;
};

class Fighter: public Actor
{
public:
	Fighter(int type, util::ShapeVector size, sf::Vector2f pos, int hp, int faction);
	virtual void act(GameState &state) = 0;
	virtual bool isDead(const GameState &state);
	virtual void hit(int damage, GameState &state);
	util::ShapeVector getSize() const;
	int getHP() const;
	int getMaxHP() const;
protected:
	virtual void attack(GameState &state) = 0;
	virtual void cooldown();
	void shoot(GameState &state, float offsetX, float offsetY);
	virtual void shootTowards(GameState &state, sf::Vector2f &dest, float offsetX, float offsetY);
	int m_attack_cd;
	int m_hp;
	int m_maxHp;
	int m_faction;
	util::ShapeVector m_size;
};

class Projectile: public Fighter
{
public:
	Projectile(int type, sf::Vector2f pos, util::ShapeVector size, int hp, int damage, int faction);
	void setVel(sf::Vector2f &v);
	void setDir(float dir);
protected:
	int m_damage;
};

class RegularBullet: public Projectile
{
public:
	RegularBullet(sf::Vector2f pos, float dir, int faction);
	virtual void act(GameState& state);
	virtual bool isDead(const GameState &state);
protected:
	virtual void attack(GameState& state);
};

class SplittingBullet: public Projectile
{
public:
	SplittingBullet(sf::Vector2f pos, float dir, int hp, int faction);
	virtual void act(GameState& state);
	virtual bool isDead(const GameState &state);
protected:
	virtual void attack(GameState& state);
};

class Player: public Fighter
{
public:
	Player(sf::Vector2f pos, int hp);
	virtual void act(GameState& state);
private:
	virtual void attack(GameState& state);
	float m_max_vy, m_max_vx;
	bool m_firing;
};

class Grunt: public Fighter
{
public:
	Grunt(sf::Vector2f pos);
	virtual void act(GameState& state);
	virtual void hit(int damage, GameState& state);
protected:
	virtual void attack(GameState& state);
	sf::Vector2f m_max_v;
	int m_decision_cd;
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

#endif